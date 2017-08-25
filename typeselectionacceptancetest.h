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
#include <QGraphicsView>
#include <QToolBar>
#include <QTimer>
#include <molscene.h>
#include <QPushButton>
#include <QCheckBox>
#include <QToolButton>
#include "mainwindow.h"
#include "utilities.h"

class TypeSelectionAcceptanceTest : public CxxTest::TestSuite {
  MainWindow *mainWindow;

  QToolButton* findToolButton(QString toolbarName, QString actionName) {
    QToolBar *toolbar = mainWindow->findChild<QToolBar*>(toolbarName);
    assertNotNull(toolbar, "Need toolbar: " + toolbarName);
    QAction *typeSelectionAction = mainWindow->findChild<QAction*>(actionName);
    assertNotNull(typeSelectionAction, "Need action: " + actionName);
    QWidget *actionWidget = toolbar->widgetForAction(typeSelectionAction);
    assertNotNull(actionWidget, "No widget available for action: " + actionName);
    QToolButton *actionButton = qobject_cast<QToolButton*>(actionWidget);
    assertNotNull(actionButton, "Widget for action '" + actionName + "' is not a button.");
    TSM_ASSERT("Action button not enabled!", actionButton->isEnabled());
    return actionButton;
  }

  void clickToolButton(QString toolbarName, QString actionName) {
    QTest::mouseClick(findToolButton(toolbarName, actionName), Qt::LeftButton);
  }

  void assertToolButtonNotSelected(QString toolbarName, QString actionName) {
    TS_ASSERT(!findToolButton(toolbarName, actionName)->isChecked())
  }

  void clickOnView() {
    QGraphicsView *view = mainWindow->findChild<QGraphicsView*>();
    QTest::mouseClick(view->viewport(), Qt::LeftButton);
  }

  void selectEverythingFromView() {
    QTest::keyClick(mainWindow, Qt::Key_Escape);
    QGraphicsView *view = mainWindow->findChild<QGraphicsView*>();
    QTest::keyClicks(mainWindow, "A", Qt::ControlModifier);
    mainWindow->findChild<Molsketch::MolScene*>()->selectAll();
  }

  class ActivateBondCheckBoxAndConfirmDialog {
    QWidget* findDialog() {
      QWidget* typeDialog = qApp->activeModalWidget();
      assertNotNull(typeDialog, "No type dialog was opened!");
      return typeDialog;
    }

    void clickBondCheckBox(QWidget* typeDialog) {
      QWidget *bondCheckbox = typeDialog->findChild<QWidget*>("bonds");
      assertNotNull(bondCheckbox, "No bond checkbox found!");
      QTest::mouseClick(bondCheckbox, Qt::LeftButton, Qt::NoModifier, QPoint(1,1)); // offset for QCheckBox
    }

    QWidget* findOkButton(QWidget* typeDialog) {
      for(QPushButton *child : findDialog()->findChildren<QPushButton*>())
        if (child->text() == "&OK") return child;
      TSM_ASSERT("Could not find OK button!", false);
      throw nullptr;
    }

  public:
    void operator()() {
      clickBondCheckBox(findDialog());
      QTest::mouseClick(findOkButton(findDialog()), Qt::LeftButton);
    }
  };

  void assertOnlyBondsAreSelected() {
    Molsketch::MolScene *scene = mainWindow->findChild<Molsketch::MolScene*>();
    assertNotNull(scene, "No MolScene found!");
    QList<QGraphicsItem*> selection = scene->selectedItems();
    TSM_ASSERT_EQUALS("Selection should contain only three items for cyclopropane", selection.size(), 3);
    for(QGraphicsItem* selectedItem : selection)
      TSM_ASSERT_EQUALS("Item should be of 'bond' type!", selectedItem->type(), Molsketch::Bond::Type);
  }

public:
  void setUp() {
    mainWindow = new MainWindow;
    mainWindow->show();
  }

  void tearDown() {
    delete mainWindow;
  }

  void testSelection() {
    clickToolButton("drawing-toolbar", "ring-action");
    clickOnView();
    selectEverythingFromView();

    QTimer::singleShot(0, mainWindow, ActivateBondCheckBoxAndConfirmDialog());
    clickToolButton("modify-toolbar", "item-type-selection-action");

    assertOnlyBondsAreSelected();
    assertToolButtonNotSelected("modify-toolbar", "item-type-selection-action");
  }

  void testSelectionFromUnselected() {
    clickToolButton("drawing-toolbar", "ring-action");
    clickOnView();

    QTimer::singleShot(0, mainWindow, ActivateBondCheckBoxAndConfirmDialog());
    clickToolButton("modify-toolbar", "item-type-selection-action");

    assertOnlyBondsAreSelected();
    assertToolButtonNotSelected("modify-toolbar", "item-type-selection-action");
  }

  // TODO test for all actions: icon defined
  // TODO test only types available within the current selection are available
};
