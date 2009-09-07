#include <Molsketch/ItemPlugin>

#include <QPainter>


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
