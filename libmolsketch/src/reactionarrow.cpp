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
#include "reactionarrow.h"
#include "reactionarrowdialog.h"

#include "math2d.h"
#include <cmath>

#include <QPainter>
#include <QGraphicsEllipseItem>
#include <QGraphicsScene>
#include <QGraphicsSceneHoverEvent>
#include <QXmlStreamWriter>
#include <QDebug>

namespace Molsketch {

  ReactionArrow::ReactionArrow()
    : arrowGraphicsItem(),
      m_arrowType(SingleArrow),
      m_end(QPointF(50.0, 0.0)),
      m_hoverBegin(false),
      m_hoverEnd(false),
      m_dialog(0)
  {
  }
  
  ReactionArrow::~ReactionArrow()
  {
    if (m_dialog)
      m_dialog->deleteLater();
  }
      
  void ReactionArrow::setArrowType(ArrowType t)
  {
    m_arrowType = t;
  }

  void ReactionArrow::setCoordinates(const QVector<QPointF> &c)
  {
    if (c.size() != 2) return ;
    setPos(c.first()) ;
    m_end = c.last() ;
  }

  QPolygonF ReactionArrow::coordinates() const
  {
    return QVector<QPointF>() << pos() << m_end ;
  }

  QRectF ReactionArrow::ownBoundingRect() const // TODO adapt
  {
    QRectF rect;
    if (m_end.x() < 0 && m_end.y() < 0)
      rect = QRectF(m_end.x() - 10.0, m_end.y() - 10.0, - m_end.x() + 20.0, - m_end.y() +  20.0);
    else if (m_end.x() < 0)
      rect = QRectF(m_end.x() - 10.0, -10.0, - m_end.x() + 20.0, m_end.y() +  20.0);
    else if (m_end.y() < 0)
      rect = QRectF(-10.0, m_end.y() - 10.0, m_end.x() + 20.0, - m_end.y() +  20.0);
    else
      rect = QRectF(-10.0, -10.0, m_end.x() + 20.0, m_end.y() +  20.0);

    return rect.adjusted(-2, -2, 2, 2);
  }


  void ReactionArrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
  {
    Q_UNUSED(option);
    Q_UNUSED(widget);

    // draw the bounding rect if the arrow is selected
    if (isSelected() && !m_hoverBegin && !m_hoverEnd) {
      painter->save();
      painter->setPen(Qt::blue);
      painter->drawRect(boundingRect());
      painter->restore();
    }

    QPointF end = normalized(m_end);
    QPointF orthogonal(end.y(), -end.x());

    painter->save();
    QPen pen;
    pen.setWidthF(lineWidth());
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    pen.setColor(getColor());
    painter->setPen(pen);
    painter->setBrush(pen.color());

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


    }

    // draw red circles when hovering above begin/end
    painter->setPen(Qt::red);
    painter->setBrush(QBrush());
    if (m_hoverBegin) {
      painter->drawEllipse(QPointF(0.0, 0.0), 5, 5);
    }
    if (m_hoverEnd) {
      painter->drawEllipse(m_end, 5, 5);
    }
    painter->restore();
  }



  void ReactionArrow::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
  {
    qreal distToBegin = length(event->scenePos() - scenePos());
    qreal distToEnd = length(event->scenePos() - scenePos() - m_end);

    m_hoverBegin = false;
    m_hoverEnd = false;
    if (distToBegin < 5.0)
      m_hoverBegin = true;
    if (distToEnd < 5.0)
      m_hoverEnd = true;
  }


  void ReactionArrow::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
  {
    qreal distToBegin = length(event->scenePos() - scenePos());
    qreal distToEnd = length(event->scenePos() - scenePos() - m_end);

    m_hoverBegin = false;
    m_hoverEnd = false;
    if (distToBegin < 5.0)
      m_hoverBegin = true;
    if (distToEnd < 5.0)
      m_hoverEnd = true;

    update();
  }

  void ReactionArrow::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
  {
    Q_UNUSED(event);
  }

  void ReactionArrow::mousePressEvent(QGraphicsSceneMouseEvent *event)
  {
    //qDebug() << "mousePressEvent";
    QGraphicsItem::mousePressEvent(event);
  }

  void ReactionArrow::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
  {
    //qDebug() << "mouseMoveEvent";
    if (m_hoverBegin) {
      m_end = m_end + scenePos() - event->scenePos() ;
      setPos(event->scenePos());
      update();
      return;
    } 
    if (m_hoverEnd) {
      m_end = - scenePos() + event->scenePos();
      update();
      return;
    }

    QGraphicsItem::mouseMoveEvent(event);
  }

  void ReactionArrow::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
  {
    QGraphicsItem::mouseReleaseEvent(event);
  }
      
  void ReactionArrow::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
  {
    Q_UNUSED(event);
    if (!m_dialog) {
      m_dialog = new ReactionArrowDialog(this);
    }

    m_dialog->show();      
  }

  void ReactionArrow::readGraphicAttributes(const QXmlStreamAttributes &attributes)
  {
    m_arrowType = (ReactionArrow::ArrowType) (attributes.value("arrowType").toString().toInt()) ;
    setPos(attributes.value("posx").toString().toFloat(),
           attributes.value("posy").toString().toFloat());
    m_end = QPointF(attributes.value("endx").toString().toFloat(),
                    attributes.value("endy").toString().toFloat()) ;
  }

  QXmlStreamAttributes ReactionArrow::graphicAttributes() const
  {
    QXmlStreamAttributes attributes ;
    attributes.append("type", "ReactionArrow") ;
    attributes.append("arrowType", QString::number(m_arrowType)) ;
    attributes.append("posx", QString::number(scenePos().x())) ;
    attributes.append("posy", QString::number(scenePos().y())) ;
    attributes.append("endx", QString::number(m_end.x())) ;
    attributes.append("endy", QString::number(m_end.y())) ;
    return attributes ;
  }

}
