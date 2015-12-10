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
}
