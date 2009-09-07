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
