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
#include "propertywidget.h"
#include <QMutex>
#include "graphicsitem.h"

namespace Molsketch {

  struct PropertyWidget::PrivateData {
    graphicsItem* item;
    QMutex lock;
    PrivateData() : item(0) {}
  };

  graphicsItem *PropertyWidget::item() const
  {
    return d->item;
  }

  void PropertyWidget::registerItem(graphicsItem *item)
  {
    if (item == d->item) return;
    graphicsItem* oldItem = d->item;
    d->item = 0;
    if (oldItem) oldItem->unregisterPropertyListener(this);
    d->item = item;
    if (item) item->registerPropertyListener(this);
    propertiesChanged();
  }

  void PropertyWidget::propertiesChanged()
  {
    if (!d->lock.tryLock()) return;
    reactToPropertyChange();
    d->lock.unlock();
  }

  void PropertyWidget::applyProperty()
  {
    if (!d->item) return;
    if (!d->lock.tryLock()) return;
    transferProperty();
    d->lock.unlock();
  }

  PropertyWidget::PropertyWidget(QWidget *parent)
    : QWidget(parent), d(new PrivateData) {}

  PropertyWidget::~PropertyWidget() {
    graphicsItem* oldItem = d->item;
    d->item = 0;
    if (oldItem) oldItem->unregisterPropertyListener(this);
    delete d;
  }

} // namespace Molsketch

