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

#include <tempfileprovider.h>
#include <QDebug>
#include <QTemporaryFile>
#include <scenesettings.h>
#include <settingsfacade.h>
#include <settingsitem.h>
#include <QSettings>
#include <QRegularExpression>
#include "utilities.h"

using namespace Molsketch;

class SettingsUnitTest : public TempFileProvider, public CxxTest::TestSuite {
  const QString SETTINGS_FILE_CONTENT = "bond-angle=25";
  const qreal BOND_ANGLE = 25;
  const qreal DEFAULT_BOND_ANGLE = 30;

public:
  void testSceneSettingsArePersisted() {
    QFile *settingsFile = createTemporaryFile();
    settingsFile->open(QFile::ReadOnly);
    SceneSettings *settings = new SceneSettings(SettingsFacade::persistedSettings(new QSettings(settingsFile->fileName(), QSettings::IniFormat)));
    settings->bondAngle()->set(BOND_ANGLE);
    delete settings;
    QFile readFile(settingsFile->fileName());
    readFile.open(QFile::ReadOnly);
    QString fileContent = readFile.readAll();
    TS_ASSERT(fileContent.contains(SETTINGS_FILE_CONTENT)); // TODO QString assertions
  }

  void testPersistedSceneSettingsAreRead() {
    QFile *settingsFile = createTemporaryFile();
    settingsFile->open(QFile::WriteOnly);
    settingsFile->write(SETTINGS_FILE_CONTENT.toUtf8());
    settingsFile->close();
    SettingsFacade *facade = SettingsFacade::persistedSettings(new QSettings(settingsFile->fileName(), QSettings::IniFormat));
    SceneSettings *settings = new SceneSettings(facade);
    TS_ASSERT_EQUALS(settings->bondAngle()->get(), BOND_ANGLE);
  }

  void testSceneSettingsReturnDefault() {
    QFile *settingsFile = createTemporaryFile();
    settingsFile->open(QFile::WriteOnly);
    settingsFile->close();
    SceneSettings *settings = new SceneSettings(SettingsFacade::persistedSettings(new QSettings(settingsFile->fileName(), QSettings::IniFormat)));
    TS_ASSERT_EQUALS(settings->bondAngle()->get(), DEFAULT_BOND_ANGLE);
  }

  // TODO mouse wheel settings test
  // TODO make sure no setting occurs twice (individual keys -> iterate over settings item list)
};
