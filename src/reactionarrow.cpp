#include "reactionarrow.h"
#include "reactionarrowdialog.h"

#include "math2d.h"
#include <cmath>

#include <QPainter>
#include <QGraphicsEllipseItem>
#include <QGraphicsScene>
#include <QGraphicsSceneHoverEvent>
#include <QDebug>

namespace Molsketch {

  ReactionArrow::ReactionArrow() : m_end(QPointF(50.0, 0.0)), m_hoverBegin(false), m_hoverEnd(false),
      m_arrowType(SingleArrow), m_dialog(0)
  {
    setFlags(QGraphicsItem::ItemIsMovable|QGraphicsItem::ItemIsSelectable|QGraphicsItem::ItemIsFocusable);
    setAcceptsHoverEvents(true);
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

  QRectF ReactionArrow::boundingRect() const
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
    // draw the bounding rect if the arrow is selected
    if (isSelected()) {
      painter->save();
      painter->setPen(Qt::blue);
      painter->drawRect(boundingRect());
      painter->restore();
    }

    QPointF end = normalized(m_end);
    QPointF orthogonal(end.y(), -end.x());

    painter->save();
    QPen pen;
    pen.setWidthF(1.5);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    painter->setPen(pen);
    painter->setBrush(Qt::black);

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


  }

  void ReactionArrow::mousePressEvent(QGraphicsSceneMouseEvent *event)
  {
    //qDebug() << "mousePressEvent";

  }

  void ReactionArrow::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
  {
    //qDebug() << "mouseMoveEvent";
    if (m_hoverBegin)
      setPos(event->scenePos());
    if (m_hoverEnd)
      m_end = - scenePos() + event->scenePos();

    update();
  }

  void ReactionArrow::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
  {
  }
      
  void ReactionArrow::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
  {
    if (!m_dialog) {
      m_dialog = new ReactionArrowDialog(this);
    }

    m_dialog->show();      
  }

}
