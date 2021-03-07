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
#include <QSettings>
#include <settingsfacade.h>
#include "tempfileprovider.h"
#include "utilities.h"

using namespace Molsketch;

const QString SETTINGS_KEY("TestKey");
const QVariant SETTINGS_VALUE(QString("TestValue"));
const QVariant SETTINGS_DEFAULT(QString("TestDefault"));
const QString SETTINGS_FILE_CONTENT("[General]\n" + SETTINGS_KEY + "=" + SETTINGS_VALUE.toString() + "\n");
const QString SETTINGS_FILE_EMPTY("");
const QString LEGACY_CONFIG_FILE("[General]\n"
                                 "atom-symbol-font=@Variant(\\0\\0\\0@\\0\\0\\0\\x12\\0N\\0o\\0t\\0o\\0 \\0S\\0\\x61\\0n\\0s@ \\0\\0\\0\\0\\0\\0\\xff\\xff\\xff\\xff\\x5\\x1\\0\\x32\\x10)\n"
                                 "electronSystems-visible=false\n"
                                 "latestReleaseNotes=0.5.0.0\n"
                                 "toolBarIconStyle=0\n");
const QString UPDATED_CONFIG_FILE("[General]\n"
                                  "atom-font=@Variant(\\0\\0\\0@\\0\\0\\0\\x12\\0N\\0o\\0t\\0o\\0 \\0S\\0\\x61\\0n\\0s@ \\0\\0\\0\\0\\0\\0\\xff\\xff\\xff\\xff\\x5\\x1\\0\\x32\\x10)\n"
                                  "electron-systems-visible=false\n"
                                  "latest-release-notes=0.5.0.0\n"
                                  "tool-bar-icon-style=0\n");

class SettingsFacadeUnitTest : public TempFileProvider, public CxxTest::TestSuite {
  SettingsFacade *facade, *otherFacade;
  QFile *settingsFile;

  void prepareDefaultFacade() {
    facade = SettingsFacade::transientSettings();
    facade->setValue(SETTINGS_KEY, SETTINGS_VALUE);
  }

  void prepareEmptyPersistedFacade()
  {
    settingsFile = createTemporaryFile();
    settingsFile->open(QFile::ReadOnly);
    facade = SettingsFacade::persistedSettings(new QSettings(settingsFile->fileName(), QSettings::IniFormat));
  }

  void prepareDefaultPersistedFacade() {
    prepareEmptyPersistedFacade();
    facade->setValue(SETTINGS_KEY, SETTINGS_VALUE);
  }

  QString settingsFileContent()
  {
    delete facade;
    facade = nullptr;
    QFile inputFile(settingsFile->fileName());
    inputFile.open(QFile::ReadOnly);
    QString actualFileContent(inputFile.readAll());
    return actualFileContent;
  }

  void preparePersistedSettingsWithSettingsFile(const QString& content) {
    settingsFile = createTemporaryFile();
    settingsFile->open(QFile::WriteOnly);
    settingsFile->write(content.toUtf8());
    settingsFile->close();
    settingsFile->open(QFile::ReadOnly);

    facade = SettingsFacade::persistedSettings(new QSettings(settingsFile->fileName(), QSettings::IniFormat));
  }

public:
  void setUp() {
    otherFacade = (facade = nullptr);
  }

  void tearDown() {
    delete facade;
    delete otherFacade;
  }

  void testConvertingLegacySettingsFile() {
    preparePersistedSettingsWithSettingsFile(LEGACY_CONFIG_FILE);
    delete facade;
    facade = nullptr;
    QS_ASSERT_EQUALS(settingsFileContent(), UPDATED_CONFIG_FILE);
  }

  void testStoringAndRetrievingValuesTransiently() {
    prepareDefaultFacade();
    QS_ASSERT_EQUALS(facade->value(SETTINGS_KEY), SETTINGS_VALUE);
  }

  void testStoringAndRetrievingValuesPersisted(){
    prepareDefaultPersistedFacade();
    QS_ASSERT_EQUALS(facade->value(SETTINGS_KEY), SETTINGS_VALUE);
    QS_ASSERT_EQUALS(settingsFileContent(), SETTINGS_FILE_CONTENT);
  }

  void testRetrievingDefaultValueTransient() {
    facade = SettingsFacade::transientSettings();
    QS_ASSERT_EQUALS(facade->value(SETTINGS_KEY, SETTINGS_DEFAULT), SETTINGS_DEFAULT);
  }


  void testRetrievingDefaultValuePersistent() {
    prepareEmptyPersistedFacade();
    QS_ASSERT_EQUALS(facade->value(SETTINGS_KEY, SETTINGS_DEFAULT), SETTINGS_DEFAULT);
    QS_ASSERT_EQUALS(settingsFileContent(), SETTINGS_FILE_EMPTY);
  }

  void testStoredValueOverridesDefaultTransient() {
    prepareDefaultFacade();
    QS_ASSERT_EQUALS(facade->value(SETTINGS_KEY, SETTINGS_DEFAULT), SETTINGS_VALUE);
  }

  void testStoredValueOverridesDefaultPersistent() {
    prepareDefaultPersistedFacade();
    QS_ASSERT_EQUALS(facade->value(SETTINGS_KEY, SETTINGS_DEFAULT), SETTINGS_VALUE);
    QS_ASSERT_EQUALS(settingsFileContent(), SETTINGS_FILE_CONTENT);
  }

  void testStoredValueIsReadFromSettings() {
    preparePersistedSettingsWithSettingsFile(SETTINGS_FILE_CONTENT);
    QS_ASSERT_EQUALS(facade->value(SETTINGS_KEY), SETTINGS_VALUE);
  }


  void testComparisonBetweenTransients() {
    prepareDefaultFacade();

    otherFacade = SettingsFacade::transientSettings();
    TS_ASSERT(!(*facade == *otherFacade));

    otherFacade->setValue(SETTINGS_KEY, SETTINGS_VALUE);
    TS_ASSERT(*facade == *otherFacade);
  }

  void testComparingBetweenTransientAndPersisted() {
    prepareDefaultFacade();
    otherFacade = facade;
    prepareDefaultPersistedFacade();
    TS_ASSERT(*otherFacade == *facade);
  }

  void testAssigningTransientToPersisted() {
    prepareDefaultFacade();
    otherFacade = facade;
    prepareEmptyPersistedFacade();
    *facade = *otherFacade;
    TS_ASSERT(*facade == *otherFacade);
    QS_ASSERT_EQUALS(settingsFileContent(), SETTINGS_FILE_CONTENT);
  }

  void testCloningToTransient() {
    prepareDefaultFacade();
    otherFacade = facade->cloneTransiently();
    TS_ASSERT(*otherFacade == *facade);
    QS_ASSERT_EQUALS(otherFacade->value(SETTINGS_KEY), SETTINGS_VALUE);
  }
};
