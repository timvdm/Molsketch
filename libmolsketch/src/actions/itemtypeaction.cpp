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
#include "itemtypeaction.h"
#include "itemtypewidget.h"

#include <QMenu>

namespace Molsketch {


  struct ItemTypeAction::privateData
  {
    ItemTypeWidget *typeWidget;
    QMenu *menu;
    bool transferingItemType;
  };

  ItemTypeAction::ItemTypeAction(MolScene *scene)
    : abstractItemAction(scene),
      d(new privateData)
  {
    d->menu = new QMenu;
    d->typeWidget = 0;
    QVBoxLayout *layout = new QVBoxLayout(d->menu);
    d->menu->setLayout(layout);
    connect(this, SIGNAL(itemsChanged()), this, SLOT(checkItemType()));
    setMinimumItemCount(1);
    setMenu(d->menu);
    setCheckable(false);
    d->transferingItemType = false;
  }

  ItemTypeAction::~ItemTypeAction()
  {
    delete d->menu;
    delete d;
  }

  void ItemTypeAction::setItemTypeWidget(ItemTypeWidget *widget)
  {
    delete d->typeWidget;
    d->typeWidget = widget;
    widget->setParent(d->menu);
    d->menu->layout()->addWidget(widget);
    connect(widget, SIGNAL(currentTypeChanged(int)), d->menu, SLOT(close()));
    connect(widget, SIGNAL(currentTypeChanged(int)), this, SLOT(trigger()));
  }

  QString ItemTypeAction::undoName() const
  {
    return tr("change ") + text().toLower();
  }

  int ItemTypeAction::defaultType() const
  {
    return 0;
  }

  void ItemTypeAction::execute()
  {
    if (d->transferingItemType) return;
    attemptBeginMacro(undoName());
    foreach(graphicsItem *item, items())
      applyTypeToItem(item, d->typeWidget->currentType());
    attemptEndMacro();
  }

  void ItemTypeAction::checkItemType()
  {
    if (!d->typeWidget) return;
    d->transferingItemType = true;
    int type = defaultType();
    foreach(graphicsItem* item, items())
      if (getTypeFromItem(item, type)) break;
    d->typeWidget->setCurrentType(type);
    d->transferingItemType = false;
  }


} // namespace Molsketch

