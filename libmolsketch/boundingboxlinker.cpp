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

#include <QDebug>
#include <QRegularExpression>
#include <QString>

namespace Molsketch {

  struct BoundingBoxLinkerPrivate {
    Anchor origin;
    Anchor target;
    QPointF offset;
    bool operator ==(const BoundingBoxLinkerPrivate& other) const {
      return origin == other.origin && target == other.target && offset == other.offset;
    }
  };

  BoundingBoxLinker::BoundingBoxLinker(Anchor originAnchor, Anchor ownAnchor, const QPointF &offset)
    : d_ptr(new BoundingBoxLinkerPrivate) {
    Q_D(BoundingBoxLinker);
    d->origin = originAnchor;
    d->target = ownAnchor;
    d->offset = offset;
  }

  BoundingBoxLinker::BoundingBoxLinker(const BoundingBoxLinker &other)
    :d_ptr(new BoundingBoxLinkerPrivate(*(other.d_ptr)))
  {}

  BoundingBoxLinker::~BoundingBoxLinker() {}

  qreal getXMultiplier(const Anchor& anchor) {
    char anchorValue = static_cast<char>(anchor);
    return (anchorValue & (1|2)) * 0.5;
  }

  qreal getYMultiplier(const Anchor& anchor) {
    char anchorValue = static_cast<char>(anchor);
    return (anchorValue & (4|8)) * 0.125;
  }

  QPointF BoundingBoxLinker::getShift(const QRectF &reference, const QRectF &target) const {
    Q_D(const BoundingBoxLinker);
    QPointF topLeftShift(reference.topLeft() - target.topLeft());
    qreal xShift = getXMultiplier(d->origin) * reference.width() - getXMultiplier(d->target) * target.width();
    qreal yShift = getYMultiplier(d->origin) * reference.height() - getYMultiplier(d->target) * target.height();
    return topLeftShift + QPointF(xShift, yShift) + d->offset;
  }

  QString BoundingBoxLinker::xmlName() const {
    return "bbLinker";
  }

  QXmlStreamAttributes BoundingBoxLinker::xmlAttributes() const {
    Q_D(const BoundingBoxLinker);
    QXmlStreamAttributes attributes;
    attributes.append("originAnchor", toString(d->origin));
    attributes.append("targetAnchor", toString(d->target));
    attributes.append("xOffset", QString::number(d->offset.x()));
    attributes.append("yOffset", QString::number(d->offset.y()));
    return attributes;
  }

  void BoundingBoxLinker::readAttributes(const QXmlStreamAttributes &attributes) {
    Q_D(BoundingBoxLinker);
    d->origin = anchorFromString(attributes.value("originAnchor").toString());
    d->target = anchorFromString(attributes.value("targetAnchor").toString());
    d->offset.setX(attributes.value("xOffset").toDouble());
    d->offset.setY(attributes.value("yOffset").toDouble());
  }

  bool BoundingBoxLinker::operator==(const BoundingBoxLinker &other) const {
    Q_D(const BoundingBoxLinker);
    const BoundingBoxLinkerPrivate* const otherD = other.d_func();
    return *d == *otherD;
  }

  BoundingBoxLinker &BoundingBoxLinker::operator=(const BoundingBoxLinker &other) {
    if (&other != this) d_ptr.reset(new BoundingBoxLinkerPrivate(*(other.d_ptr)));
    return *this;
  }

  int convertAnchor(const Anchor &anchor) {
    return static_cast<char>(anchor);
  }

  Anchor convertAnchor(const int &integer) {
    Anchor result = static_cast<Anchor>(integer);
    switch(result) {
      case Anchor::Top:
      case Anchor::Bottom:
      case Anchor::Left:
      case Anchor::Right:
      case Anchor::TopLeft:
      case Anchor::BottomLeft:
      case Anchor::TopRight:
      case Anchor::BottomRight:
      case Anchor::Center:
        return result;
      default:
        return Anchor::Center;
    }
  }

