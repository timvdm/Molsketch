/***************************************************************************
 *   Copyright (C) 2015 Hendrik Vennekate                                  *
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
#include "grid.h"
#include <QGraphicsScene>
#include <QPainter>
#if QT_VERSION >= 0x050000
#include <QtMath>
#else
#include <QtCore/qmath.h>
#endif

namespace Molsketch {

  struct grid::privateData
  {
    qreal vertical, horizontal, linewidth;
    QColor color;
  };

  grid::grid()
    : d(new privateData)
  {
    setZValue(-INFINITY);
    setFlags(0);
    d->vertical = 10;
    d->horizontal = 10;
    d->linewidth = 0;
    d->color = Qt::gray;
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
    Q_UNUSED(option)
    Q_UNUSED(widget)
    painter->save();
    QPen pen(d->color);
    pen.setWidth(d->linewidth);
    painter->setPen(pen);
    QRectF rect(boundingRect());
    for (qreal x = qBound(rect.left(),qRound(rect.left()/d->horizontal)*d->horizontal,rect.right()) ; x < rect.right() ; x += d->horizontal)
      painter->drawLine(x, rect.top(), x, rect.bottom());
    for (qreal y = qBound(rect.top(),qRound(rect.top()/d->vertical)*d->vertical,rect.bottom()) ; y < rect.bottom() ; y += d->vertical)
        painter->drawLine(rect.left(), y, rect.right(), y);
    painter->restore();
  }

  QRectF grid::boundingRect() const
  {
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

  void grid::setColor(const QColor &color)
  {
    d->color = color;
    update();
  }

  void grid::setLinewidth(const qreal &linewidth)
  {
    d->linewidth = linewidth;
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

  QColor grid::color() const
  {
    return d->color;
  }

  qreal grid::linewidth() const
  {
    return d->linewidth;
  }

} // namespace Molsketch

