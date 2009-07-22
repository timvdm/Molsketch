/***************************************************************************
 *   Copyright (C) 2007 by Harm van Eersel                                 *
 *   devsciurus@xs4all.nl                                                  *
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

  Bond::Bond(Atom* atomA, Atom* atomB, int order, int type, QGraphicsItem* parent,QGraphicsScene* scene) : QGraphicsItem(parent,scene)
  {
    Q_CHECK_PTR(atomA);
    Q_CHECK_PTR(atomB);

    m_bondType = type;
    m_bondOrder = order;
    m_beginAtom = atomA;
    m_endAtom = atomB;
    m_ring = 0;

    atomA->addNeighbor(atomB);
    atomB->addNeighbor(atomA);
  
    setPos(m_beginAtom->scenePos());

  //   setFlag(QGraphicsItem::ItemIsSelectable);
  //   setAcceptedMouseButtons(Qt::LeftButton);
  }
  
  Bond::~Bond()
  {
    //m_beginAtom->removeNeighbor(m_endAtom);
    //m_endAtom->removeNeighbor(m_beginAtom);
  }

/*
void Bond::redoValency()
{
  // Check if the atoms still exist
  if (!m_beginAtom || !m_endAtom) return;
  Q_CHECK_PTR(m_beginAtom);
  Q_CHECK_PTR(m_endAtom);

  // Registering the atoms as connected
  m_beginAtom->addNeighbor(m_endAtom);
  m_endAtom->addNeighbor(m_beginAtom);

  // Set the new number of bonds of the atoms
  m_beginAtom->setNumberOfBonds(m_beginAtom->numBonds() + bondOrder());
  m_endAtom->setNumberOfBonds(m_endAtom->numBonds() + bondOrder());

  // Setting the new valency of the atoms
  int e1 = molsKetch::valencyOfElement(molsKetch::symbol2number(m_beginAtom->element()));
  int e2 = molsKetch::valencyOfElement(molsKetch::symbol2number(m_endAtom->element()));
  if (e1 < 0 && e2 < 0 )
    {
      m_beginAtom->setValency(m_beginAtom->valency() - m_bondOrder);
      m_endAtom->setValency(m_endAtom->valency() - m_bondOrder);
      return;
    }

  if (e1 > 0 && e2 > 0 )
    {
      m_beginAtom->setValency(m_beginAtom->valency() + m_bondOrder);
      m_endAtom->setValency(m_endAtom->valency() + m_bondOrder);
      return;
    }

  if ( e1 > e2 )
    {
      m_beginAtom->setValency(m_beginAtom->valency() + m_bondOrder);
      m_endAtom->setValency(m_endAtom->valency() - m_bondOrder);
      return;
    }
  if ( e1 < e2 )
    {
      m_beginAtom->setValency(m_beginAtom->valency() - m_bondOrder);
      m_endAtom->setValency(m_endAtom->valency() + m_bondOrder);
      return;
    }

  m_beginAtom->setValency(m_beginAtom->valency() - m_bondOrder);
  m_endAtom->setValency(m_endAtom->valency() - m_bondOrder);*/
/*
}

void Bond::undoValency()
{
  // Check if the atoms still exist
  if (!m_beginAtom || !m_endAtom) return;
  Q_CHECK_PTR(m_beginAtom);
  Q_CHECK_PTR(m_endAtom);

  // Registering the atoms as disconnected
  m_beginAtom->removeNeighbor(m_endAtom);
  m_endAtom->removeNeighbor(m_beginAtom);

  // Set the new number of bonds of the atoms
  m_beginAtom->setNumberOfBonds(m_beginAtom->numBonds() - bondOrder());
  m_endAtom->setNumberOfBonds(m_endAtom->numBonds() - bondOrder());
*/

/*  // Setting the new valency of the atoms
  int e1 = -molsKetch::valencyOfElement(molsKetch::symbol2number(m_beginAtom->element()));
  int e2 = -molsKetch::valencyOfElement(molsKetch::symbol2number(m_endAtom->element()));
  if (e1 < 0 && e2 < 0 )
    {
      m_beginAtom->setValency(m_beginAtom->valency() - m_bondOrder);
      m_endAtom->setValency(m_endAtom->valency() - m_bondOrder);
      return;
    }

  if (e1 > 0 && e2 > 0 )
    {
      m_beginAtom->setValency(m_beginAtom->valency() + m_bondOrder);
      m_endAtom->setValency(m_endAtom->valency() + m_bondOrder);
      return;
    }

  if ( e1 > e2 )
    {
      m_beginAtom->setValency(m_beginAtom->valency() + m_bondOrder);
      m_endAtom->setValency(m_endAtom->valency() - m_bondOrder);
      return;
    }
  if ( e1 < e2 )
    {
      m_beginAtom->setValency(m_beginAtom->valency() - m_bondOrder);
      m_endAtom->setValency(m_endAtom->valency() + m_bondOrder);
      return;
    }

  m_beginAtom->setValency(m_beginAtom->valency() - m_bondOrder);
  m_endAtom->setValency(m_endAtom->valency() - m_bondOrder);*/
