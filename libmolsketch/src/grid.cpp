#include "grid.h"
#include <QGraphicsScene>
#include <QPainter>
#include <cmath>

namespace Molsketch {

  struct grid::privateData
  {
    qreal vertical, horizontal;
  };

  grid::grid()
    : d(new privateData)
  {
    setZValue(-INFINITY);
    setFlags(0);
    d->vertical = 10;
    d->horizontal = 10;
  }

  grid::~grid()
  {
    delete d;
  }

  QPointF grid::alignPoint(const QPointF &point)
  {
    return QPointF(qRound(point.x()/d->horizontal)*d->horizontal,
                   qRound(point.y()/d->vertical)*d->vertical);
  }

  void grid::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
  {
    // TODO colors, size customizable
    Q_UNUSED(option)
    Q_UNUSED(widget)
    painter->save();
    QPen pen(Qt::gray);
    pen.setWidth(0);
    painter->setPen(pen);
    painter->setBrush(Qt::gray);
    QRectF rect(boundingRect());
    for (qreal x = qBound(rect.left(),qRound(rect.left()/d->horizontal)*d->horizontal,rect.right()) ; x < rect.right() ; x += d->horizontal)
      painter->drawLine(x, rect.top(), x, rect.bottom());
    for (qreal y = qBound(rect.top(),qRound(rect.top()/d->vertical)*d->vertical,rect.bottom()) ; y < rect.bottom() ; y += d->vertical)
        painter->drawLine(rect.left(), y, rect.right(), y);
    painter->restore();
  }

  QRectF grid::boundingRect() const
  { // TODO use sceneRectChanged signal to update
    if (!scene()) return QRectF();
    return scene()->sceneRect();
  }

  void grid::setHorizontalInterval(qreal h)
  {
    d->horizontal = h;
    update();
  }

  void grid::setVerticalInterval(qreal v)
  {
    d->vertical = v;
    update();
  }

  qreal grid::horizontalInterval() const
  {
    return d->horizontal;
  }

  qreal grid::verticalInterval() const
  {
    return d->vertical;
  }

} // namespace Molsketch

