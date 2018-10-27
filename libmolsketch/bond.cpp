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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#include <QtGlobal>
#include <QPainter>
#include <QMenu>
#include <QAction>
#include <QGraphicsSceneMouseEvent>

#include <math.h>

#include "bond.h"

#include "atom.h"
#include "element.h"
#include "molscene.h"
#include "math2d.h"
#include "molecule.h"
#include <actions/bondtypeaction.h>
#include <actions/flipbondaction.h>
#include <actions/flipstereobondsaction.h>
#include "scenesettings.h"
#include "settingsitem.h"
#include <QDebug>

#define CHECKFORATOMS if (!m_beginAtom || !m_endAtom || !molecule())

namespace Molsketch {

  int Bond::orderFromType(const Bond::BondType &type) {
    return type / 10;
  }

  Bond::BondType Bond::simpleTypeFromOrder(const int &order)
  {
    switch (order)
    {
      case 1: return Single;
      case 2: return DoubleLegacy;
      case 3: return Triple;
      default: return Invalid;
    }
  }

  Bond::Bond(Atom* atomA, Atom* atomB, Bond::BondType type, QGraphicsItem* parent GRAPHICSSCENESOURCE )
    : graphicsItem (parent GRAPHICSSCENEINIT ),
      m_bondType(type),
      m_beginAtom(0),
      m_endAtom(0)
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
      setColor(molScene->settings()->defaultColor()->get());
    else
      setColor(QColor(0, 0, 0));

