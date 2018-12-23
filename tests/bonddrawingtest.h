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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#include <cxxtest/TestSuite.h>

#include <molscene.h>
#include <atom.h>
#include <bond.h>
#include <molecule.h>
#include <QSet>
#include <QDebug>
#include <QXmlQuery>
#include <QXmlResultItems>
#include "xmlassertion.h"
#include <scenesettings.h>
#include <settingsitem.h>

using namespace Molsketch;

const char QUERY_LINE_COORDS[] = "//*:polyline/@points/data(.)";

class BondDrawingTest : public CxxTest::TestSuite {
  MolScene *scene;
  Atom *a1, *a2;
  Bond *b;
  Molecule *m;

  void assertLineCoords(const QString& value) {
    XmlAssertion::assertThat(scene->toSvg())->contains(QUERY_LINE_COORDS)->exactlyOnceWithContent(value);
  }

public:
  void setUp() {
    a1 = new Atom(QPointF(), "H");
    a2 = new Atom(QPointF(), "H");
    b = new Bond(a1, a2);
    m = new Molecule(QSet<Atom*>{a1, a2}, QSet<Bond*>{b});
    scene = new MolScene;
    scene->settings()->atomFont()->set(QFont("Noto Sans", 8));
    scene->addItem(m);
  }

  void tearDown() {
    delete scene;
  }

  void testDrawingDownFromAtom() { // TODO these tests could go into a data provider
    a2->setCoordinates(QPolygonF() << QPointF(0,50));
    assertLineCoords("0,7.33333 0,42.6667 ");
  }

  void testDrawingRightFromAtom() {
    a2->setCoordinates(QPolygonF() << QPointF(50,0));
    assertLineCoords("4.33333,0 45.6667,0 ");
  }

  void testDrawingUpFromAtom() {
    a2->setCoordinates(QPolygonF() << QPointF(0,-50));
    assertLineCoords("0,-7.33333 0,-42.6667 ");
  }

  void testDrawingLeftFromAtom() {
    a2->setCoordinates(QPolygonF() << QPointF(-50,0));
    assertLineCoords("-4.33333,0 -45.6667,0 ");
  }

  void testDrawingUpLeftFromAtom() {
    a2->setCoordinates(QPolygonF() << QPointF(-20,-30));
    assertLineCoords("-4.33333,-6.5 -15.6667,-23.5 ");
  }

  void testDrawingUpRightFromAtom() {
    a2->setCoordinates(QPolygonF() << QPointF(20,-30));
    assertLineCoords("4.33333,-6.5 15.6667,-23.5 ");
  }

  void testDrawingDownLeftFromAtom() {
    a2->setCoordinates(QPolygonF() << QPointF(-20,30));
    assertLineCoords("-4.33333,6.5 -15.6667,23.5 ");
  }

  void testDrawingDownRightFromAtom() {
    a2->setCoordinates(QPolygonF() << QPointF(20,30));
    assertLineCoords("4.33333,6.5 15.6667,23.5 ");
  }

  void testDrawingFromUndrawnAtoms() {
    a1->setElement("C");
    a2->setElement("C");
    scene->settings()->showTerminalMethyls()->set(false);
    a2->setCoordinates(QPolygonF() << QPointF(20,30));
    assertLineCoords("0,0 20,30 ");
  }

  void testDrawingFromNewmanAtom() {
    a1->setNewmanDiameter(6);
    a2->setCoordinates(QPolygonF() << QPointF(50,50));
    assertLineCoords("2.82843,2.82843 45.6667,45.6667 ");
  }

  void testDrawingFromNewmanAtomIfCarbon() {
    a1->setNewmanDiameter(6);
    a2->setCoordinates(QPolygonF() << QPointF(50,50));
    Atom *a3 = new Atom(QPointF(-50,-70), "C");
    a1->setElement("C");
    m->addAtom(a3);
    m->addBond(a1, a3);
    XmlAssertion::assertThat(scene->toSvg())->contains(QUERY_LINE_COORDS)
        ->inAnyOrderWithValues({"2.82843,2.82843 45.6667,45.6667 ", "-2.32495,-3.25493 -46.1667,-64.6333 "});
  }

  void testDrawingSecondAtomInOriginBond() {
    a1->setCoordinates(QPolygonF() << QPointF(20,30));
    assertLineCoords("15.6667,23.5 4.33333,6.5 ");
  }

  void testDrawingNonOriginBasedBond() {
    a1->setCoordinates(QPolygonF() << QPointF(20,30));
    a2->setCoordinates(QPolygonF() << QPointF(-5,17));
    assertLineCoords("15.6667,27.7467 -0.666667,19.2533 ");
  }

  void testBondNotDrawnIfOverlapsWithNewmanAtom() {
    a1->setNewmanDiameter(6);
    Atom *a3 = new Atom(QPointF(-50,-70), "C");
    a2->setElement("C");
    m->addAtom(a3);
    m->addBond(a2, a3, Bond::Invalid);
    a2->setCoordinates(QPolygonF() << QPointF(1,1));
    XmlAssertion::assertThat(scene->toSvg())->contains(QUERY_LINE_COORDS)->never();
  }

  void testBondNotDrawnIfOverlapsWithNewmanAtomReverse() {
    a2->setNewmanDiameter(6);
    Atom *a3 = new Atom(QPointF(-50,-70), "C");
    a1->setElement("C");
    m->addAtom(a3);
    m->addBond(a1, a3, Bond::Invalid);
    a1->setCoordinates(QPolygonF() << QPointF(1,1));
    XmlAssertion::assertThat(scene->toSvg())->contains(QUERY_LINE_COORDS)->never();
  }
};
