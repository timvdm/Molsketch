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
#include <molscene.h>
#include <molview.h>

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

    arrow->setArrowTipPart(Arrow::UpperForward); // this won't push an undo command and hence not trigger the listener
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
    for (int i = 0 ; i < 20 ; ++i) {
      arrow->setCoordinates(QVector<QPointF>(i));
      popup->connectArrow(arrow);
      TS_ASSERT((i-1) % 3 ? !spline->isEnabled() : spline->isEnabled());
      popup->connectArrow(nullptr);
    }
  }

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

  void testCoordinateTableChangesArrowCoordinates() {
    arrow->setCoordinates({QPointF(0,0), QPointF(5,5)});
    popup->connectArrow(arrow);
    QTest::keyClicks(coordinates, "5.3\t3.4\t1.2\t2.4");
    QTest::keyClick(coordinates, Qt::Key_Enter);
    QVector<QPointF> expectedCoordinates = {QPointF(5.3, 3.4), QPointF(1.2,2.4)};
    TS_ASSERT_EQUALS(arrow->coordinates(), expectedCoordinates);
  }


  void testMoveArrowPointChangesCoordinates() {
    arrow->setCoordinates({QPointF(0,0), QPointF(5,5)});
    MolScene scene;
    MolView view;
    view.setScene(&scene);
    scene.addItem(arrow);
    popup->connectArrow(arrow);

    QTest::mousePress(view, Qt::LeftButton, 0, view.mapFromScene(arrow->mapToScene(arrow->coordinates().first())));
    QTest::mouseMove(view, view.mapFromScene(QPointF(10,10)));
    QVector<QPointF> expectedCoordinates = {QPointF(10, 10), QPointF(5, 5)};
    TS_ASSERT_EQUALS(arrow->coordinates(), expectedCoordinates);
  }

  void testMoveArrowChangesCoordinates() {
    arrow->setCoordinates({QPointF(0,0), QPointF(5,5)});
    MolScene scene;
    MolView view;
    view.setScene(&scene);
    scene.addItem(arrow);
    popup->connectArrow(arrow);

    QTest::mousePress(view, Qt::LeftButton, 0, view.mapFromScene(QPointF(2, 2));
    QTest::mouseMove(view, view.mapFromScene(QPointF(10,10)));
    QVector<QPointF> expectedCoordinates = {QPointF(8, 8), QPointF(13, 13)};
    TS_ASSERT_EQUALS(arrow->coordinates(), expectedCoordinates);
  }
};
