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
#include "graphicsitem.h"

#include "transformcommand.h"
#include "molscene.h"

#include <QSet>

namespace Molsketch {

  struct transformCommand::privateData
  {
    typedef QPair<graphicsItem*, QPolygonF> itemCoordPair;
    QList<itemCoordPair> Items;
    int originalTrafoType ;
    privateData(const QList<graphicsItem*>& items, const QTransform& trafo, const QPointF& center)
    {
      if (items.isEmpty()) return;
      QTransform transformation;
      transformation.translate(center.x(), center.y());
      transformation = transformation.inverted() *
          trafo * transformation;
      foreach(graphicsItem* item, items)
        Items << qMakePair(item, transformation.map(item->coordinates()));
      originalTrafoType = trafo.type();
    }
    void exchangeCoords()
    {
      for (QList<itemCoordPair>::iterator it = Items.begin() ; it != Items.end() ; ++it)
      {
        QPolygonF coords = it->first->coordinates();
        it->first->setCoordinates(it->second);
        it->second.swap(coords);
      }
      if (!Items.isEmpty() && Items.first().first->scene())
        Items.first().first->scene()->update();
    }
    bool operator!=(const privateData& other)
    {
      QSet<graphicsItem*> itemSet, otherSet;
      foreach(const itemCoordPair& icp, Items)
        itemSet << icp.first;
      foreach(const itemCoordPair& icp, other.Items)
        otherSet << icp.first;
      return itemSet != otherSet;
    }
  };

  transformCommand::transformCommand(graphicsItem *item,
                                     const QTransform& trafo,
                                     const QPointF& center,
                                     QUndoCommand* parent)
    : QUndoCommand(parent),
      d(new transformCommand::privateData(QList<graphicsItem*>() << item, trafo, center))
  {}

  transformCommand::transformCommand(const QList<graphicsItem *>& items,
                                     const QTransform &trafo,
                                     const QPointF& center,
                                     QUndoCommand *parent)
    : QUndoCommand(parent),
      d(new transformCommand::privateData(items, trafo, center))
  {}

  transformCommand::~transformCommand()
  {
    delete d ;
  }

  void transformCommand::undo()
  {
    redo() ;
  }

  void transformCommand::redo()
  {
    d->exchangeCoords();
  }

  bool transformCommand::mergeWith(const QUndoCommand *other)
  {
    const transformCommand* otherTrafo = dynamic_cast<const transformCommand*>(other) ;
    if (!otherTrafo) return false ;
    if ((*d) != *(otherTrafo->d)) return false;
    return true ;
  }

  int transformCommand::id() const
  {
    return d->originalTrafoType ;
  }

} // namespace Molsketch
