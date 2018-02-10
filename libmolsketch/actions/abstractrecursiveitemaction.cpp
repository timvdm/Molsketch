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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#include "abstractrecursiveitemaction.h"
#include <QUndoStack>
#include "molscene.h"
#include "graphicsitem.h"
#include <QDebug>

namespace Molsketch {

  QList<QGraphicsItem*> getFamily(const QList<QGraphicsItem*>& list)
  {
    QList<QGraphicsItem*> family(list);
    foreach(QGraphicsItem* item, list)
      family += getFamily(item->childItems());
    return family;
  }

  abstractRecursiveItemAction::abstractRecursiveItemAction(MolScene *parent)
    : AbstractItemAction(parent)
  {}

  QSet<graphicsItem *> abstractRecursiveItemAction::filterItems(const QList<QGraphicsItem *> &list) const
  {
    QSet<graphicsItem*> result;
    foreach(QGraphicsItem *item, getFamily(list))
    {
      graphicsItem *gItem = dynamic_cast<graphicsItem*>(item);
      if (gItem) result << gItem;
    }
    return result;
  }

} // namespace
