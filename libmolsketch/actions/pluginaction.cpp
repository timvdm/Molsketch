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
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "pluginaction.h"
#include "itemplugin.h"
#include "molscene.h"
#include "commands.h"

#include <QGraphicsSceneMouseEvent>

namespace Molsketch {

  struct pluginAction::privateData
  {
    ItemPlugin *currentPlugin;
  };

  pluginAction::pluginAction(MolScene *scene)
    : multiAction(scene),
      d(new privateData)
  {
    d->currentPlugin = 0;
    setText(tr("Insert plugin"));
    setIcon(QIcon(":images/plugin.svg"));
    // from mainwindow::createMenus()
    foreach(ItemPluginFactory *factory, ItemPluginFactory::factories())
    {
      QAction *newAction = new QAction(factory->icon(), factory->output(), this);
      newAction->setData(factory->output());
      addSubAction(newAction);
    }
  }

  pluginAction::~pluginAction()
  {
    delete d;
  }

  void pluginAction::mousePressEvent(QGraphicsSceneMouseEvent *event)
  {
    event->accept();
    d->currentPlugin = ItemPluginFactory::createInstance(activeSubAction()->data().toString());
    if (!d->currentPlugin) return;
    scene()->addItem(d->currentPlugin);
    d->currentPlugin->setPos(event->scenePos());
  }

  void pluginAction::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
  {
    event->accept();
    if (!d->currentPlugin) return;
    d->currentPlugin->setPos(event->scenePos());
  }

  void pluginAction::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
  {
    event->accept();
    if (!d->currentPlugin) return;
    undoStack()->push(new Commands::AddItem(d->currentPlugin, scene(),
                                            tr("Add ") + activeSubAction()->data().toString() + tr(".")));
    d->currentPlugin = 0;
  }

  // TODO simply insert plugin for selected molecule, if triggered
  // TODO snap to molecule while moving (or after double click?)

} // namespace Molsketch

