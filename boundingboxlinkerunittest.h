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

#include <cxxtest/TestSuite.h>
#include "utilities.h"
#include <QRectF>
#include <tuple>
#include <boundingboxlinker.h>

using namespace Molsketch;

QString toString(const QPointF& point) {
  return "(" + QString::number(point.x()) + ", " + QString::number(point.y()) +")";
}

class BoundingBoxLinkerUnitTest : public CxxTest::TestSuite {
  QPointF getShift(const QRectF& reference, const QRectF& subject,
                   Anchor referenceAnchor, Anchor subjectAnchor){
    BoundingBoxLinker linker(referenceAnchor, subjectAnchor);
    return linker.getShift(reference, subject);
  }

  void assertShift(const QRectF& reference, const QRectF& subject,
                   Anchor referenceAnchor, Anchor subjectAnchor,
                   const QPointF& expected) {
    QPointF actual = getShift(reference, subject, referenceAnchor, subjectAnchor);
    QString message = "Mismatch for reference anchor " + QString::number(referenceAnchor) + " and subject anchor " + QString::number(subjectAnchor) + ". Expected: " + toString(expected) + " Actual: " + toString(actual);
    QSM_ASSERT_EQUALS(message, actual, expected);
  }

public:
  void setUp() {
  }

  void tearDown() {
  }

