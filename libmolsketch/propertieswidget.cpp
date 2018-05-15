/***************************************************************************
 *   Copyright (C) 2015 Hendrik Vennekate                                  *
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
#include "propertieswidget.h"
#include <QSet>
#include <QUndoCommand>
#include "molscene.h"

namespace Molsketch {

  struct PropertiesWidget::PrivateData {
    PrivateData() : scene(0), lock(false) {}
    MolScene *scene;
    bool lock;
  };

  void PropertiesWidget::attemptToPushUndoCommand(QUndoCommand *cmd)
  {
    if (d->lock) {
      delete cmd;
      return;
    }
    d->lock = true;
    if (d->scene && d->scene->stack()) {
      d->scene->stack()->push(cmd);
      d->lock = false;
      return;
    }
    cmd->redo();
    delete cmd;
    d->lock = false;
  }

  void PropertiesWidget::attemptBeginMacro(const QString &text) const { // TODO write test
    if (d->lock) return;
    MolScene *molscene = dynamic_cast<MolScene*>(scene());
    if (!molscene || !molscene->stack()) return;
    molscene->stack()->beginMacro(text);
  }

  void PropertiesWidget::attemptEndMacro() const { // TODO write test
    MolScene *molscene = dynamic_cast<MolScene*>(scene());
    if (!molscene || !molscene->stack() || d->lock) return;
    molscene->stack()->endMacro();
  }


  MolScene* PropertiesWidget::scene() const
  {
    return d->scene;
  }

  QSet<graphicsItem *> PropertiesWidget::items() const
  {
    if (!d->scene) return QSet<graphicsItem*>();
    QSet<graphicsItem*> items;
    for (QGraphicsItem* item : d->scene->selectedItems())
      items << dynamic_cast<graphicsItem*>(item);
    items.remove(0);
    return items;
  }

  bool PropertiesWidget::blocked() const
  {
    return d->lock;
  }

  PropertiesWidget::PropertiesWidget(QWidget *parent)
    : QWidget(parent), d(new PrivateData) {
    setEnabled(false);
  }

  PropertiesWidget::~PropertiesWidget() {
    delete d;
  }

  void PropertiesWidget::setScene(MolScene *scene)
  {
    if (d->scene) {
      disconnect(d->scene, 0, this, 0);
      if (d->scene->stack()) disconnect(d->scene->stack(), 0, this, 0);
    }
    d->scene = scene;
    if (d->scene) {
      connect(d->scene, SIGNAL(destroyed(QObject*)), this, SLOT(sceneDeleted()));
      connect(d->scene, SIGNAL(selectionChanged()), this, SLOT(propertiesChange()));
      connect(d->scene->stack(), SIGNAL(indexChanged(int)), this, SLOT(propertiesChange()));
    }
    propertiesChange();
    setEnabled(d->scene);
  }

  void PropertiesWidget::propertiesChange()
  {
    if (d->lock) return;
    d->lock = true;
    propertiesChanged();
    d->lock = false;
  }

  void PropertiesWidget::sceneDeleted()
  {
    setScene(0);
  }

} // namespace Molsketch

