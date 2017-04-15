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
#ifndef ABSTRACTITEMACTION_H
#define ABSTRACTITEMACTION_H

#include "genericaction.h"

class QGraphicsItem;

namespace Molsketch {

  class graphicsItem;

  class AbstractItemAction : public genericAction
  {
    Q_OBJECT
  public:
    explicit AbstractItemAction(MolScene *parent = 0);
    ~AbstractItemAction();

    /** set an item  (convenience function) */
    void setItem(graphicsItem*) ; // TODO do this via scene's selectionChanged slot
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
  signals:
    void itemsChanged();
  protected:
    /** obtain items currently associated with this action */
    QList<graphicsItem*> items() const ;
    /** will be executed, when action is triggered */
    virtual void execute() = 0 ;
    virtual QSet<graphicsItem *> filterItems(const QList<QGraphicsItem *> &inputItems) const;
  private:
    class privateData;
    privateData *d;
  private slots:
    void gotTrigger() ;
    void updateItems();
  };

} // namespace Molsketch

#endif // MOLSKETCH_ABSTRACTITEMACTION_H
