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
#include <QDebug>

namespace Molsketch {

  struct abstractItemAction::privateData
  {
    QSet<graphicsItem *> itemList ;
    abstractItemAction *parent;
    int minItemCount;

    privateData(abstractItemAction* p)
      : parent(p), minItemCount(0) {}
    void checkItems()
    {
      itemList.remove(0) ;
      parent->setEnabled(itemList.size() >= minItemCount);
    }
  };

  abstractItemAction::abstractItemAction(MolScene *parent) :
    genericAction(parent),
    d(new privateData(this))
  {
    connect(this, SIGNAL(triggered()), this, SLOT(gotTrigger())) ;
  }

  abstractItemAction::~abstractItemAction()
  {
    delete d;
  }



  void abstractItemAction::setItem(graphicsItem * item)
  {
    d->itemList.clear();
    d->itemList << item;
  }

  QList<QGraphicsItem*> getFamily(const QList<QGraphicsItem*>& list)
  {
    QList<QGraphicsItem*> family(list);
    foreach(QGraphicsItem* item, list)
      family += getFamily(item->childItems());
    return family;
  }

  void abstractItemAction::setItems(const QList<QGraphicsItem *>& list)
  {
    d->itemList.clear();
    foreach(QGraphicsItem* item, getFamily(list))
    {
      graphicsItem *gItem = dynamic_cast<graphicsItem*>(item);
      if (gItem) d->itemList << gItem;
    }
    d->checkItems();
  }

  void abstractItemAction::removeItem(graphicsItem *item)
  {
    removeItems(QList<graphicsItem*>() << item) ;
  }

  void abstractItemAction::removeItems(const QList<graphicsItem *> &list)
  {
    foreach(graphicsItem* item, list)
      d->itemList.remove(item) ;
    d->checkItems();
  }

  void abstractItemAction::clearItems()
  {
    d->itemList.clear();
    d->checkItems();
  }

  void abstractItemAction::addItem(graphicsItem *item)
  {
    d->itemList << item;
    d->checkItems();
  }

  void abstractItemAction::setMinimumItemCount(const int &count)
  {
    d->minItemCount = count;
    d->checkItems();
  }

  QList<graphicsItem *> abstractItemAction::items() const
  {
    return d->itemList.toList() ;
  }

  void abstractItemAction::gotTrigger()
  {
    if (d->itemList.size() < d->minItemCount) return ;
    qDebug() << "itemaction" << scene() ;
    execute();
  }

} // namespace
