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

const QString BBLINKER_XML("<BBLinker originAnchor=\"TopLeft\" targetAnchor=\"TopRight\" xOffset=\"4.5\" yOffset=\"7.25\"/>");
const BoundingBoxLinker SAMPLE_LINKER(Anchor::TopLeft, Anchor::TopRight, QPointF(4.5, 7.25));

QString toString(const QPointF& point) {
  return "(" + QString::number(point.x()) + ", " + QString::number(point.y()) +")";
}

class BoundingBoxLinkerUnitTest : public CxxTest::TestSuite {
  QPointF getShift(const QRectF& reference, const QRectF& subject,
                   Anchor referenceAnchor, Anchor subjectAnchor){
    BoundingBoxLinker linker(referenceAnchor, subjectAnchor);
    return linker.getShift(reference, subject);
  }

  QPointF getShiftWithOffset(const QRectF& reference, const QRectF& subject,
                             Anchor referenceAnchor, Anchor subjectAnchor,
                             const QPointF& offset) {
    BoundingBoxLinker linker(referenceAnchor, subjectAnchor, offset);
    return linker.getShift(reference, subject);
  }

  void assertShift(const QRectF& reference, const QRectF& subject,
                   Anchor referenceAnchor, Anchor subjectAnchor,
                   const QPointF& expected) {
    QPointF actual = getShift(reference, subject, referenceAnchor, subjectAnchor);
    QString message = "Mismatch for reference anchor " + QString::number(convertAnchor(referenceAnchor)) + " and subject anchor " + QString::number(convertAnchor(subjectAnchor)) + ". Expected: " + toString(expected) + " Actual: " + toString(actual);
    QSM_ASSERT_EQUALS(message, actual, expected);
  }

  void assertShiftWithOffset(const QRectF& reference, const QRectF& subject,
                             Anchor referenceAnchor, Anchor subjectAnchor,
                             const QPointF& offset, const QPointF& expected) {
    QPointF actual = getShiftWithOffset(reference, subject, referenceAnchor, subjectAnchor, offset);
    QString message = "Mismatch for reference anchor " + QString::number(static_cast<char>(referenceAnchor)) + " and subject anchor " + QString::number(static_cast<char>(subjectAnchor)) + " with offset " + toString(offset) + ". Expected: " + toString(expected) + " Actual: " + toString(actual);
    QSM_ASSERT_EQUALS(message, actual, expected);
  }

