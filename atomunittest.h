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

#include <bond.h>
#include <lonepair.h>

#include <atom.h>
#include <radicalelectron.h>
#include "utilities.h"
#include "xmlassertion.h"

using namespace Molsketch;
using XmlAssert::assertThat;

const QString ATOM_XML_WITH_RADICAL("<atom elementType=\"\" hydrogenCount=\"0\" colorR=\"0\" colorG=\"0\" colorB=\"0\" scalingParameter=\"1\" coordinates=\"0,0\">"
                                    "<radicalElectron diameter=\"3\" colorR=\"0\" colorG=\"0\" colorB=\"0\">"
                                    "<bbLinker originAnchor=\"Top\" targetAnchor=\"Bottom\" xOffset=\"0\" yOffset=\"0\"/>"
                                    "</radicalElectron>"
                                    "</atom>");
const QString ATOM_XML_WITH_LONE_PAIR("<atom elementType=\"\" hydrogenCount=\"0\" colorR=\"0\" colorG=\"0\" colorB=\"0\" scalingParameter=\"1\" coordinates=\"0,0\">"
                                      "<lonePair angle=\"45\" length=\"10\" lineWidth=\"1.5\" colorR=\"0\" colorG=\"0\" colorB=\"0\">"
                                      "<bbLinker originAnchor=\"TopLeft\" targetAnchor=\"Center\" xOffset=\"0\" yOffset=\"0\"/>"
                                      "</lonePair>"
                                      "</atom>");
const QString ATOM_XML_WITH_LONE_PAIR_AND_RADICAL(
    "<atom elementType=\"\" hydrogenCount=\"0\" colorR=\"0\" colorG=\"0\" colorB=\"0\" scalingParameter=\"1\" coordinates=\"0,0\">"
    "<lonePair angle=\"45\" length=\"10\" lineWidth=\"1.5\" colorR=\"0\" colorG=\"0\" colorB=\"0\">"
    "<bbLinker originAnchor=\"TopLeft\" targetAnchor=\"Center\" xOffset=\"0\" yOffset=\"0\"/>"
    "</lonePair>"
    "<radicalElectron diameter=\"3\" colorR=\"0\" colorG=\"0\" colorB=\"0\">"
    "<bbLinker originAnchor=\"Top\" targetAnchor=\"Bottom\" xOffset=\"0\" yOffset=\"0\"/>"
    "</radicalElectron>"
    "</atom>");
const char NEWMAN_XQUERY[] = "//*:atom/@newmanDiameter/data(.)";
const qreal RADICAL_DIAMETER = 3;
const qreal LONE_PAIR_ANGLE = 45;
const qreal LONE_PAIR_LINE_WIDTH = 1.5;
const qreal LONE_PAIR_LENGTH = 10;

class AtomUnitTest : public CxxTest::TestSuite {
  Atom *atom;
  RadicalElectron *radical;
  LonePair *lonePair;

  void assertXmlOutput(const QString expected) {
    QString output;
    QXmlStreamWriter writer(&output);
    atom->writeXml(writer);
    QS_ASSERT_EQUALS(output, expected);
  }

  void readXmlInput(const QString& input) {
    QXmlStreamReader reader(input);
    reader.readNextStartElement();
    atom->readXml(reader);
  }

public:
  void setUp() {
    atom = new Atom();
    lonePair = new LonePair(LONE_PAIR_ANGLE, LONE_PAIR_LINE_WIDTH, LONE_PAIR_LENGTH, BoundingBoxLinker::atTopLeft);
    radical = new RadicalElectron(RADICAL_DIAMETER);
  }

  void tearDown() {
    delete lonePair;
    delete radical;
    for (QGraphicsItem* childItem : atom->childItems())
      delete childItem;
    delete atom;
  }

  void testXmlOutputWithRadicalElectron() {
    radical->setParentItem(atom);
    assertXmlOutput(ATOM_XML_WITH_RADICAL);
  }

  void testXmlInputWithRadicalElectron() {
    readXmlInput(ATOM_XML_WITH_RADICAL);
    TS_ASSERT_EQUALS(atom->childItems().size(), 1);
    TS_ASSERT_DIFFERS(dynamic_cast<RadicalElectron*>(atom->childItems().first()), nullptr);
    // TODO delete atom's children?
  }

  void testXmlOutputWithLonePair() {
    lonePair->setParentItem(atom);
    assertXmlOutput(ATOM_XML_WITH_LONE_PAIR);
  }

