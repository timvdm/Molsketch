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

#ifndef APPLICATIONSETTINGS_H
#define APPLICATIONSETTINGS_H

#include <QObject>
#include "scenesettings.h"

class ProgramVersion;
class QByteArray;

class ApplicationSettings : public Molsketch::SceneSettings
{
  Q_OBJECT
public:
  explicit ApplicationSettings(QObject *parent = 0);

  ProgramVersion latestReleaseNotesVersionShown() const;
  ProgramVersion currentVersion() const;
  void updateReleaseNotesShownVersion();
  QString versionNick() const;
  void setLibraries(const QStringList&);
  QStringList libraries() const;
  void setToolButtonStyle(const Qt::ToolButtonStyle&);
  Qt::ToolButtonStyle toolButtonStyle() const;
  void setWindowPosition(const QPoint&);
  QPoint windowPosition() const;
  void setWindowSize(const QSize&);
  QSize windowSize() const;
  void setWindowState(const QByteArray&);
  QByteArray windowState() const;
  void setLastPath(const QString&);
  QString lastPath() const;
  void setAutoSaveInterval(const int&);
  int autoSaveInterval() const;
  void setObabelIfacePath(const QString&);
  QString obabelIfacePaht() const;
  void setObabelFormatsPath(const QString&);
  QString obabelFormatsPath() const;
};

#endif // APPLICATIONSETTINGS_H
