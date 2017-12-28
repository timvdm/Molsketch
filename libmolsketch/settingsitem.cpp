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
#include "settingsitem.h"
#include <QDebug>

namespace Molsketch {

  struct SettingsItemPrivate {
    SettingsFacade *facade;
    const char* description;
    QString key;
  };

  SettingsItem::SettingsItem(const char *description, const QString& key, SettingsFacade *facade, QObject *parent)
    : QObject(parent),
      d_ptr(new SettingsItemPrivate)
  {
    Q_D(SettingsItem);
    d->facade = facade;
    d->description = description;
    d->key = key;
  }

  SettingsItem::~SettingsItem() {}

  DoubleSettingsItem::DoubleSettingsItem(const char *description, const QString& key, SettingsFacade *facade, QObject *parent)
    : SettingsItem(description, key, facade, parent) {}

  QWidget *DoubleSettingsItem::produceWidget(QObject *parent) const {

  }

  QString DoubleSettingsItem::serialize() const {
    return QString::number(get());
  }

  QVariant DoubleSettingsItem::getVariant() const {
    return QVariant(get());
  }

  qreal DoubleSettingsItem::get() const {
    return d_ptr->facade->value(d_ptr->key).toDouble();
  }

  void DoubleSettingsItem::set(const QVariant &value) {
    d_ptr->facade->setValue(d_ptr->key, value);
    emit updated(get());
  }

  void DoubleSettingsItem::set(const QString &value) {
    set(value.toDouble());
  }

  void DoubleSettingsItem::set(const qreal &value) {
    set(QVariant(value));
  }

} // namespace Molsketch
