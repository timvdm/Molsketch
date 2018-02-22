/***************************************************************************
 *   Copyright (C) 2018 Hendrik Vennekate                                  *
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
#include "settingsconnector.h"
#include "settingsitem.h"

#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QDebug>

namespace Molsketch {
  class SettingsConnectorPrivate {
    Q_DISABLE_COPY(SettingsConnectorPrivate)
  public:
  };

  SettingsConnector::SettingsConnector(QString description, std::function<void()> uiToSetting, std::function<void()> settingToUi, SettingsItem *setting, QUndoStack* stack, QObject *parent)
    : QObject(parent),
      locked(false),
      description(description),
      setting(setting),
      stack(stack),
      uiToSetting(uiToSetting),
      settingToUi(settingToUi)
  {
    settingChanged();
  }

  SettingsConnector *SettingsConnector::connect(QDoubleSpinBox *control, DoubleSettingsItem *setting, QUndoStack *stack, QString description)
  {
    auto connector = new SettingsConnector(description,
                                 [=] { setting->set(control->value()); },
                                 [=] { qDebug() << "setting to control:" << setting->get(); control->setValue(setting->get()); },
                                 setting, stack, control);
    QObject::connect(control, SIGNAL(valueChanged(double)), connector, SLOT(uiChanged()));
    QObject::connect(setting, SIGNAL(updated(qreal)), connector, SLOT(settingChanged()));
    return connector;
  }

  SettingsConnector *SettingsConnector::connect(QCheckBox *control, BoolSettingsItem *setting, QUndoStack *stack, QString description) {
    auto connector = new SettingsConnector(description,
                                 [=] { setting->set(control->isChecked()); },
                                 [=] { control->setChecked(setting->get()); },
                                 setting, stack, control);
    QObject::connect(control, SIGNAL(toggled(bool)), connector, SLOT(uiChanged()));
    QObject::connect(setting, SIGNAL(updated(bool)), connector, SLOT(settingChanged()));
    return connector;

  }

  SettingsConnector::~SettingsConnector() {}

  void SettingsConnector::settingChanged() {
    if (locked) return;
    locked = true;
    qDebug() << "Updating UI from setting:" << description;
    settingToUi();
    locked = false;
  }

  void SettingsConnector::uiChanged() {
    if (locked) return;
    locked = true;
    qDebug() << "Updating setting from UI: " << description;
    SettingsItemUndoCommand::forCurrentValue(setting, description, stack)->execute();
    uiToSetting();
    locked = false;
  }

} // namespace Molsketch
