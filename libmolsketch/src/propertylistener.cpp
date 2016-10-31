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
#include "propertylistener.h"
#include "graphicsitem.h"

namespace Molsketch {

  struct PropertyListener::PrivateData {
    graphicsItem* item;
    PrivateData() : item(0) {}
  };

  graphicsItem *PropertyListener::item() const
  {
    return d->item;
  }

  void PropertyListener::registerItem(graphicsItem *item)
  {
    if (item == d->item) return;
    graphicsItem* oldItem = d->item;
    d->item = 0;
    if (oldItem) oldItem->unregisterPropertyListener(this);
    d->item = item;
    if (item) item->registerPropertyListener(this);
    propertiesChanged();
  }

  void PropertyListener::propertiesChanged()
  {
    reactToPropertyChange();
  }

  PropertyListener::PropertyListener()
    : d(new PrivateData) {}

  PropertyListener::~PropertyListener() {
    graphicsItem* oldItem = d->item;
    d->item = 0;
    if (oldItem) oldItem->unregisterPropertyListener(this);
    delete d;
  }

} // namespace Molsketch

