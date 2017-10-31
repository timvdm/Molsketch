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
#include <arrow.h>
using namespace Molsketch;

class ArrowUnitTest : public CxxTest::TestSuite {
  template<class T>
  void testSerializationTour(const Arrow& original, T (Arrow::*getter)() const) {
    QString xml;
    QXmlStreamWriter out(&xml);
    original.writeXml(out);
    QXmlStreamReader in(xml);
    in.readNextStartElement();
    Arrow restored;
    restored.readXml(in);
    TS_ASSERT_EQUALS((restored.*getter)(), (original.*getter)());
  }

public:
  void setUp() {
  }

  void tearDown() {
  }

  void testArrowSplineSurvivesSaving() {
    Arrow original;
    original.setSpline(true);
    testSerializationTour(original, &Arrow::getSpline);
  }

  void testArrowNoSplineSurvivesSaving() {
    Arrow original;
    original.setSpline(false);
    testSerializationTour(original, &Arrow::getSpline);
  }
};