  void testMovingToAnchor() {
    QRectF referenceBoundingBox(3.5, 5.125, 20.5, 13),
        ownBoundingBox(10.375, -11.25, 3.5, 4);

    assertShift(referenceBoundingBox, ownBoundingBox, Center, Center, QPointF(1.625, 20.875));
    assertShift(referenceBoundingBox, ownBoundingBox, Center, Top, QPointF(1.625, 22.875));
    assertShift(referenceBoundingBox, ownBoundingBox, Center, Bottom, QPointF(1.625, 18.875));
//    // TODO random test: top - bottom = height of box
    assertShift(referenceBoundingBox, ownBoundingBox, Center, Left, QPointF(3.375, 20.875));
    assertShift(referenceBoundingBox, ownBoundingBox, Center, Right, QPointF(-.125, 20.875));
    assertShift(referenceBoundingBox, ownBoundingBox, Center, TopLeft, QPointF(3.375, 22.875));
    assertShift(referenceBoundingBox, ownBoundingBox, Center, BottomLeft, QPointF(3.375, 18.875));
    assertShift(referenceBoundingBox, ownBoundingBox, Center, TopRight, QPointF(-.125, 22.875));
    assertShift(referenceBoundingBox, ownBoundingBox, Center, BottomRight, QPointF(-.125, 18.875));

    assertShift(referenceBoundingBox, ownBoundingBox, Top, Center, QPointF(1.625, 14.375));
    assertShift(referenceBoundingBox, ownBoundingBox, Top, Top, QPointF(1.625, 16.375));
    assertShift(referenceBoundingBox, ownBoundingBox, Top, Bottom, QPointF(1.625, 12.375));
    assertShift(referenceBoundingBox, ownBoundingBox, Top, Left, QPointF(3.375, 14.375));
    assertShift(referenceBoundingBox, ownBoundingBox, Top, Right, QPointF(-.125, 14.375));
    assertShift(referenceBoundingBox, ownBoundingBox, Top, TopLeft, QPointF(3.375, 16.375));
    assertShift(referenceBoundingBox, ownBoundingBox, Top, BottomLeft, QPointF(3.375, 12.375));
    assertShift(referenceBoundingBox, ownBoundingBox, Top, TopRight, QPointF(-.125, 16.375));
    assertShift(referenceBoundingBox, ownBoundingBox, Top, BottomRight, QPointF(-.125, 12.375));

    assertShift(referenceBoundingBox, ownBoundingBox, Bottom, Center, QPointF(1.625, 27.375));
    assertShift(referenceBoundingBox, ownBoundingBox, Bottom, Top, QPointF(1.625, 29.375));
    assertShift(referenceBoundingBox, ownBoundingBox, Bottom, Bottom, QPointF(1.625, 25.375));
    assertShift(referenceBoundingBox, ownBoundingBox, Bottom, Left, QPointF(3.375, 27.375));
    assertShift(referenceBoundingBox, ownBoundingBox, Bottom, Right, QPointF(-.125, 27.375));
    assertShift(referenceBoundingBox, ownBoundingBox, Bottom, TopLeft, QPointF(3.375, 29.375));
    assertShift(referenceBoundingBox, ownBoundingBox, Bottom, BottomLeft, QPointF(3.375, 25.375));
    assertShift(referenceBoundingBox, ownBoundingBox, Bottom, TopRight, QPointF(-.125, 29.375));
    assertShift(referenceBoundingBox, ownBoundingBox, Bottom, BottomRight, QPointF(-.125, 25.375));

    assertShift(referenceBoundingBox, ownBoundingBox, Left, Center, QPointF(-8.625, 20.875));
    assertShift(referenceBoundingBox, ownBoundingBox, Left, Top, QPointF(-8.625, 22.875));
    assertShift(referenceBoundingBox, ownBoundingBox, Left, Bottom, QPointF(-8.625, 18.875));
    assertShift(referenceBoundingBox, ownBoundingBox, Left, Left, QPointF(-6.875, 20.875));
    assertShift(referenceBoundingBox, ownBoundingBox, Left, Right, QPointF(-10.375, 20.875));
    assertShift(referenceBoundingBox, ownBoundingBox, Left, TopLeft, QPointF(-6.875, 22.875));
    assertShift(referenceBoundingBox, ownBoundingBox, Left, BottomLeft, QPointF(-6.875, 18.875));
    assertShift(referenceBoundingBox, ownBoundingBox, Left, TopRight, QPointF(-10.375, 22.875));
    assertShift(referenceBoundingBox, ownBoundingBox, Left, BottomRight, QPointF(-10.375, 18.875));

    assertShift(referenceBoundingBox, ownBoundingBox, Right, Center, QPointF(11.875, 20.875));
    assertShift(referenceBoundingBox, ownBoundingBox, Right, Top, QPointF(11.875, 22.875));
    assertShift(referenceBoundingBox, ownBoundingBox, Right, Bottom, QPointF(11.875, 18.875));
    assertShift(referenceBoundingBox, ownBoundingBox, Right, Left, QPointF(13.625, 20.875));
    assertShift(referenceBoundingBox, ownBoundingBox, Right, Right, QPointF(10.125, 20.875));
    assertShift(referenceBoundingBox, ownBoundingBox, Right, TopLeft, QPointF(13.625, 22.875));
    assertShift(referenceBoundingBox, ownBoundingBox, Right, BottomLeft, QPointF(13.625, 18.875));
    assertShift(referenceBoundingBox, ownBoundingBox, Right, TopRight, QPointF(10.125, 22.875));
    assertShift(referenceBoundingBox, ownBoundingBox, Right, BottomRight, QPointF(10.125, 18.875));

    assertShift(referenceBoundingBox, ownBoundingBox, TopLeft, Center, QPointF(-8.625, 14.375));
    assertShift(referenceBoundingBox, ownBoundingBox, TopLeft, Top, QPointF(-8.625, 16.375));
    assertShift(referenceBoundingBox, ownBoundingBox, TopLeft, Bottom, QPointF(-8.625, 12.375));
    assertShift(referenceBoundingBox, ownBoundingBox, TopLeft, Left, QPointF(-6.875, 14.375));
    assertShift(referenceBoundingBox, ownBoundingBox, TopLeft, Right, QPointF(-10.375, 14.375));
    assertShift(referenceBoundingBox, ownBoundingBox, TopLeft, TopLeft, QPointF(-6.875, 16.375));
    assertShift(referenceBoundingBox, ownBoundingBox, TopLeft, BottomLeft, QPointF(-6.875, 12.375));
    assertShift(referenceBoundingBox, ownBoundingBox, TopLeft, TopRight, QPointF(-10.375, 16.375));
    assertShift(referenceBoundingBox, ownBoundingBox, TopLeft, BottomRight, QPointF(-10.375, 12.375));

    assertShift(referenceBoundingBox, ownBoundingBox, BottomLeft, Center, QPointF(-8.625, 27.375));
    assertShift(referenceBoundingBox, ownBoundingBox, BottomLeft, Top, QPointF(-8.625, 29.375));
    assertShift(referenceBoundingBox, ownBoundingBox, BottomLeft, Bottom, QPointF(-8.625, 25.375));
    assertShift(referenceBoundingBox, ownBoundingBox, BottomLeft, Left, QPointF(-6.875, 27.375));
    assertShift(referenceBoundingBox, ownBoundingBox, BottomLeft, Right, QPointF(-10.375, 27.375));
    assertShift(referenceBoundingBox, ownBoundingBox, BottomLeft, TopLeft, QPointF(-6.875, 29.375));
    assertShift(referenceBoundingBox, ownBoundingBox, BottomLeft, BottomLeft, QPointF(-6.875, 25.375));
    assertShift(referenceBoundingBox, ownBoundingBox, BottomLeft, TopRight, QPointF(-10.375, 29.375));
    assertShift(referenceBoundingBox, ownBoundingBox, BottomLeft, BottomRight, QPointF(-10.375, 25.375));

    assertShift(referenceBoundingBox, ownBoundingBox, TopRight, Center, QPointF(11.875, 14.375));
    assertShift(referenceBoundingBox, ownBoundingBox, TopRight, Top, QPointF(11.875, 16.375));
    assertShift(referenceBoundingBox, ownBoundingBox, TopRight, Bottom, QPointF(11.875, 12.375));
    assertShift(referenceBoundingBox, ownBoundingBox, TopRight, Left, QPointF(13.625, 14.375));
    assertShift(referenceBoundingBox, ownBoundingBox, TopRight, Right, QPointF(10.125, 14.375));
    assertShift(referenceBoundingBox, ownBoundingBox, TopRight, TopLeft, QPointF(13.625, 16.375));
    assertShift(referenceBoundingBox, ownBoundingBox, TopRight, BottomLeft, QPointF(13.625, 12.375));
    assertShift(referenceBoundingBox, ownBoundingBox, TopRight, TopRight, QPointF(10.125, 16.375));
    assertShift(referenceBoundingBox, ownBoundingBox, TopRight, BottomRight, QPointF(10.125, 12.375));

    assertShift(referenceBoundingBox, ownBoundingBox, BottomRight, Center, QPointF(11.875, 27.375));
    assertShift(referenceBoundingBox, ownBoundingBox, BottomRight, Top, QPointF(11.875, 29.375));
    assertShift(referenceBoundingBox, ownBoundingBox, BottomRight, Bottom, QPointF(11.875, 25.375));
    assertShift(referenceBoundingBox, ownBoundingBox, BottomRight, Left, QPointF(13.625, 27.375));
    assertShift(referenceBoundingBox, ownBoundingBox, BottomRight, Right, QPointF(10.125, 27.375));
    assertShift(referenceBoundingBox, ownBoundingBox, BottomRight, TopLeft, QPointF(13.625, 29.375));
    assertShift(referenceBoundingBox, ownBoundingBox, BottomRight, BottomLeft, QPointF(13.625, 25.375));
    assertShift(referenceBoundingBox, ownBoundingBox, BottomRight, TopRight, QPointF(10.125, 29.375));
    assertShift(referenceBoundingBox, ownBoundingBox, BottomRight, BottomRight, QPointF(10.125, 25.375));
  }
};
