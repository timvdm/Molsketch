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
#include "stringify.h"
#include <QColor>
#include <QFont>

namespace Molsketch {

  struct SettingsItemPrivate {
    SettingsFacade *facade;
    QString key;
  };

  SettingsItem::SettingsItem(const QString& key, SettingsFacade *facade, QObject *parent)
    : QObject(parent),
      d_ptr(new SettingsItemPrivate)
  {
    Q_D(SettingsItem);
    d->facade = facade;
    d->key = key;
  }

  SettingsItem::~SettingsItem() {}

  DoubleSettingsItem::DoubleSettingsItem(const QString& key, SettingsFacade *facade, QObject *parent)
    : SettingsItem(key, facade, parent) {}

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

  BoolSettingsItem::BoolSettingsItem(const QString &key, SettingsFacade *facade, QObject *parent)
    : SettingsItem(key, facade, parent) {}

  QString BoolSettingsItem::serialize() const {
    return get() ? "True" : "False";
  }

  QVariant BoolSettingsItem::getVariant() const {
    return QVariant(get());
  }

  bool BoolSettingsItem::get() const {
    return d_ptr->facade->value(d_ptr->key).toBool();
  }

  void BoolSettingsItem::set(const QVariant &value) {
    d_ptr->facade->setValue(d_ptr->key, value);
    emit updated(get());
  }

  void BoolSettingsItem::set(const QString &value) {
    set(value == "True" || value == "true");
  }

  void BoolSettingsItem::set(const bool &value) {
    set(QVariant(value));
  }

  ColorSettingsItem::ColorSettingsItem(const QString &key, SettingsFacade *facade, QObject *parent)
    : SettingsItem(key, facade, parent) {}

  QString ColorSettingsItem::serialize() const {
    return stringify(get());
  }

  QVariant ColorSettingsItem::getVariant() const {
    return QVariant(get());
  }

  QColor ColorSettingsItem::get() const {
    return d_ptr->facade->value(d_ptr->key).value<QColor>();
  }

  void ColorSettingsItem::set(const QVariant &value) {
    d_ptr->facade->setValue(d_ptr->key, value);
    emit updated(get());
  }

  void ColorSettingsItem::set(const QString &value) {
    set(makeFromString<QColor>(value));
  }

  void ColorSettingsItem::set(const QColor &value) {
    set(QVariant(value));
  }

  FontSettingsItem::FontSettingsItem(const QString &key, SettingsFacade *facade, QObject *parent)
    : SettingsItem(key, facade, parent) {}

  QString FontSettingsItem::serialize() const {
    return stringify(get());
  }

  QVariant FontSettingsItem::getVariant() const {
    return QVariant(get());
  }

  QFont FontSettingsItem::get() const {
    return d_ptr->facade->value(d_ptr->key).value<QFont>();
  }

  void FontSettingsItem::set(const QVariant &value) {
    d_ptr->facade->setValue(d_ptr->key, value);
    emit updated(get());
  }

  void FontSettingsItem::set(const QString &value) {
    set(makeFromString<QFont>(value));
  }

  void FontSettingsItem::set(const QFont &value) {
    set(QVariant(value));
  }
} // namespace Molsketch
