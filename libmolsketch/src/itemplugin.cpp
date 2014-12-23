/***************************************************************************
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

#include <QPainter>

#include "itemplugin.h"


namespace Molsketch {

  ItemPlugin::ItemPlugin()
  {
    setAcceptDrops(true);
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
  }

  ItemPlugin::~ItemPlugin()
  {
  }
 
  void ItemPlugin::paintDefault(QPainter *painter)
  {
    painter->drawRect(boundingRect());
    QPen pen = painter->pen();
    // draw connect symbol in the center
    painter->drawLine(10, 0, 30, 0);
    painter->drawEllipse(0, -5, 10, 10);
    painter->drawLine(-30, 0, -10, 0);
    painter->drawArc(-10, -10, 20, 20, 16 * 90, 16 * 180);
    // draw input/output
    painter->drawText(QRectF(-100, -50, 200, 20), Qt::AlignCenter, input());
    painter->drawText(QRectF(-100, 30, 200, 20), Qt::AlignCenter, output());

  }
  
  QRectF ItemPlugin::defaultBoundingRect() const
  {
    return QRectF(-100, -50, 200, 100);
  }
      
  ItemPlugin* ItemPluginFactory::createInstance(const QString &out)
  {
    foreach (ItemPluginFactory *plugin, instanceList()) {
      if (plugin->output() == out)
        return plugin->createInstance();    
    }
  
    return 0;
  }

  QList<ItemPluginFactory*>& ItemPluginFactory::instanceList()
  {
    static QList<ItemPluginFactory*> list = QList<ItemPluginFactory*>();
    return list;  
  }

  QStringList ItemPluginFactory::outputs()
  {
    QStringList out;
    foreach (ItemPluginFactory *plugin, instanceList())
      out << plugin->output();
    return out;        
  }

}
