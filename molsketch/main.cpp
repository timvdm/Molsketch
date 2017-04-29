/***************************************************************************
 *   Copyright (C) 2007-2008 by Harm van Eersel                            *
 *   Copyright (C) 2009 Tim Vandermeersch                                  *
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


#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QSettings>

#include "mainwindow.h"
#include "programversion.h"
#include "releasenotesdialog.h"

int main(int argc, char *argv[])
{
#if defined(QT_STATIC_BUILD) || defined(_WIN32)
  Q_INIT_RESOURCE(molsketch);
  Q_INIT_RESOURCE(oxygenicons);
  QIcon::setThemeName("oxygen");
#endif
  QApplication app(argc, argv);

  QCoreApplication::setOrganizationName("SourceForge");
  QCoreApplication::setOrganizationDomain("sourceforge.net");
  QCoreApplication::setApplicationName("Molsketch");

  // Add support for i18n
  QString locale = QLocale::system().name();
  QTranslator translator;
  translator.load(QString("molsketch_") + locale);
  app.installTranslator(&translator);

  MainWindow window;
  window.show();

  QSettings settings;
  ProgramVersion latestReleaseNotesShown(settings.value("latestReleaseNotes").toString()),
      currentVersion(readFileContent(":/version"));
  if (latestReleaseNotesShown < currentVersion) {
    ReleaseNotesDialog releaseNotes;
    releaseNotes.exec();
    settings.setValue("latestReleaseNotes", currentVersion.toString());
  }

  return app.exec();
}

