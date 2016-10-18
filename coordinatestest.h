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
#include <atom.h>

using namespace Molsketch;

class CoordinatesTest : public CxxTest::TestSuite
{
public:
  void testAtomCoordinates() {
    QVector<QPointF> coordinates;
    coordinates << QPointF() << QPointF(10,10) << QPointF(-5,10);
    for (QPointF coordinate : coordinates)
    {
      Atom atom(coordinate, "C");
      QFontMetricsF metrics = QFontMetricsF(QFont());
      QRectF expectedBoundingRect = metrics.boundingRect(atom.element());
      expectedBoundingRect.moveTopLeft(-QPointF(expectedBoundingRect.width()/2, expectedBoundingRect.height()/2));
      TSM_ASSERT_EQUALS("Bounding rectangle", atom.boundingRect(), expectedBoundingRect);
      TSM_ASSERT_EQUALS("Position", atom.pos(), coordinate);
      TSM_ASSERT_EQUALS("Coordinate", atom.coordinates().first(), coordinate);
      TSM_ASSERT_EQUALS("Bounding rectangle in scene coordinates", atom.boundingRect(), expectedBoundingRect.translated(coordinate));
    }
  }
};
