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
#include <math.h>

#include "bond.h"

#include "atom.h"
#include "ring.h"
#include "element.h"
#include "molscene.h"
#include "math2d.h"

#include <QDebug>

// Constructor

namespace Molsketch {

  Bond::Bond(Atom* atomA, Atom* atomB, int order, Bond::BondType type, QGraphicsItem* parent,QGraphicsScene* scene) : QGraphicsItem(parent,scene)
  {
    Q_CHECK_PTR(atomA);
    Q_CHECK_PTR(atomB);

    m_bondType = type;
    m_bondOrder = order;
    m_beginAtom = atomA;
    m_endAtom = atomB;
    m_ring = 0;

    atomA->addBond(this);
    atomB->addBond(this);
  
    setPos(m_beginAtom->scenePos());
    MolScene* molScene = dynamic_cast<MolScene*>(scene);
    if (molScene) 
      setColor(molScene->color());
    else 
      setColor(QColor(0, 0, 0));

  //   setFlag(QGraphicsItem::ItemIsSelectable);
  //   setAcceptedMouseButtons(Qt::LeftButton);
  }
  
  Bond::~Bond()
  {
    //m_beginAtom->removeNeighbor(m_endAtom);
    //m_endAtom->removeNeighbor(m_beginAtom);
  }

  // Inherited methods

  QRectF Bond::boundingRect() const
  {
    Q_CHECK_PTR(m_beginAtom);
    Q_CHECK_PTR(m_endAtom);

    qreal w = m_endAtom->x() - m_beginAtom->x();
    qreal h = m_endAtom->y() - m_beginAtom->y();

    // 	qreal x = qMax(m_beginAtom->pos().x(),m_endAtom->pos().x());
    // 	qreal y = qMax(m_beginAtom->pos().y(),m_beginAtom->pos().y());
    return QRectF(mapFromParent(m_beginAtom->pos()) - QPointF(5,5), QSizeF(w+10,h+10));
  }

  // draw single, double and triple bonds
  void Bond::drawSimpleBond(QPainter *painter)
  {
    if (m_ring && (m_bondOrder == 2)) {
      // draw double bond inside ring
      drawRingBond(painter);
      return;
    }

    Q_CHECK_PTR(m_beginAtom);
    Q_CHECK_PTR(m_endAtom);

    qreal m_bondSpacing = 4.0;

    QPointF begin = mapFromParent(m_beginAtom->pos());
    QPointF end = mapFromParent(m_endAtom->pos());
    QPointF vb = end - begin;
    QPointF uvb = vb / sqrt(vb.x()*vb.x() + vb.y()*vb.y());

    MolScene* molScene = dynamic_cast<MolScene*>(scene());
	  
    if (m_beginAtom->hasLabel())
      begin += 0.20 * uvb * molScene->bondLength();
    if (m_endAtom->hasLabel())
      end -= 0.20 * uvb * molScene->bondLength();

    switch (m_bondOrder) {
      case 1:
        painter->drawLine(QLineF(begin, end));
        break;
      case 2:
      {
        QPointF orthogonal(uvb.y(), -uvb.x());
        QPointF offset = orthogonal * 0.5 * m_bondSpacing;
        if (m_bondType == CisOrTrans) {
          painter->drawLine(QLineF(begin + offset, end - offset));
          painter->drawLine(QLineF(begin - offset, end + offset));
        } else {
          painter->drawLine(QLineF(begin + offset, end + offset));
          painter->drawLine(QLineF(begin - offset, end - offset));
        }
        break;
      }
      case 3:
      {
        QPointF orthogonal(uvb.y(), -uvb.x());
        QPointF offset = orthogonal * m_bondSpacing;
        painter->drawLine(QLineF(begin, end));
        painter->drawLine(QLineF(begin + offset, end + offset));
        painter->drawLine(QLineF(begin - offset, end - offset));
        break;
      }
    }
  }

  // draw a double bond with one line inside the ring
  void Bond::drawRingBond(QPainter *painter)
  {
    Q_CHECK_PTR(m_beginAtom);
    Q_CHECK_PTR(m_endAtom);
    Q_CHECK_PTR(m_ring);

    qreal m_bondSpacing = 4.0;

    QPointF center = mapFromParent(m_ring->center());
    QPointF begin = mapFromParent(m_beginAtom->pos());
    QPointF end = mapFromParent(m_endAtom->pos());
    QPointF vb = end - begin;

    QPointF uvb = vb / sqrt(vb.x()*vb.x() + vb.y()*vb.y());
    QPointF orthogonal(uvb.y(), -uvb.x());
    QPointF spacing = orthogonal * m_bondSpacing;
    QPointF offset = uvb * m_bondSpacing;
    if (length(begin + spacing - center) > length(begin - spacing - center))
      spacing *= -1.0;
    
    painter->drawLine(QLineF(begin, end));
    painter->drawLine(QLineF(begin + spacing + offset, end + spacing - offset));
  }

