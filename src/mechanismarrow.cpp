/***************************************************************************
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
#include "mechanismarrow.h"
//#include "reactionarrowdialog.h"

#include "math2d.h"
#include <cmath>

#include <QPainter>
#include <QGraphicsEllipseItem>
#include <QGraphicsScene>
#include <QGraphicsSceneHoverEvent>
#include <QDebug>

namespace Molsketch {

  MechanismArrow::MechanismArrow() : m_p1(QPointF(0.0, 0.0)), m_p2(QPointF(0.0, -50.0)),
      m_p3(QPointF(50.0, -50.0)), m_p4(QPointF(50.0, 0.0)), m_hoverP1(false), 
      m_hoverP2(false), m_hoverP3(false), m_hoverP4(false), m_arrowType(SingleArrow), m_dialog(0)
  {
    setFlags(QGraphicsItem::ItemIsMovable|QGraphicsItem::ItemIsSelectable|QGraphicsItem::ItemIsFocusable);
    setAcceptsHoverEvents(true);
  }
  
  MechanismArrow::~MechanismArrow()
  {
//    if (m_dialog)
  //    m_dialog->deleteLater();
  }
      
  void MechanismArrow::setArrowType(ArrowType t)
  {
    m_arrowType = t;  
  }

  QRectF MechanismArrow::boundingRect() const
  {
    QRectF rect(-200,-200,400,400);

    rect = QRectF(m_p1, m_p3).normalized() | QRectF(m_p2, m_p4).normalized();

    /*
    if (m_end.x() < 0 && m_end.y() < 0)
      rect = QRectF(m_end.x() - 10.0, m_end.y() - 10.0, - m_end.x() + 20.0, - m_end.y() +  20.0);
    else if (m_end.x() < 0)
      rect = QRectF(m_end.x() - 10.0, -10.0, - m_end.x() + 20.0, m_end.y() +  20.0);
    else if (m_end.y() < 0)
      rect = QRectF(-10.0, m_end.y() - 10.0, m_end.x() + 20.0, - m_end.y() +  20.0);
    else
      rect = QRectF(-10.0, -10.0, m_end.x() + 20.0, m_end.y() +  20.0);
    */

    return rect.adjusted(-10,-10,10,10);
  }


  void MechanismArrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
  {
    // draw the bounding rect if the arrow is selected
    if (isSelected() && !m_hoverP1 && !m_hoverP2 && !m_hoverP3 && !m_hoverP4) {
      painter->save();
      painter->setPen(Qt::blue);
      painter->drawRect(boundingRect());
      painter->restore();
    }

    //QPointF end = normalized(m_end);
    //QPointF orthogonal(end.y(), -end.x());

    painter->save();
    QPen pen;
    pen.setWidthF(1.5);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    painter->setPen(pen);
    //painter->setBrush(Qt::black);
        
    QPainterPath path;
    path.moveTo(m_p1);
    path.cubicTo(m_p2, m_p3, m_p4);
    painter->drawPath(path);

    if (isSelected()) {
      painter->setPen(Qt::gray);
      painter->drawLine(m_p1, m_p2);
      painter->drawLine(m_p3, m_p4);
    }


/*
    // draw the arrow
    switch (m_arrowType) {
      default:
      case SingleArrow:
        {
        painter->drawLine(QPointF(0.0, 0.0), m_end);
        QPolygonF polygon;
        polygon << m_end;
        polygon << m_end - 15 * end + 5 * orthogonal;
        polygon << m_end - 12 * end;
        polygon << m_end - 15 * end - 5 * orthogonal;
        painter->drawPolygon(polygon);
        }
        break;
      case DoubleArrow:
        {
        painter->drawLine(QPointF(0.0, 0.0), m_end);
        QPolygonF polygon;
        polygon << m_end;
        polygon << m_end - 15 * end + 5 * orthogonal;
        polygon << m_end - 12 * end;
        polygon << m_end - 15 * end - 5 * orthogonal;
        painter->drawPolygon(polygon);
        polygon.clear();
        polygon << QPointF(0.0, 0.0);
        polygon << 15 * end + 5 * orthogonal;
        polygon << 12 * end;
        polygon << 15 * end - 5 * orthogonal;
        painter->drawPolygon(polygon);
        }
        break;
      case Equilibrium:
        {
        QPointF offset(-orthogonal * 2);
        painter->drawLine(-offset, m_end - offset);
        painter->drawLine(offset, m_end + offset);
        QPolygonF polygon;
        polygon << m_end - offset;
        polygon << m_end - 12 * end - offset;
        polygon << m_end - 15 * end + 7 * orthogonal + offset;
        painter->drawPolygon(polygon);
        polygon.clear();
        polygon << offset;
        polygon << 12 * end + offset;
        polygon << 15 * end - 7 * orthogonal - offset;
        painter->drawPolygon(polygon);
        }
        break;
      case EqRightShifted:
        {
        QPointF offset(-orthogonal * 2);
        QPointF offset2(end * 15);
        painter->drawLine(-offset, m_end - offset);
        painter->drawLine(offset + offset2, m_end + offset - offset2);
        QPolygonF polygon;
        polygon << m_end - offset;
        polygon << m_end - 12 * end - offset;
        polygon << m_end - 15 * end + 7 * orthogonal + offset;
        painter->drawPolygon(polygon); // upper polygon
        polygon.clear();
        polygon << offset + offset2;
        polygon << 12 * end + offset + offset2;
        polygon << 15 * end - 7 * orthogonal - offset + offset2;
        painter->drawPolygon(polygon); // lower polygon
        }
        break;
      case EqLeftShifted:
        {
        QPointF offset(-orthogonal * 2);
        QPointF offset2(end * 15);
        painter->drawLine(-offset + offset2, m_end - offset - offset2);
        painter->drawLine(offset, m_end + offset);
        QPolygonF polygon;
        polygon << m_end - offset - offset2;
        polygon << m_end - 12 * end - offset - offset2;
        polygon << m_end - 15 * end + 7 * orthogonal + offset - offset2;
        painter->drawPolygon(polygon); // upper polygon
        polygon.clear();
        polygon << offset;
        polygon << 12 * end + offset;
        polygon << 15 * end - 7 * orthogonal - offset;
        painter->drawPolygon(polygon); // lower polygon
        }
        break;


    }*/

    // draw red circles when hovering above points
    painter->setPen(Qt::red);
    painter->setBrush(QBrush());
    if (m_hoverP1)
      painter->drawEllipse(m_p1, 5, 5);
    if (m_hoverP2) 
      painter->drawEllipse(m_p2, 5, 5);
    if (m_hoverP3) 
      painter->drawEllipse(m_p3, 5, 5);
    if (m_hoverP4) 
      painter->drawEllipse(m_p4, 5, 5);
    painter->restore();
  }



  void MechanismArrow::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
  {
    qreal distToP1 = length(event->scenePos() - scenePos() - m_p1);
    qreal distToP2 = length(event->scenePos() - scenePos() - m_p2);
    qreal distToP3 = length(event->scenePos() - scenePos() - m_p3);
    qreal distToP4 = length(event->scenePos() - scenePos() - m_p4);

    m_hoverP1 = false;
    m_hoverP2 = false;
    m_hoverP3 = false;
    m_hoverP4 = false;
    if (distToP1 < 5.0)
      m_hoverP1 = true;
    if (distToP2 < 5.0)
      m_hoverP2 = true;
    if (distToP3 < 5.0)
      m_hoverP3 = true;
    if (distToP4 < 5.0)
      m_hoverP4 = true;
  }


  void MechanismArrow::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
  {
    qreal distToP1 = length(event->scenePos() - scenePos() - m_p1);
    qreal distToP2 = length(event->scenePos() - scenePos() - m_p2);
    qreal distToP3 = length(event->scenePos() - scenePos() - m_p3);
    qreal distToP4 = length(event->scenePos() - scenePos() - m_p4);

    m_hoverP1 = false;
    m_hoverP2 = false;
    m_hoverP3 = false;
    m_hoverP4 = false;
    if (distToP1 < 5.0)
      m_hoverP1 = true;
    if (distToP2 < 5.0)
      m_hoverP2 = true;
    if (distToP3 < 5.0)
      m_hoverP3 = true;
    if (distToP4 < 5.0)
      m_hoverP4 = true;

    update();
  }

  void MechanismArrow::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
  {


  }

  void MechanismArrow::mousePressEvent(QGraphicsSceneMouseEvent *event)
  {
    //qDebug() << "mousePressEvent";

    QGraphicsItem::mousePressEvent(event);

  }

  void MechanismArrow::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
  {
    //qDebug() << "mouseMoveEvent";
    if (m_hoverP1) {
      m_p1 = event->pos() ;
      update();
      return;
    } 
    if (m_hoverP2) {
      m_p2 = event->pos() ;
      update();
      return;
    }
    if (m_hoverP3) {
      m_p3 = event->pos() ;
      update();
      return;
    }
    if (m_hoverP4) {
      m_p4 = event->pos() ;
      update();
      return;
    }




    QGraphicsItem::mouseMoveEvent(event);
  }

  void MechanismArrow::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
  {
    QGraphicsItem::mouseReleaseEvent(event);
  }
      
  void MechanismArrow::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
  {
    /*
    if (!m_dialog) {
      m_dialog = new MechanismArrowDialog(this);
    }

    m_dialog->show();
    */
  }

}