    setZValue(2);
  }

  Bond::Bond(const Bond &other, Atom* atomA, Atom* atomB)
    : graphicsItem(other),
      m_bondType(other.m_bondType),
      m_beginAtom(0),
      m_endAtom(0)
  {
    setAtoms(atomA, atomB);
  }

  Bond::~Bond() {}

  QRectF Bond::boundingRect() const
  {
    CHECKFORATOMS return QRect() ;
    qreal w = m_endAtom->x() - m_beginAtom->x();
    qreal h = m_endAtom->y() - m_beginAtom->y();
    return QRectF(mapFromParent(m_beginAtom->pos()) - QPointF(5,5), QSizeF(w+10,h+10)).normalized();
  }

  qreal Bond::bondAngle(const Atom *origin) const
  {
    CHECKFORATOMS return 0 ;
    return Molecule::toDegrees(bondAxis().angle() + (origin == endAtom()) * 180.) ;
  }

  QLineF Bond::bondAxis() const
  {
    CHECKFORATOMS return QLineF();
    return QLineF(mapFromParent(m_beginAtom->pos()),
                  mapFromParent(m_endAtom->pos())) ;
  }

  QPainterPath Bond::drawHashBond(const QPointF &begin, const QPointF &end) const
  {
    qreal m_bondSpacing = 4.0;

//    QPointF begin = mapFromParent(m_beginAtom->pos());
//    QPointF end = mapFromParent(m_endAtom->pos());
    QPointF vb = end - begin;

    QPointF uvb = vb / sqrt(vb.x()*vb.x() + vb.y()*vb.y());
    QPointF orthogonal(uvb.y(), -uvb.x());
    orthogonal *= m_bondSpacing;

    qreal lines[5] = { 0.25, 0.40, 0.55, 0.70, 0.90 };

    int last = (m_endAtom->hasLabel()) ? 4 : 5;

    QPainterPath result;
    for (int i = 0; i < last; ++i) {
      qreal w = lines[i];
      result.moveTo(begin + w * (vb + orthogonal));
      result.lineTo(begin + w * (vb - orthogonal));
    }
    return result;
  }

  QPainterPath Bond::drawWedgeBond(const QPointF &begin, const QPointF &end) const
  {
    auto axis = bondAxis();
    auto normalVector = axis.normalVector().unitVector();
    if (MolScene* s = qobject_cast<MolScene*>(scene()))
      normalVector.setLength(s->settings()->bondWedgeWidth()->get()/2.);
    normalVector.translate(axis.dx(), axis.dy());

    QLineF outer1{axis.p1(), normalVector.p2()},
           outer2{axis.p1(), normalVector.pointAt(-1)};
    QLineF outer1Begin{mapToItem(beginAtom(), outer1.p1()), mapToItem(beginAtom(), outer1.p2())};
    QLineF outer2Begin{mapToItem(beginAtom(), outer2.p1()), mapToItem(beginAtom(), outer2.p2())};
    qreal beginExtent = beginAtom()->getBondExtent(outer1Begin, outer2Begin, lineWidth());

    QLineF outer1End{mapToItem(endAtom(), outer1.p2()), mapToItem(endAtom(), outer1.p1())};
    QLineF outer2End{mapToItem(endAtom(), outer2.p2()), mapToItem(endAtom(), outer2.p1())};
    qreal endExtent = 1 - endAtom()->getBondExtent(outer1End, outer2End, lineWidth());

    QPainterPath path;
    path.moveTo(outer1.pointAt(beginExtent));
    path.lineTo(outer1.pointAt(endExtent));
    path.lineTo(outer2.pointAt(endExtent));
    path.lineTo(outer2.pointAt(beginExtent));
    path.closeSubpath();
    return path;
  }

  QPainterPath Bond::getWedgeBondShape() const {
//    QPointF begin = mapFromParent(m_beginAtom->pos());
//    QPointF end = mapFromParent(m_endAtom->pos());
//    QPointF vb = end - begin;

//    QPointF uvb = vb / sqrt(vb.x()*vb.x() + vb.y()*vb.y());
//    QPointF orthogonal(uvb.y(), -uvb.x());
//    QPointF bondUnitVector(uvb);
//    QPointF bondNormalVector(orthogonal);
//    if (MolScene* s = qobject_cast<MolScene*>(scene()))
//      orthogonal *= s->settings()->bondWedgeWidth()->get();
//    qreal gap = 2;

//    QPainterPath path;
//    if (m_beginAtom->hasLabel()) {
//      path.moveTo(begin + 0.25 * (vb - orthogonal) + gap * (-bondUnitVector - bondNormalVector));
//      path.lineTo(begin + 0.25 * (vb + orthogonal) + gap * (-bondUnitVector + bondNormalVector));
//    } else {
//      path.moveTo(begin + gap * (-bondUnitVector - bondNormalVector));
//      path.lineTo(begin + gap * (-bondUnitVector + bondNormalVector));
//    }
//    if (m_endAtom->hasLabel()) {
//      path.lineTo(end - 0.25 * vb + 0.75 * orthogonal + gap * (bondUnitVector + bondNormalVector));
//      path.lineTo(end - 0.25 * vb - 0.75 * orthogonal + gap * (bondUnitVector - bondNormalVector));
//    } else {
//      path.lineTo(end + orthogonal + gap * (bondUnitVector + bondNormalVector));
//      path.lineTo(end - orthogonal + gap * (bondUnitVector - bondNormalVector));
//    }
//    path.closeSubpath();
//    return path;

    QPainterPath path;
    path.addRect(-1000, -1000, 2000, 2000);
    return path;
  }

  double minimumAngle(const Bond* reference, const QSet<Bond*>& others, const Atom* origin, bool clockwise)
  {
    double minAngle = 361;
    for (auto b : others)
    {
      double angle = b->bondAngle(origin) - reference->bondAngle(origin);
      if (clockwise) angle = 360 - angle;
      angle = Molecule::toDegrees(angle);
      minAngle = qMin(minAngle, angle);
    }
    return minAngle;
  }

  void Bond::determineDoubleBondOrientation()
  {
    if (m_bondType != DoubleLegacy) return;
    m_bondType = DoubleSymmetric;
    QSet<Bond*> beginBonds = m_beginAtom->bonds().toSet();
    beginBonds -= this;
    QSet<Bond*> endBonds = m_endAtom->bonds().toSet();
    endBonds -= this;
    // no other bonds: symmetric
    if (beginBonds.empty() && endBonds.empty()) return;
    // other bonds: add up angles upper and lower
    double sumUpperAngles = minimumAngle(this, beginBonds, m_beginAtom, false)
        + minimumAngle(this, endBonds, m_endAtom, true);
    double sumLowerAngles = minimumAngle(this, beginBonds, m_beginAtom, true)
        + minimumAngle(this, endBonds, m_endAtom, false);
    // equal (float tolerance): symmetric
    if (qAbs(sumUpperAngles - sumLowerAngles) < 1e-7) return;
    // else: unsymmetric
    m_bondType = DoubleAsymmetric;
    // consider swapping atoms
    if (sumUpperAngles > sumLowerAngles) qSwap(m_beginAtom, m_endAtom);
  }

  QLineF effectiveBondLine(const Bond* b, const Atom* a)
  {
    QLineF bl(b->bondAxis());
    if (b->beginAtom() != a)
      return QLineF(bl.p2(), bl.p1());
    return bl;
  }

  qreal findIdealAngle(const Atom* atom, const Bond* bond, bool inverted)
  {
    qreal angle = 120; // normal angle (e.g. in benzene)
    QLineF bondLine(effectiveBondLine(bond, atom));
    // TODO skip if we have a displayed label for the beginAtom
    foreach (const Bond* otherBond, atom->bonds())
    {
      if (otherBond == bond) continue;
      QLineF otherBondAxis(effectiveBondLine(otherBond, atom));
      angle = qMin(angle, (inverted
                           ? otherBondAxis.angleTo(bondLine)
                           : bondLine.angleTo(otherBondAxis)));
    }
    return angle*M_PI/360.; // includes division by 2
  }

  QPainterPath Bond::bondPath() const {
    // Get beginning and end
    QPointF begin = determineBondDrawingStart(m_beginAtom, m_endAtom);
    QPointF end = determineBondDrawingStart(m_endAtom, m_beginAtom);
    QPointF vb = end - begin;
    QPointF uvb = vb / sqrt(vb.x()*vb.x() + vb.y()*vb.y());
    if (MolScene* s = qobject_cast<MolScene*>(scene()))
      uvb *= s->settings()->bondSeparation()->get();
    QPointF normalVector(uvb.y(), -uvb.x());

    QPainterPath result;
    switch ( m_bondType ) // TODO beautify
    {
      case Bond::Single:
      case Bond::DativeDot:
      case Bond::DativeDash:
      case Bond::WedgeOrHash:
        result.moveTo(begin);
        result.lineTo(end);
        break;
      case Bond::Wedge:
        return drawWedgeBond(mapFromParent(m_beginAtom->pos()), mapFromParent(m_endAtom->pos()));
      case Bond::Hash:
        return drawHashBond(mapFromParent(m_beginAtom->pos()), mapFromParent(m_endAtom->pos()));
      case Bond::DoubleSymmetric:
        {
          QPointF offset = .5*normalVector;
          result.moveTo(begin + offset);
          result.lineTo(end + offset);
          result.moveTo(begin - offset);
          result.lineTo(end - offset);
          break;
        }
      case Bond::CisOrTrans:
        {
          QPointF offset = .5*normalVector;
          result.moveTo(begin + offset);
          result.lineTo(end - offset);
          result.moveTo(begin - offset);
          result.lineTo(end + offset);
          break;
        }
      case Bond::DoubleAsymmetric:
        {
          result.moveTo(begin);
          result.lineTo(end);
          // now the double part
          QPointF offset = normalVector;
          qreal beginAngle = findIdealAngle(beginAtom(), this, false),
              endAngle = findIdealAngle(endAtom(), this, true),
              limitAngle = atan(2*QLineF(QPointF(0,0),uvb).length()/QLineF(begin, end).length());
          beginAngle = qMax(beginAngle, limitAngle);
          endAngle = qMax(endAngle, limitAngle);
          result.moveTo(begin + uvb/tan(beginAngle) + offset);
          result.lineTo(end - uvb/tan(endAngle) + offset);
          break;
        }
      case Bond::Triple:
        {
          result.moveTo(begin);
          result.lineTo(end);
          QPointF offset = normalVector;
          result.moveTo(begin + offset);
          result.lineTo(end + offset);
          result.moveTo(begin - offset);
          result.lineTo(end - offset);
          break;
        }
      case TripleAsymmetric:
        {
          result.moveTo(begin);
          result.lineTo(end);
          // now the double part
          QPointF offset = normalVector;
          qreal beginAngle = findIdealAngle(beginAtom(), this, false),
              endAngle = findIdealAngle(endAtom(), this, true),
              limitAngle = atan(2*QLineF(QPointF(0,0),uvb).length()/QLineF(begin, end).length());
          beginAngle = qMax(beginAngle, limitAngle);
          endAngle = qMax(endAngle, limitAngle);
          result.moveTo(begin + uvb/tan(beginAngle) + offset);
          result.lineTo(end - uvb/tan(endAngle) + offset);
          beginAngle = findIdealAngle(beginAtom(), this, true);
          endAngle = findIdealAngle(endAtom(), this, false);
          beginAngle = qMax(beginAngle, limitAngle);
          endAngle = qMax(endAngle, limitAngle);
          result.moveTo(begin + uvb/tan(beginAngle) - offset);
          result.lineTo(end - uvb/tan(endAngle) - offset);
          break;
        }
      default: ;
    }
    return result;
  }

  QPointF Bond::determineBondDrawingStart(Atom *start, Atom *end) const {
    return mapFromScene(start->bondDrawingStart(end, lineWidth()));
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

    if (m_bondType == DoubleLegacy) determineDoubleBondOrientation();

    QRectF startRect = m_beginAtom->mapRectToItem(this, m_beginAtom->boundingRect()),
        endRect = m_endAtom->mapRectToItem(this, m_endAtom->boundingRect());
    if (startRect.intersects(endRect) && m_beginAtom->isDrawn() && m_endAtom->isDrawn()) return; // TODO should not be necessary anymore


    // Get beginning and end
    QPointF begin = mapFromParent(m_beginAtom->pos());
    QPointF end = mapFromParent(m_endAtom->pos());
    QPointF vb = end - begin;
    QPointF uvb = vb / sqrt(vb.x()*vb.x() + vb.y()*vb.y());
    if (MolScene* s = qobject_cast<MolScene*>(scene()))
      uvb *= s->settings()->bondSeparation()->get();
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

    begin = determineBondDrawingStart(m_beginAtom, m_endAtom);
    end = determineBondDrawingStart(m_endAtom, m_beginAtom);
    // TODO this collision detection rests on the factor used in Atom for the determination of the starting point. Improve!
    if (m_beginAtom->contains(mapToItem(m_beginAtom, end
                                        + 0.75 * lineWidth() * QLineF(QPointF(), m_beginAtom->pos() - m_endAtom->pos()).unitVector().p2()))
        || m_endAtom->contains(mapToItem(m_endAtom, begin
                                         + 0.75* lineWidth() * QLineF(QPointF(), m_endAtom->pos() - m_beginAtom->pos()).unitVector().p2())))
      return;


    QPen pen;

    painter->save();
    pen.setWidthF(lineWidth());
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    pen.setColor (getColor());
    switch ( m_bondType ) // TODO beautify
    {
      case Bond::DativeDot:
        pen.setStyle(Qt::DotLine);
        break;
      case Bond::DativeDash:
        pen.setStyle(Qt::DashLine);
        break;
      case Bond::WedgeOrHash:
        pen.setDashPattern(QVector<qreal>() << 2 << 5);
        break;
      case Bond::Wedge:
          painter->setBrush( QBrush(getColor()) );
      default: ;
    }
    painter->setPen(pen);

    QPainterPath coveringShapes;
    for (auto coveringItem : collidingItems())
      if (coveringItem->zValue() > zValue())
        if (auto coveringBond = dynamic_cast<Bond*>(coveringItem))
          coveringShapes = coveringShapes.united(mapFromItem(coveringBond, coveringBond->shape()));

    painter->setClipPath(shape().subtracted(coveringShapes));

    QPainterPath path = bondPath();
    painter->drawPath(path);

    painter->setClipping(false);

    if (isSelected()) {
      painter->setPen(Qt::blue);
      painter->drawPath(shape());
    }
    painter->restore();

    graphicsItem::paint(painter, option, widget);
  }

  QVariant Bond::itemChange(GraphicsItemChange change, const QVariant &value)
  {
    if (change == ItemPositionChange && parentItem()) parentItem()->update();
    return graphicsItem::itemChange(change, value);
  }

  QPainterPath Bond::shape() const
  {
    CHECKFORATOMS return QPainterPath() ;
    return outline();
  }

  void Bond::setType(const BondType &t)
  {
    m_bondType = t;
    update();
  }

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
    m_beginAtom = A ;
    m_endAtom = B ;
    if (m_beginAtom) {
      m_beginAtom->updateShape();
      setPos(m_beginAtom->scenePos()) ;
    }
    if (m_endAtom) m_endAtom->updateShape();
  }

  void Bond::setAtoms(const QPair<Atom *, Atom *> &atoms) {
    setAtoms(atoms.first, atoms.second);
  }

  QPair<Atom *, Atom *> Bond::atoms() const {
    return qMakePair(beginAtom(), endAtom());
  }

  Molecule* Bond::molecule() const
  {
    return dynamic_cast<Molecule*>(this->parentItem());
  }

  QLineF Bond::shiftVector(const QLineF &vector, qreal shift) // Shifts a vector on the perpendicular axis
  {
    qreal rx1 = vector.x1() + shift*(vector.unitVector().y2()-vector.unitVector().y1());
    qreal ry1 = vector.y1() + shift*-(vector.unitVector().x2()-vector.unitVector().x1());
    qreal rx2 = vector.x2() + shift*(vector.unitVector().y2()-vector.unitVector().y1());
    qreal ry2 = vector.y2() + shift*-(vector.unitVector().x2()-vector.unitVector().x1());
    return QLineF(rx1,ry1,rx2,ry2);
  }

  QString Bond::xmlName() const { return xmlClassName(); }

  QString Bond::xmlClassName() { return "bond" ; }

  void Bond::setCoordinates(const QVector<QPointF> &c)
  {
    if (c.size() != 2) return ;
    CHECKFORATOMS return;
    m_beginAtom->setCoordinates(c.mid(0,1)) ;
    m_endAtom->setCoordinates(c.mid(1,1));
  }

  QPolygonF Bond::coordinates() const
  {
   CHECKFORATOMS return QVector<QPointF>();
    return QVector<QPointF>()
        << m_beginAtom->coordinates()
        << m_endAtom->coordinates() ;
  }

  QXmlStreamAttributes Bond::graphicAttributes() const
  {
    QXmlStreamAttributes attributes ;
    attributes.append("atomRefs2", m_beginAtom->index() + " " + m_endAtom->index()) ;
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
    if (attributes.hasAttribute("order"))
      m_bondType = (BondType) (10 *attributes.value("order").toInt());
  }

  void Bond::prepareContextMenu(QMenu *contextMenu) // TODO simply use a function that returns the scene's actions pertaining to the item
  {
    // Prepare bond menu
    MolScene *sc = qobject_cast<MolScene*>(scene());
    if (sc)
    {
      QList<QAction*> actions;
      actions << sc->findChild<bondTypeAction*>()
              << sc->findChild<flipBondAction*>()
              << sc->findChild<flipStereoBondsAction*>();
      foreach(QAction* action, actions)
      {
        if (!action) continue;
        QObject::connect(action, SIGNAL(triggered()), contextMenu, SLOT(close())); // TODO check if "changed()" event can accomplish this. Only required for bondType action
        contextMenu->addAction(action);
      }
    }
    graphicsItem::prepareContextMenu(contextMenu);
  }

  class LegacyBondStereo : public XmlObjectInterface {
    Bond *bond;
  public:
    LegacyBondStereo(Bond *bond) : bond(bond) {}
    QXmlStreamReader& readXml(QXmlStreamReader &in) {
      auto text = in.readElementText();
      if ("H" == text) bond->setType(Bond::Hash);
      if ("W" == text) bond->setType(Bond::Wedge);
      return in;
    }

    QXmlStreamWriter& writeXml(QXmlStreamWriter &out) const {
      return out;
    }
  };

  XmlObjectInterface *Bond::produceChild(const QString &name, const QString &type) {
    if (name != "bondStereo" || !type.isEmpty()) return nullptr;
    XmlObjectInterface *helper = new LegacyBondStereo(this);
    helpers << helper;
    return helper;
  }

  void Bond::afterReadFinalization()
  {
    for (auto helper : helpers) delete helper;
    helpers.clear();
  }

  QPainterPath Bond::outline() const {
    // Get beginning and end (taken from bondPath()
    QPointF begin = determineBondDrawingStart(m_beginAtom, m_endAtom);
    QPointF end = determineBondDrawingStart(m_endAtom, m_beginAtom);
    QPointF vb = end - begin;
    QPointF uvb = vb / sqrt(vb.x()*vb.x() + vb.y()*vb.y());
    QPointF bondUnitVector(uvb);
    QPointF bondNormalVector(uvb.y(), -uvb.x());
    if (MolScene* s = qobject_cast<MolScene*>(scene()))
      uvb *= s->settings()->bondSeparation()->get();
    QPointF normalVector(uvb.y(), -uvb.x());

    QPainterPath result;

    qreal gap = 2;

    switch (m_bondType) {
      case Bond::Single:
      case Bond::DativeDot:
      case Bond::DativeDash:
      case WedgeOrHash:
        result.moveTo(begin + gap * (-bondUnitVector + bondNormalVector));
        result.lineTo(begin + gap * (-bondUnitVector - bondNormalVector));
        result.lineTo(end + gap * (bondUnitVector - bondNormalVector));
        result.lineTo(end + gap * (bondUnitVector + bondNormalVector));
        result.closeSubpath();
        break;
      case Bond::Wedge:
      case Bond::Hash:
        return getWedgeBondShape();
      case DoubleSymmetric:
        {
          QPointF offset = .5*normalVector;
          result.moveTo(begin + offset + gap * (-bondUnitVector + bondNormalVector));
          result.lineTo(end + offset + gap * (bondUnitVector + bondNormalVector));
          result.lineTo(end - offset + gap * (bondUnitVector - bondNormalVector));
          result.lineTo(begin - offset + gap * (-bondUnitVector - bondNormalVector));
          result.closeSubpath();
          break;
        }
      case DoubleAsymmetric:
        {
          result.moveTo(begin + gap * (-bondUnitVector - bondNormalVector));
          result.lineTo(end + gap * (bondUnitVector - bondNormalVector));
          // now the double part
          QPointF offset = normalVector;
          qreal beginAngle = findIdealAngle(beginAtom(), this, false),
              endAngle = findIdealAngle(endAtom(), this, true),
              limitAngle = atan(2*QLineF(QPointF(0,0),uvb).length()/QLineF(begin, end).length());
          beginAngle = qMax(beginAngle, limitAngle);
          endAngle = qMax(endAngle, limitAngle);
          result.lineTo(end - uvb/tan(endAngle) + offset + gap * (bondUnitVector + bondNormalVector));
          result.lineTo(begin + uvb/tan(beginAngle) + offset + gap * (-bondUnitVector + bondNormalVector));
          result.closeSubpath();
          break;
        }
      case CisOrTrans:
        {
          QPointF offset = .5*normalVector;
          result.moveTo(begin + offset + gap * (-bondUnitVector + bondNormalVector));
          result.lineTo((begin + end)/2 + gap * bondNormalVector);
          result.lineTo(end + offset + gap * (bondUnitVector + bondNormalVector));
          result.lineTo(end - offset + gap * (bondUnitVector - bondNormalVector));
          result.lineTo((begin + end)/2 + gap * -bondNormalVector);
          result.lineTo(begin - offset + gap * (-bondUnitVector - bondNormalVector));
          result.closeSubpath();
          break;
        }
      case Bond::Triple:
        {
          QPointF offset = normalVector;
          result.moveTo(begin + offset + gap * (-bondUnitVector + bondNormalVector));
          result.lineTo(end + offset + gap * (bondUnitVector + bondNormalVector));
          result.lineTo(end - offset + gap * (bondUnitVector - bondNormalVector));
          result.lineTo(begin - offset + gap * (-bondUnitVector - bondNormalVector));
          result.closeSubpath();
          break;
        }
      case Bond::TripleAsymmetric:
        {
          result.moveTo(begin + gap * -bondUnitVector);
          // now the double part
          QPointF offset = normalVector;
          qreal beginAngle = findIdealAngle(beginAtom(), this, false),
              endAngle = findIdealAngle(endAtom(), this, true),
              limitAngle = atan(2*QLineF(QPointF(0,0),uvb).length()/QLineF(begin, end).length());
          beginAngle = qMax(beginAngle, limitAngle);
          endAngle = qMax(endAngle, limitAngle);
          result.lineTo(begin + uvb/tan(beginAngle) + offset + gap * (-bondUnitVector + bondNormalVector));
          result.lineTo(end - uvb/tan(endAngle) + offset + gap * (bondUnitVector + bondNormalVector));
          result.lineTo(end + gap * bondUnitVector);
          beginAngle = findIdealAngle(beginAtom(), this, true);
          endAngle = findIdealAngle(endAtom(), this, false);
          beginAngle = qMax(beginAngle, limitAngle);
          endAngle = qMax(endAngle, limitAngle);
          result.lineTo(end - uvb/tan(endAngle) - offset + gap * (bondUnitVector - bondNormalVector));
          result.lineTo(begin + uvb/tan(beginAngle) - offset + gap * (-bondUnitVector - bondNormalVector));
          result.closeSubpath();
          break;
        }
      default: ;
    }
    return result;
  }
} // namespace
