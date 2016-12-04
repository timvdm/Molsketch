/***************************************************************************
 *   Copyright (C) 2016 by Hendrik Vennekate                               *
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
#include <QApplication>
#include <QFontMetricsF>
#include <QRectF>
#include <QSet>
#include <atom.h>
#include <bond.h>
#include <molecule.h>

using namespace Molsketch;

class CoordinatesTest : public CxxTest::TestSuite
{
public:
  void testAtomCoordinates() {
    QVector<QPointF> coordinates;
    coordinates << QPointF() << QPointF(10,10) << QPointF(-5,10); // TODO atoms in molecules
    for (QPointF coordinate : coordinates)
    {
      Atom atom(coordinate, "C");
      QFontMetricsF metrics = QFontMetricsF(QFont());
      QRectF expectedBoundingRect = metrics.boundingRect(atom.element());
      expectedBoundingRect.moveTopLeft(-QPointF(expectedBoundingRect.width()/2, expectedBoundingRect.height()/2));
      TSM_ASSERT_EQUALS("Bounding rectangle", atom.boundingRect(), expectedBoundingRect);
      TSM_ASSERT_EQUALS("Position", atom.pos(), coordinate);
      TSM_ASSERT_EQUALS("Coordinate", atom.coordinates().first(), coordinate);
      TSM_ASSERT_EQUALS("Bounding rect", atom.mapRectToScene(atom.boundingRect()), expectedBoundingRect.translated(coordinate));
    }
  }

  void testMoleculeBoundingBox() {
//    QList<QPointF> coordinates;
//    coordinates << QPointF(0,0) << QPointF(100,0) << QPointF(0,100) << QPointF(100,100);
//    QList<Atom*> atoms = coordinatesToAtoms(coordinates, "H");
//    QSet<Bond*> bonds = allConnections(atoms);
//    Molecule molecule(atoms.toSet(), bonds); // TODO this seems to crash

//    QRectF boundingBox;
//    for(Atom* atom : atoms) boundingBox |= atom->boundingRect().translated(atom->pos());
//    for(Bond* bond : bonds) boundingBox |= bond->boundingRect().translated(bond->pos());

//    TSM_ASSERT_EQUALS("Molecule bounding box", molecule.boundingRect(), boundingBox);
    // From old test:
    // TODO check prepareGeometryChange() in items, molscene rendering refresh mode
    // TODO seems to be a blinker
    //FAIL!  : coordinatesTest::moleculeBoundingBox(square) Compared values are not the same
    //   Actual   (molecule->boundingRect()): QRectF(-5,-9 110x132) (bottomright 105,123)
    //   Expected (boundingBox)             : QRectF(-5,-9 110x118) (bottomright 105,109)
    //   Loc: [../../../../Programme/Molsketch/tests/coordinatestest.cpp(103)]
  }

  QList<Atom*> coordinatesToAtoms(QList<QPointF> coordinates, const QString& element)
  {
    QList<Atom*> atoms;
    foreach(QPointF coordinate, coordinates) atoms << new Atom(coordinate, element);
    return atoms;
  }

  QSet<Bond*> allConnections(const QList<Atom*> atoms) {
    QSet<Bond*> result;
    for (int i = 0 ; i < atoms.size(); ++i)
      for (int j = i+1 ; j < atoms.size() ; ++j)
        result << new Bond(atoms[i], atoms[j]);
    return result;
  }
};
