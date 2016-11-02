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
#include <coordinatetableview.h>

using namespace Molsketch;

class ArrowPopupAcceptanceTest : public CxxTest::TestSuite
{
  Arrow *arrow;
  ArrowPopup* popup;

  QCheckBox *beginningTop, *beginningBottom, *endTop, *endBottom, *spline;
  CoordinateTableView *coordinates;

public:
  void setUp() {
    arrow = new Arrow();
    arrow->setSpline(false);
    arrow->setArrowType(Arrow::NoArrow);

    popup = new ArrowPopup;
    beginningTop = popup->findChild<QCheckBox*>("beginningTopTip");
    beginningBottom = popup->findChild<QCheckBox*>("beginningBottomTip");
    endTop = popup->findChild<QCheckBox*>("endTopTip");
    endBottom = popup->findChild<QCheckBox*>("endBottomTip");
    spline = popup->findChild<QCheckBox*>("curved");
    coordinates = popup->findChild<CoordinateTableView*>();
  }

  void tearDown() {
    delete arrow;
    delete popup;
  }

  void testGuiElementsFound() {
    TS_ASSERT(beginningTop);
    TS_ASSERT(beginningBottom);
    TS_ASSERT(endTop);
    TS_ASSERT(endBottom);
    TS_ASSERT(spline);
    TS_ASSERT(coordinates);
  }

  void testGuiTestCase() {
    popup->connectArrow(arrow);

    TS_ASSERT_EQUALS(arrow->getProperties().arrowType, Arrow::NoArrow);

    QTest::mouseClick(beginningTop, Qt::LeftButton);

    TS_ASSERT_EQUALS(arrow->getProperties().arrowType, Arrow::UpperForward);
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
