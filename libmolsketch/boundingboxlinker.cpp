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
#include "boundingboxlinker.h"

namespace Molsketch {

  struct BoundingBoxLinkerPrivate {
    Anchor origin;
    Anchor target;
  };

  BoundingBoxLinker::BoundingBoxLinker(Anchor originAnchor, Anchor ownAnchor, const QPointF &offset)
    : d_ptr(new BoundingBoxLinkerPrivate) {
    Q_D(BoundingBoxLinker);
    d->origin = originAnchor;
    d->target = ownAnchor;
  }

  BoundingBoxLinker::~BoundingBoxLinker() {}

  qreal getXMultiplier(const Anchor& anchor) {
    return (anchor & (1|2)) * 0.5;
  }

  qreal getYMultiplier(const Anchor& anchor) {
    return (anchor & (4|8)) * 0.125;
  }

  QPointF BoundingBoxLinker::getShift(const QRectF &reference, const QRectF &target) const {
    Q_D(const BoundingBoxLinker);
    QPointF topLeftShift(reference.topLeft() - target.topLeft());
    qreal xShift = getXMultiplier(d->origin) * reference.width() - getXMultiplier(d->target) * target.width();
    qreal yShift = getYMultiplier(d->origin) * reference.height() - getYMultiplier(d->target) * target.height();
    return topLeftShift + QPointF(xShift, yShift);
  }

} // namespace Molsketch
