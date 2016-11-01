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
#include <QCheckBox>
#include <QTest>
#include <arrow.h>
#include <arrowpopup.h>

using namespace Molsketch;

class ArrowPopupAcceptanceTest : public CxxTest::TestSuite
{
public:
  void setUp() {

  }

  void tearDown() {

  }

  void testGuiTestCase() {
    Arrow arrow;
    ArrowPopup popup;
    popup.connectArrow(&arrow);
    QCheckBox *box = popup.findChild<QCheckBox*>("beginningTopTip");

    TS_ASSERT_EQUALS(arrow.getProperties().arrowType,
                     Arrow::LowerBackward |
                     Arrow::UpperBackward);

    QTest::mouseClick(box, Qt::LeftButton);

    TS_ASSERT_EQUALS(arrow.getProperties().arrowType,
                     Arrow::UpperForward |
                     Arrow::LowerBackward |
                     Arrow::UpperBackward);
  }

  // Situation when no arrow connected
  // connect null arrrow
  // destructor?
  // check GUI elements reflect arrow:
  //   - forward upper
  //   - forward lower
  //   - backward upper
  //   - backward lower

  //   - coordinates
  //   - spline

  //   - initial connect
  //   - when arrow is changed

  // check GUI elements control arrow:
  //   - forward upper
  //   - forward lower
  //   - backward upper
  //   - backward lower
  //   - coordinates
  //   - spline

  // check availability of spline

};
