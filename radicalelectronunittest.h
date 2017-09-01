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
#include <molscene.h>
#include <radicalelectron.h>
#include "utilities.h"

using namespace Molsketch;

const int DIAMETER = 2;

class RadicalElectronUnitTest : public CxxTest::TestSuite {
  MolScene *scene;
  Atom *atom;

  int xmlElementCount(const QString& xml, const QString& element) {
    int count = 0;
    QXmlStreamReader reader(xml);
    while (findNextElement(reader, element))
      ++count;
    return count;
  }

  bool findNextElement(QXmlStreamReader& reader, const QString& element) {
    while (!reader.atEnd())
      if (QXmlStreamReader::StartElement == reader.readNext() && reader.name() == element)
        return true;
    return false;
  }

  QString svgWithAtomAndRadicalElectron() {
    RadicalElectron *radical = new RadicalElectron(DIAMETER);
    radical->setParentItem(atom);
    return scene->toSvg();
  }

public:
  void setUp() {
    scene = new MolScene;
    atom = new Atom(QPointF(0,0), "C");
    scene->addItem(atom);
  }

  void tearDown() {
    delete scene;
  }

  void testNothingDrawnIfNoParentWasSet() {
    scene->addItem(new RadicalElectron(DIAMETER));
    int circleCount = xmlElementCount(scene->toSvg(), "circle");
    TSM_ASSERT_EQUALS("No radical electrons should be drawn if parent was not assigned.", circleCount, 0);
  }

  void testCircleIsDrawnIfParentIsAtom() {
    int circleCount = xmlElementCount(svgWithAtomAndRadicalElectron(), "circle");
    TSM_ASSERT_EQUALS("A radical electron should be drawn if parent was not assigned.", circleCount, 1);
  }

  void testCircleSize() {
    QXmlStreamReader reader(svgWithAtomAndRadicalElectron());
    assertTrue(findNextElement(reader, "circle"), "Could not find circle in SVG!");
    TS_ASSERT_EQUALS(reader.attributes().value("r").toDouble() * 2, DIAMETER);
  }

  void testPosition() {
    QXmlStreamReader reader(svgWithAtomAndRadicalElectron());
    assertTrue(findNextElement(reader, "circle"), "Could not find circle in SVG!");
    TS_ASSERT_EQUALS(reader.attributes().value("cx").toDouble() * 2, 0);
    TS_ASSERT_EQUALS(reader.attributes().value("cy").toDouble() * 3, -19.5);
  }

  void testBoundingRectWithoutParent() {
    QS_ASSERT_EQUALS(RadicalElectron(DIAMETER).boundingRect(), QRectF());
  }

  void testAdaptationOfBoundingRectToParent() {
    QPointF offset(2,3);
    RadicalElectron *radical = new RadicalElectron(DIAMETER, BoundingBoxLinker(Anchor::TopRight, Anchor::BottomLeft, offset));
    radical->setParentItem(atom);
    QRectF boundingRect = radical->boundingRect();
    QS_ASSERT_EQUALS(boundingRect.bottomLeft(), atom->boundingRect().topRight() + offset);
    QS_ASSERT_EQUALS(boundingRect.width(), DIAMETER);
    QS_ASSERT_EQUALS(boundingRect.height(), DIAMETER);
  }

  // TODO check that radicals get deleted if atom gets deleted
};
