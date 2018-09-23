/***************************************************************************
 *   Copyright (C) 2018 by Hendrik Vennekate                               *
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
#include "utilities.h"
#include <sumformula.h>

using namespace Molsketch;

class SumFormulaUnitTest : public CxxTest::TestSuite {
public:
  void testCreation() {
    QS_ASSERT_EQUALS(SumFormula{"A"}, QString("A"));

    auto a2 = SumFormula{"A", 2};
    QS_ASSERT_EQUALS(a2, QString("A2"));

    auto ach = SumFormula{{"A"}, {"C"}, {"H"}};
    QS_ASSERT_EQUALS(ach, QString("CHA"));

    auto aa = SumFormula{{"A"}, {"H"}, {"A"}};
    QS_ASSERT_EQUALS(aa, QString("HA2"));
  }

  void testFormatting() {
    QS_ASSERT_EQUALS(SumFormula{"A"}.toHtml(), QString("A"));

    auto a2 = SumFormula{"A", 20};
    QS_ASSERT_EQUALS(a2.toHtml(), QString("A<sub>20</sub>"));

    auto ach = SumFormula{{"A"}, {"C"}, {"H"}};
    QS_ASSERT_EQUALS(ach.toHtml(), QString("CHA"));

    auto aa = SumFormula{{"A"}, {"H"}, {"A"}};
    QS_ASSERT_EQUALS(aa.toHtml(), QString("HA<sub>2</sub>"));
  }

  void testAdding() {
  }

  void testFromString() {
  }
};
