#include "pluginaction.h"
#include "itemplugin.h"
#include "molscene.h"
#include "commands.h"

#include <QGraphicsSceneMouseEvent>

namespace Molsketch {

  struct pluginAction::privateData
  {
    ItemPlugin *currentPlugin;
  };

  pluginAction::pluginAction(MolScene *scene)
    : multiAction(scene),
      d(new privateData)
  {
    d->currentPlugin = 0;
    // from mainwindow::createMenus()
    foreach(ItemPluginFactory *factory, ItemPluginFactory::factories())
    {
      QAction *newAction = new QAction(factory->input() + " -> " + factory->output(), this);
      newAction->setData(factory->output());
      addSubAction(newAction);
    }
  }

  pluginAction::~pluginAction()
  {
    delete d;
  }

  void pluginAction::mousePressEvent(QGraphicsSceneMouseEvent *event)
  {
    d->currentPlugin = ItemPluginFactory::createInstance(activeSubAction()->data().toString());
    if (!d->currentPlugin) return;
    scene()->addItem(d->currentPlugin);
    d->currentPlugin->setPos(event->scenePos());
  }

  void pluginAction::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
  {
    if (!d->currentPlugin) return;
    d->currentPlugin->setPos(event->scenePos());
  }

  void pluginAction::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
  {
    Q_UNUSED(event)
    if (!d->currentPlugin) return;
    undoStack()->push(new Commands::AddItem(d->currentPlugin, scene(),
                                            tr("Add ") + activeSubAction()->data().toString() + tr(".")));
    d->currentPlugin = 0;
  }

  // TODO simply insert plugin for selected molecule, if triggered
  // TODO snap to molecule while moving (or after double click?)

} // namespace Molsketch

