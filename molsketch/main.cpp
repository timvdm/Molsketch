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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/


#include <QApplication>
#include <QTranslator>
#include <QLocale>

#include "applicationsettings.h"
#include "mainwindow.h"
#include "programversion.h"
#include "releasenotesdialog.h"

void messageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
        fprintf(stderr, "Debug: (%s:%u, %s) %s\n", context.file, context.line, context.function, localMsg.constData());
        break;
    case QtWarningMsg:
        fprintf(stderr, "Warning: (%s:%u, %s) %s\n", context.file, context.line, context.function, localMsg.constData());
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Critical: (%s:%u, %s) %s\n", context.file, context.line, context.function, localMsg.constData());
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal: (%s:%u, %s) %s\n", context.file, context.line, context.function, localMsg.constData());
        abort();
      case QtInfoMsg:
        fprintf(stderr, "Info: (%s:%u, %s) %s\n", context.file, context.line, context.function, localMsg.constData());
    }
}

int main(int argc, char *argv[])
{
  qInstallMessageHandler(messageOutput);
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

  ApplicationSettings appSettings;

  if (appSettings.latestReleaseNotesVersionShown() < appSettings.currentVersion()) {
    ReleaseNotesDialog().exec(); // TODO check that this still works
    appSettings.updateReleaseNotesShownVersion();
  }

  return app.exec();
}