  void testXmlInputWithLonePair() {
    readXmlInput(ATOM_XML_WITH_LONE_PAIR);
    TS_ASSERT_EQUALS(atom->childItems().size(), 1);
    TS_ASSERT_DIFFERS(dynamic_cast<LonePair*>(atom->childItems().first()), nullptr);
  }

  void testXmlOutputWithLonePairAndRadicalElectron() {
    lonePair->setParentItem(atom);
    radical->setParentItem(atom);
    assertXmlOutput(ATOM_XML_WITH_LONE_PAIR_AND_RADICAL);

  }

  void testXmlInputWithLonePairAndRadicalElectron() {
    readXmlInput(ATOM_XML_WITH_LONE_PAIR_AND_RADICAL);
    TS_ASSERT_EQUALS(atom->childItems().size(), 2);
    TS_ASSERT_DIFFERS(dynamic_cast<LonePair*>(atom->childItems()[0]), nullptr);
    TS_ASSERT_DIFFERS(dynamic_cast<RadicalElectron*>(atom->childItems()[1]), nullptr);
  }

  void testAtomIsShownIfItHasChildren() {
    Atom otherAtom, thirdAtom;
    Bond firstBond(atom, &otherAtom), secondBond(atom, &thirdAtom); // need at least two bonds
    atom->setCharge(-3); // neutralize charge

    atom->setElement("C");
    TS_ASSERT(!atom->isDrawn());

    lonePair->setParentItem(atom);
    TS_ASSERT(atom->isDrawn());

    firstBond.setAtoms(nullptr, nullptr);
    secondBond.setAtoms(nullptr, nullptr);
  }

  void testNormalAtomHasNoNewmanDiameter() {
    Atom atom(QPointF(), "C");
    assertThat(atom)->contains(NEWMAN_XQUERY)->never();
  }

  void testNewmanAtomHasNewmanDiameter() {
    Atom atom(QPointF(), "C");
    atom.setNewmanDiameter(5.5);
    TS_ASSERT_EQUALS(atom.getNewmanDiameter(), 5.5);
    assertThat(atom)->contains(NEWMAN_XQUERY)->exactlyOnceWithContent("5.5");
  }

  void testDisabledNewmanAtomHasNoNewmanDiameter() {
    Atom atom(QPointF(), "C");
    atom.setNewmanDiameter(5.5);
    TS_ASSERT_EQUALS(atom.getNewmanDiameter(), 5.5);
    atom.disableNewman();
    assertThat(atom)->contains(NEWMAN_XQUERY)->never();
  }

  void testAtomWithNewmanDiameterCanBeRead() {
    Atom atom;
    TS_ASSERT_EQUALS(atom.getNewmanDiameter(), 0);
    QXmlStreamReader in("<atom elementType=\"C\" hydrogenCount=\"0\" colorR=\"0\" colorG=\"0\" colorB=\"0\" scalingParameter=\"1\" coordinates=\"0,0\" newmanDiameter=\"3.5\"/>");
    in.readNextStartElement();
    atom.readXml(in);
    TS_ASSERT_EQUALS(atom.getNewmanDiameter(), 3.5);
  }

  void testAtomWithoutNewmanDiameterCanBeRead() {
    Atom atom;
    atom.setNewmanDiameter(5.5);
    TS_ASSERT_EQUALS(atom.getNewmanDiameter(), 5.5);
    QXmlStreamReader in("<atom elementType=\"C\" hydrogenCount=\"0\" colorR=\"0\" colorG=\"0\" colorB=\"0\" scalingParameter=\"1\" coordinates=\"0,0\"/>");
    in.readNextStartElement();
    atom.readXml(in);
    TS_ASSERT_EQUALS(atom.getNewmanDiameter(), 0);
  }

  void testBoundingRectForCarbonNewmanAtom() {
    Atom a1, a2;
    Bond b1(&a1, atom), b2(atom, &a2);
    atom->setElement("C");
    atom->setCharge(0);
    atom->setNewmanDiameter(6);
    TS_ASSERT(atom->isDrawn());
    QS_ASSERT_EQUALS(atom->boundingRect(), QRectF(-3, -3, 6, 6));
  }

  void testBondDrawingStart() {
    atom->setElement("A");
    Atom a1(QPointF(10,15)); // TODO more test cases
    QS_ASSERT_EQUALS(atom->bondDrawingStart(&a1), QPointF(2.5,3.75));
  }
};
