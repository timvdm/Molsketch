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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/
#include "colorbutton.h"
#include "fontchooser.h"
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

  // TODO refactor to use ordinary properties widget mechanism
  SettingsConnector *SettingsConnector::connect(QDoubleSpinBox *control, DoubleSettingsItem *setting, QUndoStack *stack, QString description)
  {
    if (!stack) {
      control->setValue(setting->get());
      QObject::connect(control, SIGNAL(valueChanged(double)), setting, SLOT(set(qreal)));
      QObject::connect(setting, SIGNAL(updated(qreal)), control, SLOT(setValue(double)));
      return nullptr;
    }
    auto connector = new SettingsConnector(description,
                                 [=] { setting->set(control->value()); },
                                 [=] { qDebug() << "setting to control:" << setting->get(); control->setValue(setting->get()); },
                                 setting, stack, control);
    QObject::connect(control, SIGNAL(valueChanged(double)), connector, SLOT(uiChanged()));
    QObject::connect(setting, SIGNAL(updated(qreal)), connector, SLOT(settingChanged()));
    return connector;
  }

  SettingsConnector *SettingsConnector::connect(QCheckBox *control, BoolSettingsItem *setting, QUndoStack *stack, QString description) {
    if (!stack) {
      control->setChecked(setting->get());
      QObject::connect(control, SIGNAL(toggled(bool)), setting, SLOT(set(bool)));
      QObject::connect(setting, SIGNAL(updated(bool)), control, SLOT(setChecked(bool)));
      return nullptr;
    }
    auto connector = new SettingsConnector(description,
                                 [=] { setting->set(control->isChecked()); },
                                 [=] { control->setChecked(setting->get()); },
                                 setting, stack, control);
    QObject::connect(control, SIGNAL(toggled(bool)), connector, SLOT(uiChanged()));
    QObject::connect(setting, SIGNAL(updated(bool)), connector, SLOT(settingChanged()));
    return connector;

  }

  SettingsConnector *SettingsConnector::connect(ColorButton *control, ColorSettingsItem *setting, QUndoStack *stack, QString description) {
    if (!stack) {
      control->setColor(setting->get());
      QObject::connect(control, SIGNAL(colorChanged(QColor)), setting, SLOT(set(QColor)));
      QObject::connect(setting, SIGNAL(updated(QColor)), control, SLOT(setColor(QColor)));
      return nullptr;
    }
    auto connector = new SettingsConnector(description,
                                           [=] { setting->set(control->getColor()); },
                                           [=] { control->setColor(setting->get()); },
                                           setting, stack, control);
    QObject::connect(control, SIGNAL(colorChanged(QColor)), connector, SLOT(uiChanged()));
    QObject::connect(setting, SIGNAL(updated(QColor)), connector, SLOT(settingChanged()));
    return connector;
  }

  SettingsConnector *SettingsConnector::connect(FontChooser *control, FontSettingsItem *setting, QUndoStack *stack, QString description) {
    if (!stack) {
      control->setFont(setting->get());
      QObject::connect(control, SIGNAL(fontChanged(QFont)), setting, SLOT(set(QFont)));
      QObject::connect(setting, SIGNAL(updated(QFont)), control, SLOT(setFont(QFont)));
      return nullptr;
    }
    auto connector = new SettingsConnector(description,
                                           [=] { setting->set(control->getSelectedFont()); },
                                           [=] { control->setFont(setting->get()); },
                                           setting, stack, control);
    QObject::connect(control, SIGNAL(fontChanged(QFont)), connector, SLOT(uiChanged()));
    QObject::connect(setting, SIGNAL(updated(QFont)), connector, SLOT(settingChanged()));
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

#ifdef QT_DEBUG
  QDebug operator<<(QDebug debug, const SettingsConnector &connector) {
    debug.nospace() << "SettingsConnector(description: " << connector.description
                    << ", parent: " << connector.parent()
                    << ", setting: " << connector.setting
                    << ", stack: " << connector.stack
                    << ", locked: " << connector.locked
                    << ")";
    return debug;
  }

  QDebug operator<<(QDebug debug, const SettingsConnector *connector) {
    debug.nospace() << "SettingsConnector (" << (void*) connector << ", description: " << connector->description << ")";
    return debug;
  }

#endif

} // namespace Molsketch