  Anchor anchorFromString(QString input) {
    auto cleaned = input.remove(QRegularExpression("\\s")).toLower();
    if ("center" == cleaned) return Anchor::Center;
    if ("left" == cleaned) return Anchor::Left;
    if ("right" == cleaned) return Anchor::Right;
    if ("top" == cleaned) return Anchor::Top;
    if ("bottom" == cleaned) return Anchor::Bottom;
    if ("topleft" == cleaned) return Anchor::TopLeft;
    if ("topright" == cleaned) return Anchor::TopRight;
    if ("bottomleft" == cleaned) return Anchor::BottomLeft;
    if ("bottomright" == cleaned) return Anchor::BottomRight;
    return Anchor::Center;
  }

  QString toString(const Anchor &anchor) {
    switch(anchor) {
      case Anchor::Center: return "Center";
      case Anchor::Left: return "Left";
      case Anchor::Right: return "Right";
      case Anchor::Top: return "Top";
      case Anchor::Bottom: return "Bottom";
      case Anchor::TopLeft: return "TopLeft";
      case Anchor::TopRight: return "TopRight";
      case Anchor::BottomLeft: return "BottomLeft";
      case Anchor::BottomRight: return "BottomRight";
    }
    return "Center";
  }

  QDebug operator<<(QDebug debug, const Anchor &anchor) {
    debug << toString(anchor);
    return debug;
  }

  QDebug operator<<(QDebug debug, const BoundingBoxLinker &linker) {
    debug.nospace() << "BoundingBoxLinker(origin ref: " << linker.d_func()->origin
                    << ", target ref: " << linker.d_func()->target
                    << ", offset: " << linker.d_func()->offset
                    << ")";
    return debug;
  }

  const BoundingBoxLinker BoundingBoxLinker::above = BoundingBoxLinker(Anchor::Top, Anchor::Bottom);
  const BoundingBoxLinker BoundingBoxLinker::below = BoundingBoxLinker(Anchor::Bottom, Anchor::Top);
  const BoundingBoxLinker BoundingBoxLinker::toLeft = BoundingBoxLinker(Anchor::Left, Anchor::Right);
  const BoundingBoxLinker BoundingBoxLinker::toRight = BoundingBoxLinker(Anchor::Right, Anchor::Left);
  const BoundingBoxLinker BoundingBoxLinker::upperLeft = BoundingBoxLinker(Anchor::TopLeft, Anchor::BottomRight);
  const BoundingBoxLinker BoundingBoxLinker::upperRight = BoundingBoxLinker(Anchor::TopRight, Anchor::BottomLeft);
  const BoundingBoxLinker BoundingBoxLinker::lowerLeft = BoundingBoxLinker(Anchor::BottomLeft, Anchor::TopRight);
  const BoundingBoxLinker BoundingBoxLinker::lowerRight = BoundingBoxLinker(Anchor::BottomRight, Anchor::TopLeft);

  // TODO consider converting constructor
  const BoundingBoxLinker BoundingBoxLinker::atBottom = BoundingBoxLinker(Anchor::Bottom);
  const BoundingBoxLinker BoundingBoxLinker::atTop = BoundingBoxLinker(Anchor::Top);
  const BoundingBoxLinker BoundingBoxLinker::atLeft = BoundingBoxLinker(Anchor::Left);
  const BoundingBoxLinker BoundingBoxLinker::atRight = BoundingBoxLinker(Anchor::Right);
  const BoundingBoxLinker BoundingBoxLinker::atTopLeft = BoundingBoxLinker(Anchor::TopLeft);
  const BoundingBoxLinker BoundingBoxLinker::atTopRight = BoundingBoxLinker(Anchor::TopRight);
  const BoundingBoxLinker BoundingBoxLinker::atBottomLeft = BoundingBoxLinker(Anchor::BottomLeft);
  const BoundingBoxLinker BoundingBoxLinker::atBottomRight = BoundingBoxLinker(Anchor::BottomRight);
} // namespace Molsketch
