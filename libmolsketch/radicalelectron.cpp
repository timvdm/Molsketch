/***************************************************************************
 *   Copyright (C) 2017 by Hendrik Vennekate                               *
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
#include "radicalelectron.h"
#include <QColor>
#include <QPainter>
namespace Molsketch {

struct RadicalElectronPrivate {
  qreal diameter;
  QColor color;
  BoundingBoxLinker linker;
};


RadicalElectron::RadicalElectron(qreal diameter, BoundingBoxLinker linker, const QColor &color)
  : d_ptr(new RadicalElectronPrivate)
{
  Q_D(RadicalElectron);
  d->diameter = diameter;
  d->color = color;
  d->linker = linker;
}

RadicalElectron::~RadicalElectron() {}

QRectF RadicalElectron::boundingRect() const {
  if (!parentItem()) return QRectF();
  Q_D(const RadicalElectron);
  QRectF bounds(0,0,d->diameter,d->diameter);
  bounds.translate(d->linker.getShift(parentItem()->boundingRect(), bounds));
  return bounds;
}

void RadicalElectron::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
  Q_UNUSED(option)
  Q_UNUSED(widget)
  Q_D(const RadicalElectron);
  if (!parentItem()) return;
  painter->drawEllipse(boundingRect());
}

} // namespace Molsketch
