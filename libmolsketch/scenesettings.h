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
#ifndef MOLSKETCH_SCENESETTINGS_H
#define MOLSKETCH_SCENESETTINGS_H

#include <QObject>

class QSettings;

namespace Molsketch {

  class SceneSettingsPrivate;

  class SceneSettings : public QObject
  {
    Q_DECLARE_PRIVATE(SceneSettings)
    QScopedPointer<SceneSettingsPrivate> d_ptr;
  public:
    explicit SceneSettings(QObject *parent = 0);
    virtual ~SceneSettings();
    void setCarbonVisible(const bool&);
    bool isCarbonVisible() const;
    void setChargeVisible(const bool&);
    bool isChargeVisible() const;
    void setElectronSystemsVisible(const bool&);
    bool isElectronSystemsVisible() const;
    void setAtomFont(const QFont&);
    QFont getAtomFont() const;
    void setBondLength(const qreal&);
    qreal getBondLength() const;
    void setBondWidth(const qreal&);
    qreal getBondWidth() const;
    void setBondAngle(const qreal&);
    qreal getBondAngle() const;

    enum MouseWheelMode {
      Unset,
      CycleTools,
      Zoom,
    };
    void setMouseWheelMode(const MouseWheelMode&);
    MouseWheelMode getMouseWheelMode() const;
  protected:
    QSettings& settings();
    const QSettings& settings() const;
  };

} // namespace Molsketch

#define PROPERTY(NAME, TYPE, CONFIGSTRING) \
  void SceneSettings::set##NAME(const TYPE& value) { settings().setValue(CONFIGSTRING, value); } \
  TYPE SceneSettings::get##NAME() const { return settings().value(CONFIGSTRING).value<TYPE>(); }

#define STRING_PROPERTY(NAME, CONFIGSTRING) \
  void SceneSettings::set##NAME(const QString& value) { settings().setValue(CONFIGSTRING, value); } \
  QString SceneSettings::get##NAME() const { return settings().value(CONFIGSTRING).toString(); }

#endif // MOLSKETCH_SCENESETTINGS_H
