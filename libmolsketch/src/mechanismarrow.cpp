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
#include "mechanismarrowdialog.h"

#include "math2d.h"
#include <cmath>

#include <QPainter>
#include <QGraphicsEllipseItem>
#include <QGraphicsScene>
#include <QGraphicsSceneHoverEvent>
#include <QXmlStreamWriter>
#include <QDebug>

namespace Molsketch {

  MechanismArrow::MechanismArrow()
    : arrowGraphicsItem(),
      m_arrowType(SingleArrowRight),
      m_p1(QPointF(0.0, 0.0)),
      m_p2(QPointF(0.0, -50.0)),
      m_p3(QPointF(50.0, -50.0)),
      m_p4(QPointF(50.0, 0.0)),
      m_hoverP1(false),
      m_hoverP2(false),
      m_hoverP3(false),
      m_hoverP4(false),
      m_dialog(0)
  {
    setFlags(QGraphicsItem::ItemIsMovable|QGraphicsItem::ItemIsSelectable|QGraphicsItem::ItemIsFocusable);
#if QT_VERSION < 0x050000
    setAcceptsHoverEvents(true);
#else
    setAcceptHoverEvents(true) ;
#endif
    setZValue(1.0);
  }
  
  MechanismArrow::MechanismArrow(QPointF c1, QPointF c2, QPointF endPoint)
    : arrowGraphicsItem(),
      m_arrowType(SingleArrowRight),
      m_p1(QPointF(0.0, 0.0)),
      m_p2(c1),
      m_p3(c2),
      m_p4(endPoint),
      m_hoverP1(false),
      m_hoverP2(false),
      m_hoverP3(false),
      m_hoverP4(false),
      m_dialog(0)
  {
    setFlags(QGraphicsItem::ItemIsMovable|QGraphicsItem::ItemIsSelectable|QGraphicsItem::ItemIsFocusable);
#if QT_VERSION < 0x050000
    setAcceptsHoverEvents(true);
#else
    setAcceptHoverEvents(true) ;
#endif
    setZValue(1.0);
  }


  MechanismArrow::~MechanismArrow()
  {
    if (m_dialog)
      m_dialog->deleteLater();
  }
      
  void MechanismArrow::setArrowType(ArrowType t)
  {
    m_arrowType = t;
  }

  void MechanismArrow::setCoordinates(const QVector<QPointF> &c)
  {
    if (c.size() != 4) return ;
    m_p1 = c[0] ;
    m_p2 = c[1] ;
    m_p3 = c[2] ;
    m_p4 = c[3] ;
  }

  QVector<QPointF> MechanismArrow::coordinates() const
  {
    return QVector<QPointF>() << m_p1 << m_p2 << m_p3 << m_p4 ;
  }

  QRectF MechanismArrow::boundingRect() const
  {
    QRectF rect(-200,-200,400,400);
    rect = QRectF(m_p1, m_p3).normalized() | QRectF(m_p2, m_p4).normalized();
    return rect.adjusted(-10,-10,10,10);


  }

  QPainterPath MechanismArrow::shape() const
  {
    QPainterPath path;
    path.addEllipse(m_p1, 5, 5);
    path.addEllipse(m_p2, 5, 5);
    path.addEllipse(m_p3, 5, 5);
    path.addEllipse(m_p4, 5, 5);
    return path;
  }


