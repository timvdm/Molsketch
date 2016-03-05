/***************************************************************************
 *   Copyright (C) 2015 Hendrik Vennekate                                  *
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
#include "coordinatestest.h"

#include <QFontMetricsF>

#include "atom.h"
#include "molecule.h"
#include "bond.h"
#include "arrow.h"
//#include "frame.h"

using namespace Molsketch;

void coordinatesTest::initTestCase()
{

}

void coordinatesTest::cleanupTestCase()
{

}

void coordinatesTest::init()
{

}

void coordinatesTest::cleanup()
{

}

void coordinatesTest::atomCoordinates()
{
  QFETCH(QPointF, ownCoordinate);
  QFETCH(QString, atomicSymbol);
  Atom atom(ownCoordinate, atomicSymbol);
  auto metrics = QFontMetrics(QFont());
  QRectF expectedBoundingRect = metrics.boundingRect(atom.element());
  expectedBoundingRect.moveTopLeft(- QPointF(expectedBoundingRect.width()/2, expectedBoundingRect.height()/2));
  QCOMPARE(atom.boundingRect(), expectedBoundingRect);
  expectedBoundingRect.translate(atom.coordinates().first());
  QCOMPARE(atom.pos(), ownCoordinate);
  QCOMPARE(atom.coordinates().first(), ownCoordinate);
  QCOMPARE(atom.mapRectToScene(atom.boundingRect()), expectedBoundingRect);
}

void coordinatesTest::atomCoordinates_data()
{
  QTest::addColumn<QPointF>("ownCoordinate");
  QTest::addColumn<QString>("atomicSymbol");

  QTest::newRow("origin") << QPointF(0,0) << "C";
  QTest::newRow("10,10") << QPointF(10,10) << "C";
  QTest::newRow("-5,10") << QPointF(-5,10) << "C";
  // TODO atoms in molecules
}

void coordinatesTest::moleculeBoundingBox()
{
  QFETCH(QVector<QPointF>, atomCoordinates);
  QFETCH(QStringList, atomSymbols);

  QSet<Atom*> atoms;
  for (int i = 0 ; i < qMin(atomCoordinates.size(), atomSymbols.size()) ; ++i)
    atoms << new Atom(atomCoordinates[i], atomSymbols[i]);
  QSet<Bond*> bonds;
  foreach(Atom* a, atoms)
  {
    foreach(Atom* b, atoms)
    {
      if (a->neighbours().contains(b)) continue;
      bonds << new Bond(a,b);
    }
  }
  Molecule *molecule = new Molecule(atoms, bonds);

  QRectF boundingBox;
  foreach(Atom* a, atoms)
    boundingBox |= a->boundingRect().translated(a->pos());

  foreach(Bond* b, bonds)
    boundingBox |= b->boundingRect().translated(b->pos());

  QCOMPARE(molecule->boundingRect(), boundingBox);
}
// TODO check prepareGeometryChange() in items, molscene rendering refresh mode
// TODO seems to be a blinker
//FAIL!  : coordinatesTest::moleculeBoundingBox(square) Compared values are not the same
//   Actual   (molecule->boundingRect()): QRectF(-5,-9 110x132) (bottomright 105,123)
//   Expected (boundingBox)             : QRectF(-5,-9 110x118) (bottomright 105,109)
//   Loc: [../../../../Programme/Molsketch/tests/coordinatestest.cpp(103)]


void coordinatesTest::moleculeBoundingBox_data()
{
  QTest::addColumn<QVector<QPointF> >("atomCoordinates");
  QTest::addColumn<QStringList>("atomSymbols");

  QTest::newRow("square") << (QPolygonF()
                              << QPointF(0,0) << QPointF(100,0) << QPointF(0,100) << QPointF(100,100))
                          << (QStringList() << "H" << "H" << "H" << "H") ;
}


