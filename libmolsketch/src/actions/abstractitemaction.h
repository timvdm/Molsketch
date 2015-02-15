/***************************************************************************
 *   Copyright (C) 2007 by Harm van Eersel                                 *
 *   devsciurus@xs4all.nl                                                  *
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

/** @file
 * This file is part of molsKetch and contains the abstractItemAction class.
 *
 * This class provides the infrastructure for an action that operates on
 * one or more items.
 *
 * @author Hendrik Vennekate <HVennekate@gmx.de>
 * @since Lithium
 */

#ifndef ABSTRACTITEMACTION_H
#define ABSTRACTITEMACTION_H

#include "genericaction.h"
#include <QList>

class QGraphicsItem;

namespace Molsketch
{
  class graphicsItem ;

class abstractItemAction : public genericAction
{
  Q_OBJECT
public:
  explicit abstractItemAction(MolScene *parent = 0);
  ~abstractItemAction();

  /** set an item  (convenience function) */
  void setItem(graphicsItem*) ;
  /** set multiple items for the action (previously set items are discarded) */
  void setItems(const QList<QGraphicsItem *> &) ;
  /** convenience function */
  void removeItem(graphicsItem*) ;
  /** remove items (e.g. because they have been deleted) */
  void removeItems(const QList<graphicsItem*>&) ;
  /** remove all items */
  void clearItems() ;
  /** add one item */
  void addItem(graphicsItem*);
  void setMinimumItemCount(const int& count);
protected:
  /** obtain items currently associated with this action */
  QList<graphicsItem*> items() const ;
  /** will be executed, when action is triggered */
  virtual void execute() = 0 ;
private:
  class privateData;
  privateData *d;
private slots:
  void gotTrigger() ;
};

} // namespace

#define ITERATEOVERITEMSMACRO(COMMENT,COMMAND,VALUE) \
  undoStack()->beginMacro(tr(COMMENT)) ;\
  foreach(graphicsItem* item, items())\
    undoStack()->push(new Commands::COMMAND(VALUE, item)) ;\
  undoStack()->endMacro() ;
#endif // ABSTRACTITEMACTION_H
