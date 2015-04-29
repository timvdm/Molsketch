/***************************************************************************
 *   Copyright (C) 2007-2008 by Harm van Eersel                            *
 *   Copyright (C) 2009 Tim Vandermeersch                                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <QtGlobal>
#include <QPainter>
#include <QMenu>
#include <QAction>
#include <QGraphicsSceneMouseEvent>

#include <math.h>

#include "bond.h"

#include "atom.h"
#include "ring.h"
#include "element.h"
#include "molscene.h"
#include "math2d.h"
#include "molecule.h"

#include <QDebug>

#include <actions/bondtypeaction.h>
#include <actions/flipbondaction.h>

#define CHECKFORATOMS if (!m_beginAtom || !m_endAtom)

// Constructor

namespace Molsketch {

  int Bond::orderFromType(const Bond::BondType &type)
  {
    return type / 10;
  }

  Bond::BondType Bond::simpleTypeFromOrder(const int &order)
  {
    switch (order)
    {
      case 1: return Single;
      case 2: return Double;
      case 3: return Triple;
      default: return Invalid;
    }
  }

  Bond::Bond(Atom* atomA, Atom* atomB, Bond::BondType type, QGraphicsItem* parent GRAPHICSSCENESOURCE )
    : graphicsItem (parent GRAPHICSSCENEINIT ),
      m_bondType(type),
      m_beginAtom(0),
      m_endAtom(0),
      m_ring(0)
  {
    setAtoms(atomA, atomB);
  
    MolScene* molScene = dynamic_cast<MolScene*>(
#if QT_VERSION < 0x050000
                scene
#else
                scene()
#endif
                );
    if (molScene) 
      setColor(molScene->color());
    else 
      setColor(QColor(0, 0, 0));

    setZValue(2);

  //   setFlag(QGraphicsItem::ItemIsSelectable);
  //   setAcceptedMouseButtons(Qt::LeftButton);
  }
  
  Bond::~Bond()
  {
  }

  // Inherited methods

  QRectF Bond::boundingRect() const
  {
    CHECKFORATOMS return QRect() ;

    qreal w = m_endAtom->x() - m_beginAtom->x();
    qreal h = m_endAtom->y() - m_beginAtom->y();

    return QRectF(mapFromParent(m_beginAtom->pos()) - QPointF(5,5), QSizeF(w+10,h+10)).normalized();
  }

  qreal Bond::bondAngle(const Atom *origin) const
  {
    if (!m_endAtom || !m_beginAtom) return 0 ;
    return Molecule::toDegrees(bondAxis().angle() + (origin == endAtom()) * 180.) ;
  }

  QLineF Bond::bondAxis() const
  {
    return QLineF(mapFromParent(m_beginAtom->pos()),
                  mapFromParent(m_endAtom->pos())) ;
  }


  // draw a double bond with one line inside the ring
  void Bond::drawRingBond(QPainter *painter)
  {
    if (!m_ring) return ;

    qreal m_bondSpacing = 4.0;

    QPointF center = mapFromParent(m_ring->center());
    QPointF begin = mapFromParent(m_beginAtom->pos());
    QPointF end = mapFromParent(m_endAtom->pos());
    QPointF vb = end - begin;

    QPointF uvb = vb / sqrt(vb.x()*vb.x() + vb.y()*vb.y());
    QPointF orthogonal(uvb.y(), -uvb.x());
    QPointF spacing = orthogonal * m_bondSpacing;
    QPointF offset = uvb * m_bondSpacing;

    QPointF offset2 = 0.20 * uvb * 40/*molScene->bondLength()*/; //FIXME

    if (length(begin + spacing - center) > length(begin - spacing - center))
      spacing *= -1.0;
    
    if (!m_beginAtom->hasLabel() && !m_endAtom->hasLabel()) {
      // begin & end have no label
      painter->drawLine(QLineF(begin, end));
      painter->drawLine(QLineF(begin + spacing + offset, end + spacing - offset));
    } else if (m_beginAtom->hasLabel() && m_endAtom->hasLabel()) {
      painter->drawLine(QLineF(begin + offset2, end - offset2));
      painter->drawLine(QLineF(begin + spacing + offset2, end + spacing - offset2));
    } else if (m_beginAtom->hasLabel()) {
      painter->drawLine(QLineF(begin + offset2, end));
      painter->drawLine(QLineF(begin + spacing + offset2, end + spacing - offset));
    } else if (m_endAtom->hasLabel()) {
      painter->drawLine(QLineF(begin, end - offset2));
      painter->drawLine(QLineF(begin + spacing + offset, end + spacing - offset2));
    }

  }

  void Bond::drawHashBond(QPainter *painter)
  {
    qreal m_bondSpacing = 4.0;

    QPointF begin = mapFromParent(m_beginAtom->pos());
    QPointF end = mapFromParent(m_endAtom->pos());
    QPointF vb = end - begin;

    QPointF uvb = vb / sqrt(vb.x()*vb.x() + vb.y()*vb.y());
    QPointF orthogonal(uvb.y(), -uvb.x());
    orthogonal *= m_bondSpacing;

    qreal lines[5] = { 0.25, 0.40, 0.55, 0.70, 0.90 };

    int last = (m_endAtom->hasLabel()) ? 4 : 5;

    for (int i = 0; i < last; ++i) {
      qreal w = lines[i];
      painter->drawLine( QLineF(begin + w * (vb + orthogonal), begin + w * (vb - orthogonal)) );
    }
  }

  void Bond::drawWedgeBond(QPainter *painter)
  {
    qreal m_bondSpacing = 4.0;

    QPointF begin = mapFromParent(m_beginAtom->pos());
    QPointF end = mapFromParent(m_endAtom->pos());
    QPointF vb = end - begin;

    QPointF uvb = vb / sqrt(vb.x()*vb.x() + vb.y()*vb.y());
    QPointF orthogonal(uvb.y(), -uvb.x());
    orthogonal *= m_bondSpacing;

    int i = 0;
    QPointF points[4];
    if (m_beginAtom->hasLabel()) {
        points[i++] = begin + 0.25 * (vb - orthogonal);
        points[i++] = begin + 0.25 * (vb + orthogonal);
    } else {
        points[i++] = begin;
    }
    if (m_endAtom->hasLabel()) {
        points[i++] = end - 0.25 * vb + 0.75 * orthogonal;
        points[i++] = end - 0.25 * vb - 0.75 * orthogonal;
    } else {
        points[i++] = end + orthogonal;
        points[i++] = end - orthogonal;
    }

    painter->setBrush( QBrush(getColor()) );
    painter->drawConvexPolygon( points, i);
  }

  QPolygonF clipBond(const QPointF& atomPoint,
                     const QPointF& otherAtom,
                     const QPointF& normalVector)
  {
    QPointF bondVector = atomPoint - otherAtom;
    return QPolygonF() << otherAtom + normalVector
                       << otherAtom - normalVector
                       << otherAtom - normalVector + .7*bondVector
                       << otherAtom + normalVector + .7*bondVector
                       << otherAtom + normalVector
                          ;
  }

  void drawBrokenIndicator(QPainter* painter,
                           const QPointF& point,
                           const QPointF& bondVector,
                           const QPointF& normalVector)
  {
    painter->save();
    QPointF bondUnitVector(bondVector/QLineF(QPointF(0,0), bondVector).length());
    const qreal decorationScale = 0.2; // TODO modifiable
    QPointF
        x = normalVector*decorationScale,
        y = -8*bondUnitVector*decorationScale;
    QPainterPath path;
    path.moveTo(-7*x);
    path.quadTo(-7*x+y, -6*x+y);
    path.cubicTo(-5*x+y, -5*x, -4*x);
    path.cubicTo(-3*x, -3*x+y, -2*x+y);
    path.cubicTo(-x+y, -x, 0*x);
    path.cubicTo(x, x+y, 2*x+y);
    path.cubicTo(3*x+y, 3*x, 4*x);
    path.cubicTo(5*x, 5*x+y, 6*x+y);
    path.quadTo(7*x+y, 7*x);
    path.translate(point + 0.3 * bondVector);

    QPen subPen(painter->pen());
    subPen.setWidthF(subPen.widthF()* 0.75);
    painter->setPen(subPen);
    painter->drawPath(path);
    painter->restore();
  }
  void Bond::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
  {
    Q_UNUSED(option);
    Q_UNUSED(widget);

    CHECKFORATOMS return ;

    // Check the scene

    // Set painter defaults
    painter->save();
    QPen pen;
    pen.setWidthF(lineWidth());
    pen.setCapStyle(Qt::RoundCap);
    pen.setColor (getColor());
    painter->setPen(pen);

    // Get beginning and end
    QPointF begin = mapFromParent(m_beginAtom->pos());
    QPointF end = mapFromParent(m_endAtom->pos());
    QPointF vb = end - begin;
    QPointF uvb = vb / sqrt(vb.x()*vb.x() + vb.y()*vb.y());
    if (MolScene* s = qobject_cast<MolScene*>(scene()))
      uvb *= s->bondLength()/10.;
    QPointF normalVector(uvb.y(), -uvb.x());

    // clip for broken bond
    const bool beginBroken = m_beginAtom->element().isEmpty();
    const bool endBroken = m_endAtom->element().isEmpty();
    QPainterPath clipPath;
    if (beginBroken)
    {
      drawBrokenIndicator(painter, begin, vb, normalVector);
      clipPath.addPolygon(clipBond(begin, end, normalVector));
    }
    if (endBroken)
    {
      drawBrokenIndicator(painter, end, -vb, normalVector);
      clipPath.addPolygon(clipBond(end, begin, normalVector));
    }
    if (!clipPath.isEmpty())
      painter->setClipPath(clipPath);

    if (m_beginAtom->hasLabel())
      begin += 0.20 * uvb * 40/*molScene->bondLength()*/; // FIXME
    if (m_endAtom->hasLabel())
      end -= 0.20 * uvb * 40/*molScene->bondLength()*/; // FIXME

    switch ( m_bondType )
    {
      case Bond::DativeDot:
        pen.setStyle(Qt::DotLine);
        painter->setPen(pen);
        painter->drawLine(begin, end);
        break;
      case Bond::DativeDash:
        pen.setStyle(Qt::DashLine);
        painter->setPen(pen);
        painter->drawLine(begin, end);
        break;
      case Bond::Single:
        painter->drawLine(begin, end);
        break;
      case Bond::Wedge:
        drawWedgeBond(painter);
        break;
      case Bond::Hash:
        drawHashBond(painter);
        break;
      case Bond::WedgeOrHash:
        pen.setDashPattern(QVector<qreal>() << 2 << 5);
        painter->setPen(pen);
        painter->drawLine(begin, end);
        break;
      case Bond::Double:
        {
          QPointF offset = .5*normalVector;
          painter->drawLine(QLineF(begin + offset, end + offset));
          painter->drawLine(QLineF(begin - offset, end - offset));
          break;
        }
      case Bond::CisOrTrans:
        {
          QPointF offset = .5*normalVector;
          painter->drawLine(QLineF(begin + offset, end - offset));
          painter->drawLine(QLineF(begin - offset, end + offset));
          break;
        }
      case Bond::Triple:
        {
          QPointF offset = normalVector;
          painter->drawLine(QLineF(begin, end));
          painter->drawLine(QLineF(begin + offset, end + offset));
          painter->drawLine(QLineF(begin - offset, end - offset));
          break;
        }
      case TripleAsymmetric:
      default:
        ;
    }

    painter->setClipping(false);

    if (isSelected()) {
      // draw square at the midpoint of the bond when selected
      painter->setPen(Qt::blue);
      QPointF begin = mapFromParent(m_beginAtom->pos());
      QPointF end = mapFromParent(m_endAtom->pos());
      QPointF vb = end - begin;
      QPointF midPoint = begin + vb / 2;
      QPointF uvb = vb / sqrt(vb.x()*vb.x() + vb.y()*vb.y());
      QPointF orthogonal(uvb.y(), -uvb.x());

      painter->drawLine(QLineF(midPoint + 5.0 * orthogonal - 5.0 * uvb, midPoint - 5.0 * orthogonal - 5.0 * uvb));
      painter->drawLine(QLineF(midPoint + 5.0 * orthogonal + 5.0 * uvb, midPoint - 5.0 * orthogonal + 5.0 * uvb));

      painter->drawLine(QLineF(midPoint + 5.0 * orthogonal + 5.0 * uvb, midPoint + 5.0 * orthogonal - 5.0 * uvb));
      painter->drawLine(QLineF(midPoint - 5.0 * orthogonal + 5.0 * uvb, midPoint - 5.0 * orthogonal - 5.0 * uvb));
    }


    //painter->drawPolygon(shape().toFillPolygon());

    // Restore old painter
    painter->restore();
  }

  QVariant Bond::itemChange(GraphicsItemChange change, const QVariant &value)
  {
    if (change == ItemPositionChange && parentItem()) parentItem()->update();
    return graphicsItem::itemChange(change, value);
  }

  QPainterPath Bond::shape() const
  {
    CHECKFORATOMS return QPainterPath() ;
    QPolygonF polygon;
    polygon << shiftVector(QLineF(mapFromParent(m_beginAtom->pos()),mapFromParent(m_endAtom->pos())),10).p1()
    << shiftVector(QLineF(mapFromParent(m_beginAtom->pos()),mapFromParent(m_endAtom->pos())),10).p2()
    << shiftVector(QLineF(mapFromParent(m_beginAtom->pos()),mapFromParent(m_endAtom->pos())),-10).p2() << shiftVector(QLineF(mapFromParent(m_beginAtom->pos()),mapFromParent(m_endAtom->pos())),-10).p1();

    QPainterPath path(mapFromParent(m_beginAtom->pos()));
    // path.quadTo(QPointF(),m_endAtom->pos());
    path.addPolygon( polygon );
    path.closeSubpath();

    return path;
  }

  // Manipulation methods

  void Bond::setType(const BondType &t)
  {
    m_bondType = t;
    update();
  }


  // Query methods

  int Bond::bondOrder() const
  {
    return orderFromType(bondType());
  }

  Bond::BondType Bond::bondType() const
  {
    return m_bondType;
  }

  Atom* Bond::beginAtom() const
  {
    return m_beginAtom;
  }

  Atom* Bond::endAtom() const
  {
    return m_endAtom;
  }

  bool Bond::hasAtom(const Atom* atom) const
  {
    return m_beginAtom == atom || m_endAtom == atom;
  }
    
  Atom* Bond::otherAtom(const Atom *atom) const
  {
    return (atom == m_beginAtom) ? m_endAtom : m_beginAtom;
  }

  void Bond::setAtoms(Atom *A, Atom *B)
  {
    if (m_beginAtom) m_beginAtom->removeBond(this) ;
    if (m_endAtom)   m_endAtom  ->removeBond(this);
    m_beginAtom = A ;
    m_endAtom = B ;
    if (m_beginAtom)
    {
      m_beginAtom->addBond(this) ;
      setPos(m_beginAtom->scenePos()) ;
    }
    if (m_endAtom)   m_endAtom  ->addBond(this);
  }
  
  Molecule* Bond::molecule() const
  {
    return dynamic_cast<Molecule*>(this->parentItem());
  }

  // Auxilary methods

  QLineF Bond::shiftVector(const QLineF &vector, qreal shift) // Shifts a vector on the perpendicular axis
  {
    //pre: true
    //ret: shifted vector

    // Calculating the new coordinates
    qreal rx1 = vector.x1() + shift*(vector.unitVector().y2()-vector.unitVector().y1());
    qreal ry1 = vector.y1() + shift*-(vector.unitVector().x2()-vector.unitVector().x1());
    qreal rx2 = vector.x2() + shift*(vector.unitVector().y2()-vector.unitVector().y1());
    qreal ry2 = vector.y2() + shift*-(vector.unitVector().x2()-vector.unitVector().x1());

    // Returning the new vector
    return QLineF(rx1,ry1,rx2,ry2);
  }

  void Bond::setCoordinates(const QVector<QPointF> &c)
  {
    if (c.size() != 2) return ;
    m_beginAtom->setCoordinates(c.mid(0,1)) ;
    m_endAtom->setCoordinates(c.mid(1,1));
  }

  QPolygonF Bond::coordinates() const
  {
    return QVector<QPointF>()
        << m_beginAtom->coordinates()
        << m_endAtom->coordinates() ;
  }

  QXmlStreamAttributes Bond::graphicAttributes() const
  {
    QXmlStreamAttributes attributes ;

    QString atomOne = molecule()->atomId(m_beginAtom),
        atomTwo = molecule()->atomId(m_endAtom) ;

    attributes.append("atomRefs2", atomOne + " " + atomTwo) ;
    attributes.append("type", QString::number(m_bondType));

    return attributes ;
  }

  void Bond::readGraphicAttributes(const QXmlStreamAttributes &attributes)
  {
    QStringList atomIndexes = attributes.value("atomRefs2").toString().split(" ") ;
    if (atomIndexes.size() != 2) return ;

    setAtoms(molecule()->atom(atomIndexes.first()),
             molecule()->atom(atomIndexes.last())) ;
    m_bondType = (BondType) (attributes.value("type").toString().toInt());
  }

  QStringList Bond::textItemAttributes() const
  {
    return QStringList() << "bondStereo" ;
  }

  void Bond::prepareContextMenu(QMenu *contextMenu)
  {
    // Prepare bond menu
    MolScene *sc = qobject_cast<MolScene*>(scene());
    if (sc)
    {
      bondTypeAction* action = sc->findChild<bondTypeAction*>();
      if (action)
      {
        contextMenu->addAction(action);
        QObject::connect(action, SIGNAL(triggered()), contextMenu, SLOT(close())); // TODO check if "changed()" event can accomplish this
      }
      flipBondAction *flipAction = sc->findChild<flipBondAction*>();
      if (flipAction) contextMenu->addAction(flipAction);
    }
    graphicsItem::prepareContextMenu(contextMenu);
  }

  qreal Bond::defaultLength = 40 ; // TODO modifiable
  qreal Bond::defaultAngle = 30 ; // TODO modifiable

} // namespace
