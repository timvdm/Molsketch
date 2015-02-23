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
#include "graphicsitem.h"

#include "transformcommand.h"
#include "molscene.h"

namespace Molsketch {

  struct transformCommand::privateData
  {
    QVector<QPointF> coordinates ;
    graphicsItem *item ;
    int originalTrafoType ;
  };

  transformCommand::transformCommand(graphicsItem *item,
                                     const QTransform& trafo,
                                     const QPointF& itemCenter,
                                     QUndoCommand* parent)
    : QUndoCommand(parent),
      d(new transformCommand::privateData)
  {
    d->item = item ;
    QTransform transformation ;
    transformation.translate(itemCenter.x(), itemCenter.y()) ;
    transformation =
        transformation.inverted() *
        trafo *
        transformation
        ;
    foreach(const QPointF& p, item->coordinates())
      d->coordinates << transformation.map(p) ;
    d->originalTrafoType = trafo.type() ;
  }

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
    QVector<QPointF> temp(d->item->coordinates()) ;
    d->item->setCoordinates(d->coordinates) ;
    d->coordinates.swap(temp);
    if (d->item->scene()) d->item->scene()->update();
  }

  bool transformCommand::mergeWith(const QUndoCommand *other)
  {
    const transformCommand* otherTrafo = dynamic_cast<const transformCommand*>(other) ;
    if (!otherTrafo) return false ;
    if (d->item != otherTrafo->d->item) return false ;
    if (d->coordinates.size() != otherTrafo->d->coordinates.size()) return false ;
    return true ;
  }

  int transformCommand::id() const
  {
    return d->originalTrafoType ;
  }

} // namespace Molsketch