  QVector<std::tuple<Anchor, Anchor, QPointF>> testCases() {
    Anchor Center = Anchor::Center;
    Anchor Left = Anchor::Left;
    Anchor Right = Anchor::Right;
    Anchor Top = Anchor::Top;
    Anchor Bottom = Anchor::Bottom;
    Anchor TopLeft = Anchor::TopLeft;
    Anchor TopRight = Anchor::TopRight;
    Anchor BottomLeft = Anchor::BottomLeft;
    Anchor BottomRight = Anchor::BottomRight;
    return QVector<std::tuple<Anchor, Anchor, QPointF>>()
        << std::make_tuple(Center, Center, QPointF(1.625, 20.875))
        << std::make_tuple(Center, Top, QPointF(1.625, 22.875))
        << std::make_tuple(Center, Bottom, QPointF(1.625, 18.875))
        << std::make_tuple(Center, Left, QPointF(3.375, 20.875))
        << std::make_tuple(Center, Right, QPointF(-.125, 20.875))
        << std::make_tuple(Center, TopLeft, QPointF(3.375, 22.875))
        << std::make_tuple(Center, BottomLeft, QPointF(3.375, 18.875))
        << std::make_tuple(Center, TopRight, QPointF(-.125, 22.875))
        << std::make_tuple(Center, BottomRight, QPointF(-.125, 18.875))

        << std::make_tuple(Top, Center, QPointF(1.625, 14.375))
        << std::make_tuple(Top, Top, QPointF(1.625, 16.375))
        << std::make_tuple(Top, Bottom, QPointF(1.625, 12.375))
        << std::make_tuple(Top, Left, QPointF(3.375, 14.375))
        << std::make_tuple(Top, Right, QPointF(-.125, 14.375))
        << std::make_tuple(Top, TopLeft, QPointF(3.375, 16.375))
        << std::make_tuple(Top, BottomLeft, QPointF(3.375, 12.375))
        << std::make_tuple(Top, TopRight, QPointF(-.125, 16.375))
        << std::make_tuple(Top, BottomRight, QPointF(-.125, 12.375))

        << std::make_tuple(Bottom, Center, QPointF(1.625, 27.375))
        << std::make_tuple(Bottom, Top, QPointF(1.625, 29.375))
        << std::make_tuple(Bottom, Bottom, QPointF(1.625, 25.375))
        << std::make_tuple(Bottom, Left, QPointF(3.375, 27.375))
        << std::make_tuple(Bottom, Right, QPointF(-.125, 27.375))
        << std::make_tuple(Bottom, TopLeft, QPointF(3.375, 29.375))
        << std::make_tuple(Bottom, BottomLeft, QPointF(3.375, 25.375))
        << std::make_tuple(Bottom, TopRight, QPointF(-.125, 29.375))
        << std::make_tuple(Bottom, BottomRight, QPointF(-.125, 25.375))

        << std::make_tuple(Left, Center, QPointF(-8.625, 20.875))
        << std::make_tuple(Left, Top, QPointF(-8.625, 22.875))
        << std::make_tuple(Left, Bottom, QPointF(-8.625, 18.875))
        << std::make_tuple(Left, Left, QPointF(-6.875, 20.875))
        << std::make_tuple(Left, Right, QPointF(-10.375, 20.875))
        << std::make_tuple(Left, TopLeft, QPointF(-6.875, 22.875))
        << std::make_tuple(Left, BottomLeft, QPointF(-6.875, 18.875))
        << std::make_tuple(Left, TopRight, QPointF(-10.375, 22.875))
        << std::make_tuple(Left, BottomRight, QPointF(-10.375, 18.875))

        << std::make_tuple(Right, Center, QPointF(11.875, 20.875))
        << std::make_tuple(Right, Top, QPointF(11.875, 22.875))
        << std::make_tuple(Right, Bottom, QPointF(11.875, 18.875))
        << std::make_tuple(Right, Left, QPointF(13.625, 20.875))
        << std::make_tuple(Right, Right, QPointF(10.125, 20.875))
        << std::make_tuple(Right, TopLeft, QPointF(13.625, 22.875))
        << std::make_tuple(Right, BottomLeft, QPointF(13.625, 18.875))
        << std::make_tuple(Right, TopRight, QPointF(10.125, 22.875))
        << std::make_tuple(Right, BottomRight, QPointF(10.125, 18.875))

        << std::make_tuple(TopLeft, Center, QPointF(-8.625, 14.375))
        << std::make_tuple(TopLeft, Top, QPointF(-8.625, 16.375))
        << std::make_tuple(TopLeft, Bottom, QPointF(-8.625, 12.375))
        << std::make_tuple(TopLeft, Left, QPointF(-6.875, 14.375))
        << std::make_tuple(TopLeft, Right, QPointF(-10.375, 14.375))
        << std::make_tuple(TopLeft, TopLeft, QPointF(-6.875, 16.375))
        << std::make_tuple(TopLeft, BottomLeft, QPointF(-6.875, 12.375))
        << std::make_tuple(TopLeft, TopRight, QPointF(-10.375, 16.375))
        << std::make_tuple(TopLeft, BottomRight, QPointF(-10.375, 12.375))

        << std::make_tuple(BottomLeft, Center, QPointF(-8.625, 27.375))
        << std::make_tuple(BottomLeft, Top, QPointF(-8.625, 29.375))
        << std::make_tuple(BottomLeft, Bottom, QPointF(-8.625, 25.375))
        << std::make_tuple(BottomLeft, Left, QPointF(-6.875, 27.375))
        << std::make_tuple(BottomLeft, Right, QPointF(-10.375, 27.375))
        << std::make_tuple(BottomLeft, TopLeft, QPointF(-6.875, 29.375))
        << std::make_tuple(BottomLeft, BottomLeft, QPointF(-6.875, 25.375))
        << std::make_tuple(BottomLeft, TopRight, QPointF(-10.375, 29.375))
        << std::make_tuple(BottomLeft, BottomRight, QPointF(-10.375, 25.375))

        << std::make_tuple(TopRight, Center, QPointF(11.875, 14.375))
        << std::make_tuple(TopRight, Top, QPointF(11.875, 16.375))
        << std::make_tuple(TopRight, Bottom, QPointF(11.875, 12.375))
        << std::make_tuple(TopRight, Left, QPointF(13.625, 14.375))
        << std::make_tuple(TopRight, Right, QPointF(10.125, 14.375))
        << std::make_tuple(TopRight, TopLeft, QPointF(13.625, 16.375))
        << std::make_tuple(TopRight, BottomLeft, QPointF(13.625, 12.375))
        << std::make_tuple(TopRight, TopRight, QPointF(10.125, 16.375))
        << std::make_tuple(TopRight, BottomRight, QPointF(10.125, 12.375))

        << std::make_tuple(BottomRight, Center, QPointF(11.875, 27.375))
        << std::make_tuple(BottomRight, Top, QPointF(11.875, 29.375))
        << std::make_tuple(BottomRight, Bottom, QPointF(11.875, 25.375))
        << std::make_tuple(BottomRight, Left, QPointF(13.625, 27.375))
        << std::make_tuple(BottomRight, Right, QPointF(10.125, 27.375))
        << std::make_tuple(BottomRight, TopLeft, QPointF(13.625, 29.375))
        << std::make_tuple(BottomRight, BottomLeft, QPointF(13.625, 25.375))
        << std::make_tuple(BottomRight, TopRight, QPointF(10.125, 29.375))
        << std::make_tuple(BottomRight, BottomRight, QPointF(10.125, 25.375));
  }