//}
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

  void Bond::drawSimpleBond(QPainter *painter)
  {
    if (m_ring && (m_bondOrder == 2)) {
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
          painter->drawLine(QLineF(begin + offset, end + offset));
          painter->drawLine(QLineF(begin - offset, end - offset));
          }
          break;
      case 3:
          {
          QPointF orthogonal(uvb.y(), -uvb.x());
          QPointF offset = orthogonal * m_bondSpacing;
          painter->drawLine(QLineF(begin, end));
          painter->drawLine(QLineF(begin + offset, end + offset));
          painter->drawLine(QLineF(begin - offset, end - offset));
          }
          break;
    }

  
  }

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

    /*
    if (m_beginAtom->hasLabel())
      begin += 0.20 * vb;
    if (m_endAtom->hasLabel())
      end -= 0.20 * vb;
    */

    QPointF uvb = vb / sqrt(vb.x()*vb.x() + vb.y()*vb.y());
    QPointF orthogonal(uvb.y(), -uvb.x());
    QPointF spacing = orthogonal * m_bondSpacing;
    QPointF offset = uvb * m_bondSpacing;
    if (length(begin + spacing - center) > length(begin - spacing - center))
      spacing *= -1.0;
    
    painter->drawLine(QLineF(begin, end));
    painter->drawLine(QLineF(begin + spacing + offset, end + spacing - offset));
  }

void Bond::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
  Q_UNUSED(option);
  Q_UNUSED(widget);

  // Check the scene
  MolScene* molScene = dynamic_cast<MolScene*>(scene());
  Q_CHECK_PTR(molScene);

  // 	painter->drawRect(boundingRect());
  QPointF points[4] =
    {
      mapFromParent(m_beginAtom->pos()),
      shiftVector(QLineF(mapFromParent(m_beginAtom->pos()),mapFromParent(m_endAtom->pos())),3).p2(),
      shiftVector(QLineF(mapFromParent(m_beginAtom->pos()),mapFromParent(m_endAtom->pos())),-3).p2(),
      mapFromParent(m_beginAtom->pos())
    };
  QPointF points2[4] =
    {
      mapFromParent(m_endAtom->pos()),
      shiftVector(QLineF(mapFromParent(m_beginAtom->pos()),mapFromParent(m_endAtom->pos())),3).p1(),
      shiftVector(QLineF(mapFromParent(m_beginAtom->pos()),mapFromParent(m_endAtom->pos())),-3).p1(),
      mapFromParent(m_endAtom->pos())
    };

  // Set painter defaults
  painter->save();
  QPen pen;
  pen.setWidthF(molScene->bondWidth());
  painter->setPen(pen);

  // Create dash pattern for dot
  QVector<qreal> dash;
  dash << 2 << 5;

  // Create a gradient for down
  QRadialGradient radialGrad(mapFromScene(m_beginAtom->scenePos()), 5);
  radialGrad.setColorAt(0, Qt::white);
  radialGrad.setColorAt(0.3,Qt::white);
  radialGrad.setColorAt(0.5,Qt::black);
  radialGrad.setColorAt(0.7, Qt::white);
  radialGrad.setColorAt(1, Qt::white);
  radialGrad.setSpread(QGradient::RepeatSpread);



  switch ( m_bondType )
    {
    case Bond::Hash:
      painter->setPen( Qt::NoPen );
      painter->setBrush( radialGrad );
      painter->drawConvexPolygon( points2, 4);
      break;
    case Bond::InvertedHash:
      painter->setPen( Qt::NoPen );
      painter->setBrush( radialGrad );
      painter->drawConvexPolygon( points, 4);
      break;
    case Bond::WedgeOrHash:
      pen.setDashPattern(dash);
      painter->setPen(pen);
      painter->drawLine(QLineF(mapFromParent(m_beginAtom->pos()),mapFromParent(m_endAtom->pos())));
      break;
    case Bond::Wedge:
      painter->setBrush( QBrush(Qt::black) );
      painter->drawConvexPolygon( points, 4);
      break;
    case Bond::InvertedWedge:
      painter->setBrush( QBrush(Qt::black) );
      painter->drawConvexPolygon( points2, 4);
      break;

    default:
      drawSimpleBond(painter);
    }

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
//     path.quadTo(QPointF(),m_endAtom->pos());
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
  //undoValency();
  m_bondOrder = order;
  //redoValency();
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

  void Bond::setType(int t)
  {
    //pre: 0 <= t < 6
    //post: bondType = t
    Q_ASSERT(0 <= t && t < 6);

    m_bondType = t;
    update();
  }

void Bond::incType()
{
  //pre: true
  //post: bondType = bondType % 6 + 1
  setType((m_bondType + 1) % 6);
}

void Bond::decType()
{
  //pre: true
  //post: bondType = (bondType + 5) % 6
  setType( (m_bondType + 5) % 6);
}


  // Query methods

  int Bond::bondOrder() const
  {
    return m_bondOrder;
  }

  int Bond::bondType() const
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
