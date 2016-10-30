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

#include <QGraphicsSceneDragDropEvent>
#include <QIcon>
#include <QPainter>
#include <QUndoCommand>

#include "itemplugin.h"
#include "mimemolecule.h"
#include "molecule.h"
#include "molscene.h"


namespace Molsketch {

  class connectToMoleculeCommand : public QUndoCommand
  {
  private:
    QGraphicsItem *oldParent;
    Molecule *molecule;
    ItemPlugin *plugin;
    QPointF originalPosition;
  public:
    connectToMoleculeCommand(Molecule *m,
                             ItemPlugin *p)
      : oldParent(p->parentItem()),
        molecule(m),
        plugin(p),
        originalPosition(p->pos())
    {}

    void redo()
    {
      plugin->setPos(molecule->boundingRect().bottomLeft());
      plugin->setParentItem(molecule);
    }

    void undo()
    {
      plugin->setPos(originalPosition);
      plugin->setParentItem(oldParent);
    }

    bool mergeWith(const QUndoCommand *otherCommand)
    {
      const connectToMoleculeCommand *other = dynamic_cast<const connectToMoleculeCommand*>(otherCommand);
      if (!other) return false;
      if (other->plugin != plugin) return false;
      molecule = other->molecule;
      return true;
    }

    int id() const {return 2000;} // TODO centralize
  };

  ItemPlugin::ItemPlugin()
  {
    setAcceptDrops(true);
  }

  ItemPlugin::~ItemPlugin()
  {
  }

  void ItemPlugin::dropEvent(QGraphicsSceneDragDropEvent *event)
  {
    const MimeMolecule *mimeMol = dynamic_cast<const MimeMolecule*>(event->mimeData());
    if (!mimeMol) return;
    if (!mimeMol->molecule()) return;
    attemptUndoPush(new connectToMoleculeCommand(mimeMol->molecule(),
                                                 this));
  }

  QPolygonF ItemPlugin::coordinates() const
  {
    return QPolygonF() << pos();
  }

  void ItemPlugin::setCoordinates(const QVector<QPointF> &c)
  {
    if (c.isEmpty()) return;
    setPos(c.first());
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

  QString ItemPlugin::xmlName() const
  {
    return "plugin";
  }

  void ItemPlugin::readAttributes(const QXmlStreamAttributes &attributes)
  {
    setPos(attributes.value("posX").toString().toDouble(), attributes.value("posY").toString().toDouble());
    QGraphicsItem *possibleParent = scene()->items().at(attributes.value("molecule").toString().toInt()); // TODO doesn't work if plugin was created before molecule
    if (possibleParent) setParentItem(possibleParent);
  }

  QXmlStreamAttributes ItemPlugin::xmlAttributes() const
  {
    QXmlStreamAttributes attributes;
    attributes.append("posX", QString::number(pos().x())); // TODO to graphicsItem
    attributes.append("posY", QString::number(pos().y()));
    attributes.append("molecule", QString::number(scene()->items().indexOf(parentItem())));
    attributes.append("type", output());
    return attributes;
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

  QIcon ItemPluginFactory::icon() const
  {
    return QIcon(":images/plugin.svg");
  }

  QStringList ItemPluginFactory::outputs()
  {
    QStringList out;
    foreach (ItemPluginFactory *plugin, instanceList())
      out << plugin->output();
    return out;        
  }

}
