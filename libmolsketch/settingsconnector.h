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
#ifndef SETTINGSCONNECTOR_H
#define SETTINGSCONNECTOR_H

#include <QObject>
#include <functional>

class QWidget;
class QUndoStack;
class QDoubleSpinBox;
class QCheckBox;

namespace Molsketch {

  class SettingsItem;
  class DoubleSettingsItem;
  class BoolSettingsItem;

  class SettingsConnector : public QObject
  {
    Q_OBJECT
    bool locked;
    QString description;
    SettingsItem *setting;
    QUndoStack *stack;
    std::function<void()> uiToSetting;
    std::function<void()> settingToUi;
    explicit SettingsConnector(QString description, std::function<void()> uiToSetting, std::function<void()> settingToUi, SettingsItem *setting, QUndoStack *stack, QObject *parent = 0);
  public:
    static SettingsConnector *connect(QDoubleSpinBox *control, DoubleSettingsItem *setting, QUndoStack *stack, QString description);
    static SettingsConnector *connect(QCheckBox *control, BoolSettingsItem *setting, QUndoStack *stack, QString description);
    ~SettingsConnector() override;
  private slots:
    void settingChanged();
    void uiChanged();
  };

} // namespace Molsketch

#endif // SETTINGSCONNECTOR_H