  void MechanismArrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
  {
    Q_UNUSED(option);
    Q_UNUSED(widget);

    // draw the bounding rect if the arrow is selected
    if (isSelected()/* && !m_hoverP1 && !m_hoverP2 && !m_hoverP3 && !m_hoverP4*/) {
      painter->save();
      painter->setPen(Qt::blue);
      painter->drawRect(boundingRect().adjusted(1,1,-1,-1));
      painter->restore();
    }

    painter->save();
    QPen pen;
    pen.setWidthF(lineWidth());
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    pen.setColor(getColor());
    painter->setPen(pen);
//    painter->setBrush(pen.color());
        
    // draw the bezier curve
    QPainterPath path;
    path.moveTo(m_p1);
    path.cubicTo(m_p2, m_p3, m_p4);
    painter->drawPath(path);

    // if selected, draw the curve hint lines
    if (isSelected()) {
      painter->setPen(Qt::gray);
      painter->drawLine(m_p1, m_p2);
      painter->drawLine(m_p3, m_p4);
    }

    QPointF line1 = normalized(m_p1 - m_p2);
    QPointF line2 = normalized(m_p4 - m_p3);
    QPointF orthogonal1(line1.y(), -line1.x());
    QPointF orthogonal2(line2.y(), -line2.x());

    painter->setPen(pen);
    painter->setBrush(pen.color());

    // draw the arrow
    switch (m_arrowType) {
      default:
      case SingleArrowRight:
        {
        QPolygonF polygon;
        polygon << m_p4;
        polygon << m_p4 - 15 * line2 + 5 * orthogonal2;
        polygon << m_p4 - 12 * line2;
        polygon << m_p4 - 15 * line2 - 5 * orthogonal2;
        painter->drawPolygon(polygon);
        }
        break;
      case SingleArrowLeft:
        {
        QPolygonF polygon;
        polygon << m_p1;
        polygon << m_p1 - 15 * line1 + 5 * orthogonal1;
        polygon << m_p1 - 12 * line1;
        polygon << m_p1 - 15 * line1 - 5 * orthogonal1;
        painter->drawPolygon(polygon);
        }
        break;
      case DoubleArrow:
        {
        QPolygonF polygon;
        polygon << m_p4;
        polygon << m_p4 - 15 * line2 + 5 * orthogonal2;
        polygon << m_p4 - 12 * line2;
        polygon << m_p4 - 15 * line2 - 5 * orthogonal2;
        painter->drawPolygon(polygon);
        polygon.clear();
        polygon << m_p1;
        polygon << m_p1 - 15 * line1 + 5 * orthogonal1;
        polygon << m_p1 - 12 * line1;
        polygon << m_p1 - 15 * line1 - 5 * orthogonal1;
        painter->drawPolygon(polygon);
        }
        break;
      case SingleHookRight:
        {
        QPolygonF polygon;
        polygon << m_p4;
        polygon << m_p4 - 15 * line2 + 5 * orthogonal2;
        polygon << m_p4 - 12 * line2;
        //polygon << m_p4 - 15 * line2 - 5 * orthogonal2;
        painter->drawPolygon(polygon);
        }
        break;
      case SingleHookLeft:
        {
        QPolygonF polygon;
        polygon << m_p1;
        //polygon << m_p1 - 15 * line1 + 5 * orthogonal1;
        polygon << m_p1 - 12 * line1;
        polygon << m_p1 - 15 * line1 - 5 * orthogonal1;
        painter->drawPolygon(polygon);
        }
        break;
      case DoubleHook:
        {
        QPolygonF polygon;
        polygon << m_p4;
        polygon << m_p4 - 15 * line2 + 5 * orthogonal2;
        polygon << m_p4 - 12 * line2;
        //polygon << m_p4 - 15 * line2 - 5 * orthogonal2;
        painter->drawPolygon(polygon);
        polygon.clear();
        polygon << m_p1;
        //polygon << m_p1 - 15 * line1 + 5 * orthogonal1;
        polygon << m_p1 - 12 * line1;
        polygon << m_p1 - 15 * line1 - 5 * orthogonal1;
        painter->drawPolygon(polygon);
        }
        break;
 
    }

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
    Q_UNUSED(event);
    m_hoverP1 = false;
    m_hoverP2 = false;
    m_hoverP3 = false;
    m_hoverP4 = false;
    update();
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
    Q_UNUSED(event)
    if (!m_dialog) {
      m_dialog = new MechanismArrowDialog(this);
    }

    m_dialog->show();
  }

  void MechanismArrow::readGraphicAttributes(const QXmlStreamAttributes &attributes)
  {
    m_arrowType = (MechanismArrow::ArrowType) (attributes.value("arrowType").toString().toInt()) ;
    setPos(attributes.value("posx").toString().toInt(),
           attributes.value("posy").toString().toInt()) ;
    QVector<QPointF*> points ;
    points << &m_p1 << &m_p2 << &m_p3 << &m_p4 ;
    for (int i = 0 ; i < points.size() ; ++i)
    {
      points[i]->setX(attributes.value("p" + QString::number(i+1) + "x").toString().toFloat());
      points[i]->setY(attributes.value("p" + QString::number(i+1) + "y").toString().toFloat());
    }
  }

  QXmlStreamAttributes MechanismArrow::graphicAttributes() const
  {
    QXmlStreamAttributes attributes ;
    attributes.append("type", "MechanismArrow");
    attributes.append("arrowType", QString::number(m_arrowType));
    attributes.append("posx", QString::number(scenePos().x()));
    attributes.append("posy", QString::number(scenePos().y()));
    QVector<const QPointF*> points ;
    points << &m_p1 << &m_p2 << &m_p3 << &m_p4 ;
    for (int i = 0 ; i < points.size() ; ++i)
    {
      attributes.append("p" + QString::number(i+1) + "x", QString::number(points[i]->x())) ;
      attributes.append("p" + QString::number(i+1) + "y", QString::number(points[i]->y())) ;
    }
    return attributes ;
  }
}
