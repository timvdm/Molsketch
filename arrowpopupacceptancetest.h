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

  void assertTipCheckBoxes(bool beginningTopChecked,
                           bool beginningBottomChecked,
                           bool endTopChecked,
                           bool endBottomChecked) {
    TS_ASSERT_EQUALS(beginningTop->checkState(), beginningTopChecked ? Qt::Checked : Qt::Unchecked);
    TS_ASSERT_EQUALS(beginningBottom->checkState(), beginningBottomChecked ? Qt::Checked : Qt::Unchecked);
    TS_ASSERT_EQUALS(endTop->checkState(), endTopChecked ? Qt::Checked : Qt::Unchecked);
    TS_ASSERT_EQUALS(endBottom->checkState(), endBottomChecked ? Qt::Checked : Qt::Unchecked);
  }

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

  void testCheckBoxToTipLink() {
    popup->connectArrow(arrow);
    QTest::mouseClick(beginningTop);
    TS_ASSERT_EQUALS(arrow->type(), Arrow::UpperForward);

    QTest::MouseClick(endTop);
    TS_ASSERT_EQUALS(arrow->type(), Arrow::UpperForward | Arrow::UpperBackward);

    QTest::MouseClick(beginningBottom);
    TS_ASSERT_EQUALS(arrow->type(), Arrow::UpperForward | Arrow::UpperBackward | Arrow::LowerForward);

    QTest::MouseClick(endBottom);
    TS_ASSERT_EQUALS(arrow->type(), Arrow::UpperForward | Arrow::UpperBackward | Arrow::LowerForward | Arrow::LowerBackward);

    QTest::MouseClick(endBottom);
    TS_ASSERT_EQUALS(arrow->type(), Arrow::UpperForward | Arrow::UpperBackward | Arrow::LowerForward);

    QTest::MouseClick(beginningBottom);
    TS_ASSERT_EQUALS(arrow->type(), Arrow::UpperForward | Arrow::UpperBackward);

    QTest::MouseClick(endTop);
    TS_ASSERT_EQUALS(arrow->type(), Arrow::UpperForward);

    QTest::mouseClick(beginningTop);
    TS_ASSERT_EQUALS(arrow->type(), Arrow::NoArrow);
  }

  void testTipToCheckBoxLink() {
    popup->connect(arrow);
    TS_ASSERT_EQUALS(spline->checkState(), Qt::Unchecked);
    assertTipCheckBoxes(false, false, false, false);

    arrow->setArrowTipPart(Arrow::UpperForward);
    assertTipCheckBoxes(true, false, false, false);

    arrow->setArrowTipPart(Arrow::LowerForward);
    assertTipCheckBoxes(true, true, false, false);

    arrow->setArrowTipPart(Arrow::UpperBackward);
    assertTipCheckBoxes(true, true, true, false);

    arrow->setArrowTipPart(Arrow::LowerBackward);
    assertTipCheckBoxes(true, true, true, true);

    arrow->setArrowTipPart(Arrow::NoArrow);
    assertTipCheckBoxes(false, false, false, false);
  }

  void testEnabledStates() {
    TS_ASSERT_EQUALS(popup->isEnabled(), false);
    popup->connect(arrow);
    TS_ASSERT_EQUALS(popup->isEnabled(), true);
    popup->connectArrow(nullptr);
    TS_ASSERT_EQUALS(popup->isEnabled(), false);
  }

  void testArrowTipCheckboxesInitialized() {
    arrow->setArrowType(Arrow::UpperForward | Arrow::LowerBackward);
    popup->connectArrow(arrow);
    assertTipCheckBoxes(true, false, false, true);
  }

  void testSplineReflectsArrow() {
    arrow->setSpline(true);
    popup->connectArrow(arrow);
    TS_ASSERT_EQUALS(spline->checkState(), Qt::Checked);

    arrow->setSpline(false);
    TS_ASSERT_EQUALS(spline->checkState(), Qt::Unchecked);

    arrow->setSpline(true);
    TS_ASSERT_EQUALS(spline->checkState(), Qt::Checked);
  }

  void testSplineCanBeChecked() {
// get fitting number of coordinates and verify that checking leads to spline enabled
  }

  // check that only valid coordinate counts enable spline checkbox
  // check that coordinates can be changed in table and change arrow
  // check that if arrow point is moved, table is updated while moving
  // check that if entire arrow is moved, table is updated while moving

  void testCoordinatesUponConnection() {
    QPolygonF testCoordinates;
    testCoordinates
        << QPointF(1.5, 3.5)
        << QPointF(2.5, 4.5)
        << QPointF(3.5, 1.53);
    arrow->setCoordinates(testCoordinates);
    popup->connect(arrow);
    TS_ASSERT_EQUALS(coordinates->model()->getCoordinates(), testCoordinates);
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
  //   + forward upper
  //   + forward lower
  //   + backward upper
  //   + backward lower

  //   + coordinates
  //   + spline

  //   - initial connect
  //   - when arrow is changed

  // check GUI elements control arrow:
  //   + forward upper
  //   + forward lower
  //   + backward upper
  //   + backward lower
  //   - coordinates
  //   - spline

  // check availability of spline

};
