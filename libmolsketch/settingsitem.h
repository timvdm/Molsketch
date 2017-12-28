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
#ifndef MOLSKETCH_SETTINGSITEM_H
#define MOLSKETCH_SETTINGSITEM_H
#include <QObject>

class QWidget;

namespace Molsketch {

  class SettingsFacade;
  class SettingsItemPrivate;

  class SettingsItem : public QObject
  {
    Q_OBJECT
    Q_DECLARE_PRIVATE(SettingsItem)
  public:
    explicit SettingsItem(const char *description, const QString &key, SettingsFacade* facade, QObject *parent = 0);
    ~SettingsItem();
    virtual QWidget* produceWidget(QObject *parent) const = 0;
    virtual QString serialize() const = 0;
    virtual QVariant getVariant() const = 0;
    virtual void set(const QVariant&) = 0;
    virtual void set(const QString&) = 0;
  protected:
    QScopedPointer<SettingsItemPrivate> d_ptr;
  };

  class DoubleSettingsItem : public SettingsItem {
    Q_OBJECT
  public:
    DoubleSettingsItem(const char* description, const QString &key, SettingsFacade* facade, QObject *parent = 0);
    QWidget *produceWidget(QObject *parent) const override;
    QString serialize() const override;
    QVariant getVariant() const override;
    qreal get() const;
    void set(const QVariant &) override;
    void set(const QString &) override;
  public slots:
    void set(const qreal&);
  signals:
    void updated(const qreal&);
  };
} // namespace Molsketch

#endif // MOLSKETCH_SETTINGSITEM_H
