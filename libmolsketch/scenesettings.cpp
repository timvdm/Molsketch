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
#include <QFont>
#include <QSettings>
#include "scenesettings.h"

Q_DECLARE_METATYPE(Molsketch::SceneSettings::MouseWheelMode)

namespace Molsketch {

  static const char CARBON_VISIBLE[] = "carbon-visible";
  static const char CHARGE_VISIBLE[] = "charge-visible";
  static const char ELECTRON_SYSTEMS_VISIBLE[] = "electronSystems-visible";
  static const char ATOM_FONT[] = "atom-symbol-font";
  static const char BOND_LENGTH[] = "bond-length";
  static const char BOND_WIDTH[] = "bond-width";
  static const char BOND_ANGLE[] = "bond-angle";
  static const char MOUSE_CYCLE_MODE[] = "mouse-cycle-mode";
  static const char LONE_PAIR_LINE_WIDTH[] = "lone-pair-line-width";
  static const char LONE_PAIR_LENGTH[] = "lone-pair-length";
  static const char RADICAL_DIAMETER[] = "radical-diameter";

  class SceneSettingsPrivate {
    Q_DISABLE_COPY(SceneSettingsPrivate)
  public:
    SceneSettingsPrivate(){}
    QSettings settings;
  };

  SceneSettings::SceneSettings(QObject *parent)
    : QObject(parent),
      d_ptr(new SceneSettingsPrivate)
  {}

  SceneSettings::~SceneSettings() {}

#define BOOL_PROPERTY(NAME, CONFIGSTRING) \
  void SceneSettings::set##NAME(const bool& value) { settings().setValue(CONFIGSTRING, value); } \
  bool SceneSettings::is##NAME() const { return settings().value(CONFIGSTRING).toBool(); }

#define REAL_PROPERTY(NAME, CONFIGSTRING, DEFAULT) \
  void SceneSettings::set##NAME(const qreal& value) { settings().setValue(CONFIGSTRING, value); } \
  qreal SceneSettings::get##NAME() const { return settings().value(CONFIGSTRING, DEFAULT).toReal(); }

  BOOL_PROPERTY(CarbonVisible, CARBON_VISIBLE)
  BOOL_PROPERTY(ChargeVisible, CHARGE_VISIBLE)
  BOOL_PROPERTY(ElectronSystemsVisible, ELECTRON_SYSTEMS_VISIBLE)

  PROPERTY(AtomFont, QFont, ATOM_FONT)

  REAL_PROPERTY(BondLength, BOND_LENGTH, 40)
  REAL_PROPERTY(BondWidth, BOND_WIDTH, 1)
  REAL_PROPERTY(BondAngle, BOND_ANGLE, 30)
  REAL_PROPERTY(LonePairLineWidth, LONE_PAIR_LINE_WIDTH, 1)
  REAL_PROPERTY(LonePairLength, LONE_PAIR_LENGTH, 7)
  REAL_PROPERTY(RadicalDiameter, RADICAL_DIAMETER, 3)

  PROPERTY(MouseWheelMode, SceneSettings::MouseWheelMode, MOUSE_CYCLE_MODE)

  QSettings &SceneSettings::settings() {
    Q_D(SceneSettings);
    return d->settings;
  }

  const QSettings &SceneSettings::settings() const
  {
    Q_D(const SceneSettings);
    return d->settings;
  }

} // namespace Molsketch
