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
#include "settingsfacade.h"

#include <QSettings>
#include <QDebug>

namespace Molsketch {
  // TODO test this
  // TODO test that scene correctly transfers settings

  SettingsFacade::SettingsFacade(QObject *parent) : QObject(parent){}

  class PersistedSettings : public SettingsFacade {
    QSettings *settings;
  public:
    explicit PersistedSettings(QSettings *settings, QObject *parent) : SettingsFacade(parent), settings(settings) {
      settings->setParent(this);
    }

    void setValue(const QString &key, const QVariant &value) override {
      settings->setValue(key, value);
    }
    QVariant value(const QString &key, const QVariant &defaultValue) const override {
      return settings->value(key, defaultValue);
    }

  protected:
    QStringList allKeys() const override {
      return settings->allKeys();
    }
  };

  SettingsFacade *SettingsFacade::persistedSettings(QSettings *settings, QObject *parent) {
    if (!settings) return transientSettings(parent);
    return new PersistedSettings(settings, parent);
  }

  SettingsFacade *SettingsFacade::cloneTransiently() const {
    SettingsFacade *result = transientSettings();
    *result = *this;
    return result;
  }

  SettingsFacade &SettingsFacade::operator =(const SettingsFacade &other) {
    qInfo() << "Transferring settings data:";
    for (auto key : other.allKeys()) {
      auto value = other.value(key);
      qDebug() << "Transferring key" << key
               << "with value" << value;
      setValue(key, other.value(key));
    }
    return *this;
  }

  bool SettingsFacade::operator ==(const SettingsFacade &other) {
    if (allKeys() != other.allKeys()) return false;
    for (auto key : allKeys())
      if (value(key) != other.value(key))
        return false;
    return true;
  }

  class TransientSettings : public SettingsFacade {
    QMap<QString, QVariant> settings;
  public:
    explicit TransientSettings(QObject *parent) : SettingsFacade(parent) {}

    void setValue(const QString &key, const QVariant &value) override {
      settings[key] = value;
    }
    QVariant value(const QString &key, const QVariant &defaultValue) const override {
      return settings.value(key, defaultValue);
    }

  protected:
    QStringList allKeys() const override {
      return settings.keys();
    }
  };

  SettingsFacade *SettingsFacade::transientSettings(QObject *parent) {
    return new TransientSettings(parent);
  }
} // namespace Molsketch
