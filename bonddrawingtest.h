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
#include <atom.h>
#include <bond.h>
#include <molecule.h>
#include <QSet>
#include <QDebug>
#include <QXmlQuery>
#include <QXmlResultItems>

using namespace Molsketch;

class bondDrawingTest : public CxxTest::TestSuite {
  MolScene *scene;
  Atom *a1, *a2;
  Bond *b;
  Molecule *m;

  QString extractValueFromXml(const QString& xml, const QString& xQuery) {
    QString xmlOut(xml);
    QXmlResultItems xmlResults;
    QXmlQuery query;
    query.setFocus(xmlOut);
    query.setQuery(xQuery);
    query.evaluateTo(&xmlResults);

    QString result = xmlResults.next().toAtomicValue().toString();
    TS_ASSERT(xmlResults.next().isNull());
    TS_ASSERT(!xmlResults.hasError());
    return result;
  }

public:
  void setUp() {
    a1 = new Atom(QPointF(), "H");
    a2 = new Atom(QPointF(), "H");
    b = new Bond(a1, a2);
    m = new Molecule(QSet<Atom*>{a1, a2}, QSet<Bond*>{b});
    scene = new MolScene;
    scene->addItem(m);
  }

  void tearDown() {
    delete scene;
  }

  void testDrawingDownFromAtom() {
    a2->setCoordinates(QPolygonF() << QPointF(0,50));
    TS_ASSERT_EQUALS(extractValueFromXml(scene->toSvg(), "//*:polyline/@points/data(.)"), "0,7.5 0,42.5 ");
  }

  void testDrawingRightFromAtom() {
    a2->setCoordinates(QPolygonF() << QPointF(50,0));
    TS_ASSERT_EQUALS(extractValueFromXml(scene->toSvg(), "//*:polyline/@points/data(.)"), "5,0 45,0 ");
  }

  void testDrawingUpFromAtom() {
    a2->setCoordinates(QPolygonF() << QPointF(0,-50));
    TS_ASSERT_EQUALS(extractValueFromXml(scene->toSvg(), "//*:polyline/@points/data(.)"), "0,-7.5 0,-42.5 ");
  }

  void testDrawingLeftFromAtom() {
    a2->setCoordinates(QPolygonF() << QPointF(-50,0));
    TS_ASSERT_EQUALS(extractValueFromXml(scene->toSvg(), "//*:polyline/@points/data(.)"), "-5,0 -45,0 ");
  }

  void testDrawingUpLeftFromAtom() {
    a2->setCoordinates(QPolygonF() << QPointF(-20,-30));
    TS_ASSERT_EQUALS(extractValueFromXml(scene->toSvg(), "//*:polyline/@points/data(.)"), "-4.1094,-6.1641 -15.8906,-23.8359 ");
  }

  void testDrawingUpRightFromAtom() {
    a2->setCoordinates(QPolygonF() << QPointF(20,-30));
    TS_ASSERT_EQUALS(extractValueFromXml(scene->toSvg(), "//*:polyline/@points/data(.)"), "4.1094,-6.1641 15.8906,-23.8359 ");
  }

  void testDrawingDownLeftFromAtom() {
    a2->setCoordinates(QPolygonF() << QPointF(-20,30));
    TS_ASSERT_EQUALS(extractValueFromXml(scene->toSvg(), "//*:polyline/@points/data(.)"), "-4.1094,6.1641 -15.8906,23.8359 ");
  }

  void testDrawingDownRightFromAtom() {
    a2->setCoordinates(QPolygonF() << QPointF(20,30));
    TS_ASSERT_EQUALS(extractValueFromXml(scene->toSvg(), "//*:polyline/@points/data(.)"), "4.1094,6.1641 15.8906,23.8359 ");
  }
};
