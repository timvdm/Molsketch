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

  MechanismArrow::MechanismArrow() : m_p1(QPointF(0.0, 0.0)), m_p2(QPointF(0.0, -50.0)),
      m_p3(QPointF(50.0, -50.0)), m_p4(QPointF(50.0, 0.0)), m_hoverP1(false), 
      m_hoverP2(false), m_hoverP3(false), m_hoverP4(false), m_arrowType(SingleArrowRight), m_dialog(0)
  {
    setFlags(QGraphicsItem::ItemIsMovable|QGraphicsItem::ItemIsSelectable|QGraphicsItem::ItemIsFocusable);
    setAcceptsHoverEvents(true);
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

  QRectF MechanismArrow::boundingRect() const
  {
    QRectF rect(-200,-200,400,400);
    rect = QRectF(m_p1, m_p3).normalized() | QRectF(m_p2, m_p4).normalized();
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

    painter->save();
    QPen pen;
    pen.setWidthF(1.5);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    painter->setPen(pen);
    //painter->setBrush(Qt::black);
        
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
    painter->setBrush(Qt::black);

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
    if (!m_dialog) {
      m_dialog = new MechanismArrowDialog(this);
    }

    m_dialog->show();
  }

  void MechanismArrow::readXML(QXmlStreamReader &xml)
  {
    QXmlStreamAttributes attr = xml.attributes();
    if (attr.hasAttribute("arrowType"))
      m_arrowType = static_cast<MechanismArrow::ArrowType>(attr.value("arrowType").toString().toInt());
    if (attr.hasAttribute("posx") && attr.hasAttribute("posy"))
      setPos(QPointF(attr.value("posx").toString().toFloat(),
                          attr.value("posy").toString().toFloat()));
    if (attr.hasAttribute("p1x") && attr.hasAttribute("p1y"))
      m_p1 = QPointF(attr.value("p1x").toString().toFloat(),
                     attr.value("p1y").toString().toFloat());
    if (attr.hasAttribute("p2x") && attr.hasAttribute("p2y"))
      m_p2 = QPointF(attr.value("p2x").toString().toFloat(),
                     attr.value("p2y").toString().toFloat());
    if (attr.hasAttribute("p3x") && attr.hasAttribute("p3y"))
      m_p3 = QPointF(attr.value("p3x").toString().toFloat(),
                     attr.value("p3y").toString().toFloat());
    if (attr.hasAttribute("p4x") && attr.hasAttribute("p4y"))
      m_p4 = QPointF(attr.value("p4x").toString().toFloat(),
                     attr.value("p4y").toString().toFloat());
  }

  void MechanismArrow::writeXML(QXmlStreamWriter &xml)
  {
    xml.writeStartElement("object");
    xml.writeAttribute("type", "MechanismArrow");
    xml.writeAttribute("arrowType", QString::number(m_arrowType));
    xml.writeAttribute("posx", QString::number(scenePos().x()));
    xml.writeAttribute("posy", QString::number(scenePos().y()));
    xml.writeAttribute("p1x", QString::number(m_p1.x()));
    xml.writeAttribute("p1y", QString::number(m_p1.y()));
    xml.writeAttribute("p2x", QString::number(m_p2.x()));
    xml.writeAttribute("p2y", QString::number(m_p2.y()));
    xml.writeAttribute("p3x", QString::number(m_p3.x()));
    xml.writeAttribute("p3y", QString::number(m_p3.y()));
    xml.writeAttribute("p4x", QString::number(m_p4.x()));
    xml.writeAttribute("p4y", QString::number(m_p4.y()));
    xml.writeEndElement();
  }
}
