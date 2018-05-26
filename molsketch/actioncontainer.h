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
#ifndef ACTIONCONTAINER_H
#define ACTIONCONTAINER_H

#include <QAction>
#include <QObject>

class ActionContainerPrivate;
class QMenu;
class QToolBar;
class OBabelIfaceLoader;

namespace Molsketch {
  class MolView;
  class MolScene;
}

class ActionContainer : public QObject {
	Q_OBJECT
  Q_DECLARE_PRIVATE(ActionContainer)
  QScopedPointer<ActionContainerPrivate> d_ptr;
public:
	explicit ActionContainer(Molsketch::MolView *view, QObject *parent = nullptr);
  ~ActionContainer();

  QMenu *createEditMenu(QWidget *parent) const;
  QMenu *createViewMenu(QWidget *parent) const;
  QToolBar *createEditToolBar(QWidget *parent) const;
  QToolBar *createZoomToolBar(QWidget *parent) const;

  static QToolBar *createDrawingToolBar(Molsketch::MolScene *scene, QWidget *parent);
  static QToolBar *createModifyToolBar(Molsketch::MolScene *scene, QWidget *parent);
  static QToolBar *createAlignmentToolBar(Molsketch::MolScene *scene, OBabelIfaceLoader *obabelLoader, QWidget *parent);
  static void addContextMenuActions(Molsketch::MolScene *scene);
  static QAction *generateAction(const QString &themeIcon, const QString &backupIcon, const QString &text, const QString &shortcut, const QString &statusTip, QObject *parent);
  template <typename OBJECT_TYPE, typename SLOT_TYPE>
  static QAction *generateAction(const QString &themeIcon, const QString &backupIcon, const QString &text, const QString &shortcut, const QString &statusTip, OBJECT_TYPE *parent,  SLOT_TYPE slot) {
    auto action = generateAction(themeIcon, backupIcon, text, shortcut, statusTip, parent);
    connect(action, &QAction::triggered, parent, slot);
    return action;
  }
};

#endif // ACTIONCONTAINER_H
