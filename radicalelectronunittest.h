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

CLASS_FOR_TESTING_WITH_FUNCTIONS(RadicalElectron, \
                                 public: \
                                 explicit RadicalElectronForTesting(qreal diameter, BoundingBoxLinker linker = BoundingBoxLinker(Anchor::Top, Anchor::Bottom), const QColor& color = QColor()) \
                                 : RadicalElectron(diameter, linker, color){})
const int DIAMETER = 2;
const QString RADICAL_ELECTRON_XML("<radicalElectron diameter=\"5\" colorR=\"255\" colorG=\"0\" colorB=\"0\"><bbLinker originAnchor=\"TopLeft\" targetAnchor=\"BottomRight\" xOffset=\"0\" yOffset=\"0\"/></radicalElectron>");
const RadicalElectron SAMPLE_RADICAL_ELECTRON(5, BoundingBoxLinker::upperLeft, Qt::red);

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
    TSM_ASSERT_EQUALS("Radical electrons should be deleted properly!", RadicalElectronForTesting::instanceCounter, 0);
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

  void testAdaptationOfBoundingRectToParentAndDeletionWithParent() {
    QPointF offset(2,3);
    RadicalElectron *radical = new RadicalElectronForTesting(DIAMETER, BoundingBoxLinker(Anchor::TopRight, Anchor::BottomLeft, offset));
    radical->setParentItem(atom);
    QRectF boundingRect = radical->boundingRect();
    QS_ASSERT_EQUALS(boundingRect.bottomLeft(), atom->boundingRect().topRight() + offset);
    QS_ASSERT_EQUALS(boundingRect.width(), DIAMETER);
    QS_ASSERT_EQUALS(boundingRect.height(), DIAMETER);
  }

  void testXmlOutput() {
    QByteArray output; // TODO write standardized output test routine
    QXmlStreamWriter writer(&output);
    SAMPLE_RADICAL_ELECTRON.writeXml(writer);
    QS_ASSERT_EQUALS(output, RADICAL_ELECTRON_XML);
  }

  void testXmlInput() {
    QXmlStreamReader reader(RADICAL_ELECTRON_XML);
    RadicalElectron radicalElectron(DIAMETER);
    reader.readNextStartElement();
    radicalElectron.readXml(reader);
    QS_ASSERT_EQUALS(radicalElectron, SAMPLE_RADICAL_ELECTRON);
  }

  // TODO test color!
};
