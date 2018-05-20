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
#include <settingsitem.h>
#include <settingsfacade.h>
#include <molscene.h>
#include "utilities.h"

using namespace Molsketch;

const QString KEY("TestKey");
const QString ALTERNATIVE_KEY("OtherTestKey");
const qreal ORIGINAL_VALUE = 5.5;
const qreal NEW_VALUE = 10.25;
const QString DESCRIPTION("Change setting");

class SettingsItemCommandUnitTest : public CxxTest::TestSuite {
  DoubleSettingsItem *testItem, *secondTestItem;
  SettingsFacade *facade;
  MolScene *scene;
public:
  void setUp() {
    scene = new MolScene;
    facade = SettingsFacade::transientSettings(scene);
    testItem = new DoubleSettingsItem(KEY, facade,scene);
    secondTestItem = new DoubleSettingsItem(KEY, facade,scene);

    testItem->set(ORIGINAL_VALUE);
  }

  void tearDown() {
    delete scene;
  }

  void testRedoing() {
    (new SettingsItemUndoCommand(testItem, NEW_VALUE, DESCRIPTION, scene->stack()))->execute();
    TS_ASSERT_EQUALS(testItem->get(), NEW_VALUE);
    TS_ASSERT_EQUALS(scene->stack()->index(), 1);
    QS_ASSERT_EQUALS(scene->stack()->undoText(), DESCRIPTION);
  }

  void testUndoing() {
    (new SettingsItemUndoCommand(testItem, NEW_VALUE, DESCRIPTION, scene->stack()))->execute();
    scene->stack()->undo();
    TS_ASSERT_EQUALS(testItem->get(), ORIGINAL_VALUE);
    QS_ASSERT_EQUALS(scene->stack()->redoText(), DESCRIPTION);
  }

  void testMerging() {
    (new SettingsItemUndoCommand(testItem, NEW_VALUE, DESCRIPTION, scene->stack()))->execute();
    (new SettingsItemUndoCommand(testItem, ORIGINAL_VALUE, DESCRIPTION, scene->stack()))->execute();

    TS_ASSERT_EQUALS(scene->stack()->index(), 1);
    QS_ASSERT_EQUALS(scene->stack()->undoText(), DESCRIPTION);
    TS_ASSERT_EQUALS(testItem->get(), ORIGINAL_VALUE);

    scene->stack()->undo();
    TS_ASSERT_EQUALS(scene->stack()->index(), 0);
    TS_ASSERT_EQUALS(testItem->get(), ORIGINAL_VALUE);
  }

  void testNotMerging() {
    (new SettingsItemUndoCommand(testItem, NEW_VALUE, DESCRIPTION, scene->stack()))->execute();
    (new SettingsItemUndoCommand(secondTestItem, NEW_VALUE, DESCRIPTION, scene->stack()))->execute();

    TS_ASSERT_EQUALS(scene->stack()->index(), 2);
    TS_ASSERT_EQUALS(testItem->get(), NEW_VALUE);
    TS_ASSERT_EQUALS(secondTestItem->get(), NEW_VALUE);
  }

  void testRedoingWithoutScene() {
    (new SettingsItemUndoCommand(testItem, NEW_VALUE))->execute();
    TS_ASSERT_EQUALS(testItem->get(), NEW_VALUE);
  }
};
