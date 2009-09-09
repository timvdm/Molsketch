#include "reactionarrow.h"
#include "math2d.h"
#include <cmath>

#include <QPainter>
#include <QGraphicsEllipseItem>
#include <QGraphicsScene>
#include <QGraphicsSceneHoverEvent>
#include <QDebug>

namespace Molsketch {

  ReactionArrow::ReactionArrow() : m_end(QPointF(50.0, 0.0)), m_hoverBegin(false), m_hoverEnd(false),
      m_arrowType(FilledArrow)
  {
    setFlags(QGraphicsItem::ItemIsMovable|QGraphicsItem::ItemIsSelectable|QGraphicsItem::ItemIsFocusable);
    setAcceptsHoverEvents(true);
  }

  QRectF ReactionArrow::boundingRect() const
  {
    if (m_end.x() < 0 && m_end.y() < 0)
      return QRectF(m_end.x() - 10.0, m_end.y() - 10.0, - m_end.x() + 20.0, - m_end.y() +  20.0);
    else if (m_end.x() < 0)
      return QRectF(m_end.x() - 10.0, -10.0, - m_end.x() + 20.0, m_end.y() +  20.0);
    else if (m_end.y() < 0)
      return QRectF(-10.0, m_end.y() - 10.0, m_end.x() + 20.0, - m_end.y() +  20.0);

    //qDebug() << "m_end =" << m_end;
    return QRectF(-10.0, -10.0, m_end.x() + 20.0, m_end.y() +  20.0);
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
      case FilledArrow:
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

}
