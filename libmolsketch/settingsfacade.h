/***************************************************************************
 *   Copyright (C) 2017 Hendrik Vennekate                                  *
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
#ifndef SETTINGSFACADE_H
#define SETTINGSFACADE_H

#include <QObject>
#include <QVariant>

class QSettings;

namespace Molsketch {

  class SettingsFacade : public QObject
  {
    Q_OBJECT
  public:
    static SettingsFacade *transientSettings(QObject *parent = 0);
    static SettingsFacade *persistedSettings(QSettings* settings, QObject *parent = 0);
    SettingsFacade *cloneTransiently() const;
    virtual void setValue(const QString& key, const QVariant& value) = 0;
    virtual QVariant value(const QString& key, const QVariant &defaultValue = QVariant()) const = 0;
    SettingsFacade& operator =(const SettingsFacade& other);
    bool operator ==(const SettingsFacade& other);
    virtual QStringList allKeys() const = 0;
  protected:
    explicit SettingsFacade(QObject *parent = nullptr);
  };

} // namespace Molsketch

#endif // SETTINGSFACADE_H
