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
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <cxxtest/TestSuite.h>
#include <QDoubleSpinBox>
#include <molscene.h>
#include <scenepropertieswidget.h>
#include <settingsitem.h>
#include <settingsfacade.h>
#include <settingsconnector.h>
#include "utilities.h"

using namespace Molsketch;

const qreal BOND_LINE_WIDTH = 5.5;
const qreal ALTERNATE_LINE_WIDTH = 11.75;

class SceneSettingsForTesting : public SceneSettings {
public:
  QList<const SettingsItem*> getAllSettingsItems() {
    QList<const SettingsItem*> result;
    for (auto child : children())
      if (auto settingsItem = dynamic_cast<const SettingsItem*>(child))
        result << settingsItem;
    return result;
  }
  SceneSettingsForTesting() : SceneSettings(SettingsFacade::transientSettings()) {}
  int connections(const char* signal) {
    return receivers(signal);
  }
};

class ScenePropertiesWidgetHeadlessTest : public CxxTest::TestSuite {
  MolScene *scene;
  ScenePropertiesWidget *propertiesWidget;
  QDoubleSpinBox *bondLineWidthControl;
  DoubleSettingsItem *bondLineWidthSetting;
  SceneSettingsForTesting *settings;

  QList<QMetaMethod> getAllMetaMethods(const QMetaObject* object, const QMetaMethod::MethodType& type) {
    QList<QMetaMethod> result;
    for (int i = object->methodOffset(); i < object->methodCount(); ++i) {
      auto method = object->method(i);
      if (type == method.methodType())
        result << method;
    }
    return result;
  }

  QList<const SettingsConnector*> findConnectors(const QObject *object) {
    QList<const SettingsConnector*> connectors;
    auto allConnectors = propertiesWidget->findChildren<SettingsConnector*>();
    std::copy_if(allConnectors.begin(), allConnectors.end(), std::back_inserter(connectors),
                 [&](const SettingsConnector* connector) { return object->disconnect(connector);});
    return connectors;
  }

public:
  void setUp() override {
    settings = new SceneSettingsForTesting;
    scene = new MolScene(settings);
    propertiesWidget = scene->producePropertiesWidget();
    bondLineWidthControl = assertNotNull<QDoubleSpinBox>(propertiesWidget->findChild<QDoubleSpinBox*>("bondLineWidth"));
    bondLineWidthSetting = assertNotNull<DoubleSettingsItem>(scene->settings()->bondWidth());
  }

  void tearDown() override {
    delete propertiesWidget;
    delete scene;
  }

  void testSettingAndUiSyncedInitially() {
    TS_ASSERT_EQUALS(bondLineWidthControl->value(), bondLineWidthSetting->get());
  }

  void testSettingPropertyThroughControl() {
    bondLineWidthControl->setValue(BOND_LINE_WIDTH);
    TS_ASSERT_EQUALS(bondLineWidthSetting->get(), BOND_LINE_WIDTH);
    TS_ASSERT_EQUALS(bondLineWidthControl->value(), BOND_LINE_WIDTH);
    TS_ASSERT_EQUALS(scene->stack()->index(), 1);
    TS_ASSERT_EQUALS(scene->stack()->count(), 1);
  }

  void testSettingPropertyThroughControlMultipleTimes() {
    qreal originalValue = bondLineWidthSetting->get();
    TS_ASSERT_DIFFERS(originalValue, BOND_LINE_WIDTH);
    bondLineWidthControl->setValue(BOND_LINE_WIDTH);
    bondLineWidthControl->setValue(ALTERNATE_LINE_WIDTH);
    TS_ASSERT_EQUALS(bondLineWidthSetting->get(), ALTERNATE_LINE_WIDTH);
    TS_ASSERT_EQUALS(bondLineWidthControl->value(), ALTERNATE_LINE_WIDTH);
    TS_ASSERT_EQUALS(scene->stack()->index(), 1);
    TS_ASSERT_EQUALS(scene->stack()->count(), 1);
    scene->stack()->undo();
    TS_ASSERT_EQUALS(bondLineWidthSetting->get(), originalValue);
    TS_ASSERT_EQUALS(bondLineWidthControl->value(), originalValue);
  }

  void testSettingPropertyThroughSettings() {
    bondLineWidthSetting->set(BOND_LINE_WIDTH);
    TS_ASSERT_EQUALS(bondLineWidthSetting->get(), BOND_LINE_WIDTH);
    TS_ASSERT_EQUALS(bondLineWidthControl->value(), BOND_LINE_WIDTH);
    TS_ASSERT_EQUALS(scene->stack()->index(), 0);
    TS_ASSERT_EQUALS(scene->stack()->count(), 0);
  }

  void testUndoSettingsProperty() {
    bondLineWidthSetting->set(BOND_LINE_WIDTH);
    bondLineWidthControl->setValue(ALTERNATE_LINE_WIDTH);
    TS_ASSERT_EQUALS(bondLineWidthSetting->get(), ALTERNATE_LINE_WIDTH);
    TS_ASSERT(scene->stack()->canUndo());
    scene->stack()->undo();
    TS_ASSERT_EQUALS(bondLineWidthSetting->get(), BOND_LINE_WIDTH);
    TS_ASSERT_EQUALS(bondLineWidthControl->value(), BOND_LINE_WIDTH);
    TS_ASSERT_EQUALS(scene->stack()->index(), 0);
    TS_ASSERT_EQUALS(scene->stack()->count(), 1);
  }

  void testAllSettingsAreConnectedToAConnector() {
    for (auto SettingsItem : settings->getAllSettingsItems()) {
      auto connectors = findConnectors(SettingsItem);
      QSO_ASSERT(connectors.size() == 1, "Expected to find exactly one connector for ", SettingsItem, ". Got: ", connectors);
    }
  }

  void testAllUiElementsAreConnectedToAtMostOneConnector() {
    for (auto widget : propertiesWidget->findChildren<QWidget*>()) {
      auto connectors = findConnectors(widget);
      QSO_ASSERT(connectors.size() <= 1, "Expecting to find at most one connector for ", widget, ". Got: ", connectors);
    }
  }
};
