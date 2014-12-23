/***************************************************************************
 *   Copyright (C) 2007-2008 by Harm van Eersel                            *
 *   Copyright (C) 2009 Tim Vandermeersch                                  *
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

#include "abstractitemaction.h"
#include <QUndoStack>
#include "molscene.h"
#include "graphicsitem.h"

namespace Molsketch {

  abstractItemAction::abstractItemAction(QObject *parent) :
    QAction(parent),
    UndoStack(0)
  {
    connect(this, SIGNAL(triggered()), this, SLOT(gotTrigger())) ;
    setEnabled(false) ;
  }

  void abstractItemAction::setItem(graphicsItem * item)
  {
    setItems(QList<graphicsItem*>() << item) ;
  }

  void abstractItemAction::setItems(const QList<graphicsItem *>& list)
  {
    itemList = list ;
    checkItems();
  }

  void abstractItemAction::removeItem(graphicsItem *item)
  {
    removeItems(QList<graphicsItem*>() << item) ;
  }

  void abstractItemAction::removeItems(const QList<graphicsItem *> &list)
  {
    foreach(graphicsItem* item, list)
      itemList.removeAll(item) ;
    checkItems();
  }

  void abstractItemAction::clearItems()
  {
    itemList.clear();
    checkItems();
  }

  QList<graphicsItem *> abstractItemAction::items() const
  {
    return itemList ;
  }

  QUndoStack *abstractItemAction::undoStack() const
  {
    return UndoStack ;
  }

  void abstractItemAction::checkItems()
  {
    itemList.removeAll(0) ;
    setEnabled(!itemList.isEmpty());
  }

  void abstractItemAction::gotTrigger()
  {
    if (itemList.isEmpty()) return ;
    UndoStack = 0 ;
    foreach(graphicsItem* item, itemList)
    {
      MolScene *scene = qobject_cast<MolScene*>(item->scene()) ;
      if (!scene) continue ;
      UndoStack = scene->stack() ;
      if (UndoStack) break ; // TODO improve
    }
    if (!UndoStack) return ;
    execute();
  }

} // namespace
