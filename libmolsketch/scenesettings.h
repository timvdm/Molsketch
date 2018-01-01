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
#include <QColor>

class QSettings;

namespace Molsketch {

  class SettingsFacade;
  class SceneSettingsPrivate;
  class DoubleSettingsItem;
  class BoolSettingsItem;
  class ColorSettingsItem;
  class FontSettingsItem;

  class SceneSettings : public QObject
  {
    Q_OBJECT
    Q_DECLARE_PRIVATE(SceneSettings)
    QScopedPointer<SceneSettingsPrivate> d_ptr;
  public:
    explicit SceneSettings(SettingsFacade *facade, QObject *parent = 0);
    virtual ~SceneSettings();

#define BOOL_PROPERTY_DECL(NAME) \
  const BoolSettingsItem* NAME() const; \
  BoolSettingsItem* NAME();

    BOOL_PROPERTY_DECL(carbonVisible)
    BOOL_PROPERTY_DECL(electronSystemsVisible)
    BOOL_PROPERTY_DECL(chargeVisible)

#define REAL_PROPERTY_DECL(NAME) \
  const DoubleSettingsItem* NAME() const; \
  DoubleSettingsItem* NAME();

    REAL_PROPERTY_DECL(bondAngle)
    REAL_PROPERTY_DECL(gridLineWidth)
    REAL_PROPERTY_DECL(verticalGridSpacing)
    REAL_PROPERTY_DECL(horizontalGridSpacing)
    REAL_PROPERTY_DECL(radicalDiameter)
    REAL_PROPERTY_DECL(lonePairLength)
    REAL_PROPERTY_DECL(lonePairLineWidth)
    REAL_PROPERTY_DECL(bondWidth)
    REAL_PROPERTY_DECL(bondLength)

#define PROPERTY_DECL(TYPE, NAME) \
  const TYPE* NAME() const; \
  TYPE* NAME();

    PROPERTY_DECL(FontSettingsItem, atomFont)
    PROPERTY_DECL(ColorSettingsItem, gridColor)

    enum MouseWheelMode { // TODO migrate this. This is not actually part of the scene settings, but rather the application settings
      Unset,
      CycleTools,
      Zoom,
    };
    void setMouseWheelMode(const MouseWheelMode&);
    MouseWheelMode getMouseWheelMode() const;
    const SettingsFacade &settingsFacade() const;
  signals:
    void settingsChanged();
  protected:
    SettingsFacade &settingsFacade();
  };

} // namespace Molsketch

#endif // MOLSKETCH_SCENESETTINGS_H
