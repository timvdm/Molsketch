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
#include <QString>
#include <programversion.h>

class ProgramVersionUnitTest : public CxxTest::TestSuite {
private:
  void assertVersion(const QString& input, int major, int minor, int patch, int build) {
    ProgramVersion version(input);
    TS_ASSERT_EQUALS(major, version.version());
    TS_ASSERT_EQUALS(minor, version.subversion());
    TS_ASSERT_EQUALS(patch, version.patch());
    TS_ASSERT_EQUALS(build, version.build());
  }

  void assertEqual(const ProgramVersion& a, const ProgramVersion& b) {
    TSM_ASSERT(("a=" + a.toString() + " b=" + b.toString()),a == b);
    TSM_ASSERT(("a=" + a.toString() + " b=" + b.toString()),a <= b);
    TSM_ASSERT(("a=" + a.toString() + " b=" + b.toString()),a >= b);
    TSM_ASSERT(("a=" + a.toString() + " b=" + b.toString()),!(a != b));
    TSM_ASSERT(("a=" + a.toString() + " b=" + b.toString()),!(b < a));
    TSM_ASSERT(("a=" + a.toString() + " b=" + b.toString()),!(a > b));
  }

  void assertFirstLarger(const ProgramVersion& a, const ProgramVersion& b) {
    TSM_ASSERT(("a=" + a.toString() + " b=" + b.toString()),b < a);
    TSM_ASSERT(("a=" + a.toString() + " b=" + b.toString()),!(a < b));

    TSM_ASSERT(("a=" + a.toString() + " b=" + b.toString()),a > b);
    TSM_ASSERT(("a=" + a.toString() + " b=" + b.toString()),!(b > a));

    TSM_ASSERT(("a=" + a.toString() + " b=" + b.toString()),a >= b);
    TSM_ASSERT(("a=" + a.toString() + " b=" + b.toString()),!(b >= a));

    TSM_ASSERT(("a=" + a.toString() + " b=" + b.toString()),b <= a);
    TSM_ASSERT(("a=" + a.toString() + " b=" + b.toString()),!(a <= b));

    TSM_ASSERT(("a=" + a.toString() + " b=" + b.toString()),a != b);
    TSM_ASSERT(("a=" + a.toString() + " b=" + b.toString()),!(a == b));
  }

public:
  void testReading() {
    assertVersion("", 0, 0, 0, 0);
    assertVersion("5", 5, 0, 0, 0);
    assertVersion("3.2", 3, 2, 0, 0);
    assertVersion("4.9.2", 4, 9, 2, 0);
    assertVersion("7.5.3.8", 7, 5, 3, 8);
    assertVersion("7.5.3.8.5", 7, 5, 3, 8);
    assertVersion("17.35.53.28.1235", 17, 35, 53, 28);
  }

  void testComparison() {
    assertEqual(ProgramVersion(), ProgramVersion());
    assertEqual(ProgramVersion(1,3), ProgramVersion(1,3,0));
    assertEqual(ProgramVersion(1,3), ProgramVersion(1,3,0,0));
    assertFirstLarger(ProgramVersion(1), ProgramVersion());
    assertFirstLarger(ProgramVersion(1,3), ProgramVersion(1));
    assertFirstLarger(ProgramVersion(1,3), ProgramVersion(1,2));
    assertFirstLarger(ProgramVersion(1,3,4,5), ProgramVersion(1,2));
    assertFirstLarger(ProgramVersion(1,3,4,5), ProgramVersion(1,3));
    assertFirstLarger(ProgramVersion(1,3,4,5), ProgramVersion(1,3,4));
    assertFirstLarger(ProgramVersion(10,3,4,5), ProgramVersion(9,3,4));
    assertFirstLarger(ProgramVersion(1,3,10,5), ProgramVersion(1,3,9));
  }
};
