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
#ifndef ITEMPLUGIN_H
#define ITEMPLUGIN_H

#include <QtPlugin>
#include <QGraphicsItem>

class QPainter;

namespace Molsketch {

  class ItemPlugin : public QGraphicsItem
  {
    public:
      ItemPlugin();
      virtual ~ItemPlugin();

      virtual QString input() const = 0;
      virtual QString output() const = 0;

      virtual void dropEvent(QGraphicsSceneDragDropEvent *event) = 0;
    protected:
      void paintDefault(QPainter *painter);
      QRectF defaultBoundingRect() const;
  };

  class ItemPluginFactory
  {
    public:
      virtual ~ItemPluginFactory() {}
      virtual QString input() const = 0;
      virtual QString output() const = 0;
 
      static QStringList outputs();
      static QList<ItemPluginFactory*> factories() { return instanceList(); }
      static ItemPlugin* createInstance(const QString &out);
      
      virtual ItemPlugin* createInstance() const = 0;
    protected:
      static QList<ItemPluginFactory*>& instanceList();
  };


#define ITEM_PLUGIN_FACTORY(className, in, out) \
class className##Factory : public ItemPluginFactory { \
  public: \
    className##Factory() { instanceList().append(this); } \
    QString input() const { return in; } \
    QString output() const { return out; } \
    ItemPlugin* createInstance() const { return new className; } \
};

}


#endif // ITEMPLUGIN_H