  QVector<Anchor> allEnumValues() {
    return QVector<Anchor>()
        << Anchor::Center
        << Anchor::Left
        << Anchor::Right
        << Anchor::Top
        << Anchor::Bottom
        << Anchor::TopLeft
        << Anchor::TopRight
        << Anchor::BottomLeft
        << Anchor::BottomRight;
  }

public:
  void setUp() {
  }

  void tearDown() {
  }

  void testMovingToAnchor() {
    QRectF referenceBoundingBox(3.5, 5.125, 20.5, 13),
        ownBoundingBox(10.375, -11.25, 3.5, 4);
    for (auto testCase : testCases())
      assertShift(referenceBoundingBox, ownBoundingBox, std::get<0>(testCase), std::get<1>(testCase), std::get<2>(testCase));
  }

  void testMovingWithOffset() {
    QRectF referenceBoundingBox(3.5, 5.125, 20.5, 13),
        ownBoundingBox(10.375, -11.25, 3.5, 4);
    QPointF offset(51.375, -10.375);
    for (auto testCase : testCases())
      assertShiftWithOffset(referenceBoundingBox, ownBoundingBox, std::get<0>(testCase), std::get<1>(testCase), offset, offset + std::get<2>(testCase));
  }

  void testAnchorStringConversion() {
    for (Anchor anchor : allEnumValues())
      QSM_ASSERT_EQUALS("String round trip conversion failed for: " + QString::number(convertAnchor(anchor)), anchorFromString(toString(anchor)), anchor);
  }

  void testAnchorStringConversionBadlyFormatted() {
    QS_ASSERT_EQUALS(anchorFromString("   top Left"), Anchor::TopLeft);
  }

  void testXmlOutputOfLinker() {
    QByteArray output;
    QXmlStreamWriter writer(&output);
    BoundingBoxLinker linker(Anchor::TopLeft, Anchor::TopRight, QPointF(4.5, 7.25));
    SAMPLE_LINKER.writeXml(writer);
    QS_ASSERT_EQUALS(BBLINKER_XML, output);
  }

  void testXmlInputOfLinker() {
    QXmlStreamReader reader(BBLINKER_XML);
    BoundingBoxLinker input;
    reader.readNextStartElement();
    input.readXml(reader);
    QS_ASSERT_EQUALS(input, SAMPLE_LINKER);
  }
    // TODO random test: top - bottom = height of box
};