  void Bond::drawHashBond(QPainter *painter, bool inverted)
  {
    Q_CHECK_PTR(m_beginAtom);
    Q_CHECK_PTR(m_endAtom);

    qreal m_bondSpacing = 4.0;

    QPointF begin = mapFromParent(m_beginAtom->pos());
    QPointF end = mapFromParent(m_endAtom->pos());
    if (inverted) {
      QPointF swap = begin;
      begin = end;
      end = swap;
    }
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

  void Bond::drawWedgeBond(QPainter *painter, bool inverted)
  {
    Q_CHECK_PTR(m_beginAtom);
    Q_CHECK_PTR(m_endAtom);

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
      if (!inverted) {
        points[i++] = begin + 0.25 * (vb - orthogonal);
        points[i++] = begin + 0.25 * (vb + orthogonal);
      } else {
        points[i++] = begin + 0.25 * vb + 0.75 * orthogonal;
        points[i++] = begin + 0.25 * vb - 0.75 * orthogonal;
      }
    } else {
      if (!inverted) {
        points[i++] = begin;
      } else {
        points[i++] = begin + orthogonal;
        points[i++] = begin - orthogonal;
      }
    }
    if (m_endAtom->hasLabel()) {
      if (inverted) {
        points[i++] = end - 0.25 * (vb + orthogonal);
        points[i++] = end - 0.25 * (vb - orthogonal);
      } else {
        points[i++] = end - 0.25 * vb + 0.75 * orthogonal;
        points[i++] = end - 0.25 * vb - 0.75 * orthogonal;
      }
    } else {
      if (inverted) {
        points[i++] = end;
      } else {
        points[i++] = end + orthogonal;
        points[i++] = end - orthogonal;
      }
    }

    painter->setBrush( QBrush(m_color) );
    painter->drawConvexPolygon( points, i);
  }

  void Bond::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
  {
    Q_UNUSED(option);
    Q_UNUSED(widget);

    // Check the scene
    MolScene* molScene = dynamic_cast<MolScene*>(scene());
    Q_CHECK_PTR(molScene);

    // Set painter defaults
    painter->save();
    QPen pen;
    pen.setWidthF(molScene->bondWidth());
    pen.setColor (m_color);
    painter->setPen(pen);


    // Create dash pattern for dot
    QVector<qreal> dash;
    dash << 2 << 5;

    switch ( m_bondType )
    {
      case Bond::Hash:
        drawHashBond(painter, false);
        break;
      case Bond::InvertedHash:
        drawHashBond(painter, true);
        break;
      case Bond::WedgeOrHash:
        pen.setDashPattern(dash);
        painter->setPen(pen);
        painter->drawLine(QLineF(mapFromParent(m_beginAtom->pos()),mapFromParent(m_endAtom->pos())));
        break;
      case Bond::Wedge:
        drawWedgeBond(painter, false);
        break;
      case Bond::InvertedWedge:
        drawWedgeBond(painter, true);
        break;

      default:
        drawSimpleBond(painter);
    }

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
    return QGraphicsItem::itemChange(change, value);
  }

  QPainterPath Bond::shape() const
  {
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

  void Bond::setOrder(int order)
  {
    //pre: order>0
    //post: m_bondOrder=order

    Q_ASSERT( order > 0 );
    m_bondOrder = order;
    molecule()->perceiveRings();
    molecule()->invalidateElectronSystems();
    update();
  }

  void Bond::incOrder()
  {
    //pre: true
    //post: m_bondOrder = oldBondOrder % 3 + 1

    // Calculating the new order
    setOrder( m_bondOrder % 3 + 1 );
  }

  void Bond::decOrder()
  {
    //pre: true
    //post: m_bondOrder = (oldBondOrder + 1) % 3 + 1

    // Calculating the new order
    setOrder( (m_bondOrder + 1) % 3 + 1 );
  }

  void Bond::setType(Bond::BondType t)
  {
    Q_ASSERT(0 <= t && t < Bond::NoType);

    m_bondType = t;
    // adjust the order if needed
    /*
    switch (t) {
      case Wedge:
      case InvertedWedge:
      case Hash:
      case InvertedHash:
      case WedgeOrHash:
        setOrder(1);
        break;
      case CisOrTrans:
        setOrder(2);
        break;
      default:
        break;
    }
*/

    update();
  }

  /*
  void Bond::incType()
  {
    //pre: true
    //post: bondType = bondType % 6 + 1
    setType((m_bondType + 1) % NoType);
  }

  void Bond::decType()
  {
    //pre: true
    //post: bondType = (bondType + 5) % 6
    setType( (m_bondType + NoType - 1) % NoType);
  }
  */

  // Query methods

  int Bond::bondOrder() const
  {
    return m_bondOrder;
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

} // namespace
