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
#ifndef BOUNDINGRECTASSERTION_H
#define BOUNDINGRECTASSERTION_H

#include <QGraphicsItem>
#include <QScopedPointer>

class BoundingRectAssertionPrivate;

class BoundingRectAssertion {
  Q_DECLARE_PRIVATE(BoundingRectAssertion)
protected:
  QScopedPointer<BoundingRectAssertionPrivate> d_ptr;
  virtual ~BoundingRectAssertion();
  BoundingRectAssertion(const QRectF& rect, const QVariant &value, const QString &message);
  BoundingRectAssertion(BoundingRectAssertion&&);
};

class BoundingRectScalarAssertion : public BoundingRectAssertion {
public:
  static BoundingRectScalarAssertion leftEdgeOf(const QGraphicsItem*);
  static BoundingRectScalarAssertion rightEdgeOf(const QGraphicsItem*);
  static BoundingRectScalarAssertion bottomEdgeOf(const QGraphicsItem*);
  static BoundingRectScalarAssertion topEdgeOf(const QGraphicsItem*);
  static BoundingRectScalarAssertion centerXCoordinate(const QGraphicsItem*);
  static BoundingRectScalarAssertion centerYCoordinate(const QGraphicsItem*);
  BoundingRectScalarAssertion &is(qreal distance);
  BoundingRectScalarAssertion &fromLeftEdgeOf(const QGraphicsItem*);
  BoundingRectScalarAssertion &fromRightEdgeOf(const QGraphicsItem*);
  BoundingRectScalarAssertion &fromBottomEdgeOf(const QGraphicsItem*);
  BoundingRectScalarAssertion &fromTopEdgeOf(const QGraphicsItem*);
  BoundingRectScalarAssertion &fromCenterXCoordinateOf(const QGraphicsItem*);
  BoundingRectScalarAssertion &fromCenterYCoordinateOf(const QGraphicsItem*);
protected:
  BoundingRectScalarAssertion(const QRectF& rect, qreal value, const QString &message);
public:
  BoundingRectScalarAssertion(BoundingRectScalarAssertion&&) = default;
};


namespace BoundingRectAssert {
  BoundingRectScalarAssertion leftEdgeOf(const QGraphicsItem* item);
  BoundingRectScalarAssertion rightEdgeOf(const QGraphicsItem* item);
  BoundingRectScalarAssertion bottomEdgeOf(const QGraphicsItem*);
  BoundingRectScalarAssertion topEdgeOf(const QGraphicsItem*);
  BoundingRectScalarAssertion centerXCoordinateOf(const QGraphicsItem*);
  BoundingRectScalarAssertion centerYCoordinateOf(const QGraphicsItem*);
}

#endif // BOUNDINGRECTASSERTION_H
