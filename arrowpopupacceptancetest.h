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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#include <cxxtest/TestSuite.h>
#include <QCheckBox>
#include <QTest>
#include <arrow.h>
#include <arrowpopup.h>
#include <coordinatetableview.h>
#include <molscene.h>
#include <molview.h>
#include <commands.h>
#include <coordinatemodel.h>
#include <QLineEdit>
#include "utilities.h"

using namespace Molsketch;

class ArrowPopupAcceptanceTest : public CxxTest::TestSuite
{
  Arrow *arrow;
  ArrowPopup* popup;
  MolScene* scene;

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

    scene = new MolScene();
    scene->addItem(arrow);
  }

  void tearDown() {
    delete arrow;
    delete scene;
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
    QTest::mouseClick(beginningTop, Qt::LeftButton);
    TS_ASSERT_EQUALS(arrow->getArrowType(), Arrow::UpperForward);

    QTest::mouseClick(endTop, Qt::LeftButton);
    TS_ASSERT_EQUALS(arrow->getArrowType(), Arrow::UpperForward | Arrow::UpperBackward);

    QTest::mouseClick(beginningBottom, Qt::LeftButton);
    TS_ASSERT_EQUALS(arrow->getArrowType(), Arrow::UpperForward | Arrow::UpperBackward | Arrow::LowerForward);

    QTest::mouseClick(endBottom, Qt::LeftButton);
    TS_ASSERT_EQUALS(arrow->getArrowType(), Arrow::UpperForward | Arrow::UpperBackward | Arrow::LowerForward | Arrow::LowerBackward);

    QTest::mouseClick(endBottom, Qt::LeftButton);
    TS_ASSERT_EQUALS(arrow->getArrowType(), Arrow::UpperForward | Arrow::UpperBackward | Arrow::LowerForward);

    QTest::mouseClick(beginningBottom, Qt::LeftButton);
    TS_ASSERT_EQUALS(arrow->getArrowType(), Arrow::UpperForward | Arrow::UpperBackward);

    QTest::mouseClick(endTop, Qt::LeftButton);
    TS_ASSERT_EQUALS(arrow->getArrowType(), Arrow::UpperForward);

    QTest::mouseClick(beginningTop, Qt::LeftButton);
    TS_ASSERT_EQUALS(arrow->getArrowType(), Arrow::NoArrow);
  }

  void testTipToCheckBoxLink() {
    popup->connectArrow(arrow);

    TS_ASSERT_EQUALS(spline->checkState(), Qt::Unchecked);
    assertTipCheckBoxes(false, false, false, false);

    scene->stack()->push(new Commands::SetArrowType(arrow, Arrow::UpperForward));
    assertTipCheckBoxes(true, false, false, false);

    scene->stack()->push(new Commands::SetArrowType(arrow, Arrow::UpperForward | Arrow::LowerForward));
    assertTipCheckBoxes(true, true, false, false);

    scene->stack()->push(new Commands::SetArrowType(arrow, Arrow::UpperBackward));
    arrow->setArrowTipPart(Arrow::UpperBackward);
    assertTipCheckBoxes(false, false, true, false);

    scene->stack()->push(new Commands::SetArrowType(arrow, Arrow::UpperBackward | Arrow::LowerBackward));
    assertTipCheckBoxes(false, false, true, true);

    scene->stack()->push(new Commands::SetArrowType(arrow, Arrow::NoArrow));
    assertTipCheckBoxes(false, false, false, false);
  }

  void testEnabledStates() {
    TS_ASSERT_EQUALS(popup->isEnabled(), false);
    popup->connectArrow(arrow);
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

    scene->stack()->push(new Commands::setArrowSplineCommand(arrow, false));
    TS_ASSERT_EQUALS(spline->checkState(), Qt::Unchecked);

    scene->stack()->push(new Commands::setArrowSplineCommand(arrow, true));
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
    popup->connectArrow(arrow);
    TS_ASSERT_EQUALS(coordinates->model()->getCoordinates(), testCoordinates);
  }

  void testCoordinateTableChangesArrowCoordinates() {
    arrow->setCoordinates({QPointF(0,0), QPointF(5,5)});
    popup->connectArrow(arrow);
    enterDataIntoCell(coordinates, "5.3", 0,0);
    enterDataIntoCell(coordinates, "3.4", 0,1);
    enterDataIntoCell(coordinates, "1.2", 1,0);
    enterDataIntoCell(coordinates, "2.4", 1,1);
    QVector<QPointF> expectedCoordinates = {QPointF(5.3, 3.4), QPointF(1.2,2.4)};
    QS_ASSERT_EQUALS(coordinates->model()->getCoordinates(), expectedCoordinates);
    QS_ASSERT_EQUALS(arrow->coordinates(), expectedCoordinates);
  }


  void testMoveArrowPointChangesCoordinates() {
    arrow->setCoordinates({QPointF(0,0), QPointF(5,5)});
    MolScene scene;
    MolView view(&scene);
    scene.addItem(arrow);
    popup->connectArrow(arrow);

    view.show();
    QTest::mousePress(view.viewport(), Qt::LeftButton, Qt::NoModifier, view.mapFromScene(QPointF(0,0)));
    mouseMoveEvent(view.viewport(), Qt::LeftButton, Qt::NoModifier, view.mapFromScene(QPointF(10,10)));

    QVector<QPointF> expectedCoordinates = {QPointF(10, 10), QPointF(5, 5)};
    QS_ASSERT_EQUALS(arrow->coordinates(), expectedCoordinates);
    QS_ASSERT_EQUALS(coordinates->model()->getCoordinates(), expectedCoordinates);
    scene.removeItem(arrow);
  }

  void testMoveArrowChangesCoordinates() {
    arrow->setCoordinates({QPointF(0,0), QPointF(10,10)});
    MolScene scene;
    MolView view(&scene);
    scene.addItem(arrow);
    popup->connectArrow(arrow);

    QTest::mousePress(view.viewport(), Qt::LeftButton, Qt::NoModifier, view.mapFromScene(QPointF(5, 5)));
    mouseMoveEvent(view.viewport(), Qt::LeftButton, Qt::NoModifier, view.mapFromScene(QPointF(10,20)));
    QVector<QPointF> expectedCoordinates = {QPointF(5, 15), QPointF(15, 25)};
    QS_ASSERT_EQUALS(arrow->coordinates(), expectedCoordinates);
    QS_ASSERT_EQUALS(coordinates->model()->getCoordinates(), expectedCoordinates);
    scene.removeItem(arrow);
  }

  // TODO test deletion of arrow from scene
};
