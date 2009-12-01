#include "movetool.h"

#include "../toolgroup.h"
#include "../atom.h"
#include "../bond.h"
#include "../molscene.h"
#include "../commands.h"
#include "../math2d.h"

#include <cmath>
#include <QDebug>

namespace Molsketch {

  using namespace Commands;

  MoveTool::MoveTool(MolScene *scene) : QObject(0), Tool(scene)
  {
    m_action = 0;
  }

  MoveTool::~MoveTool()
  {
  }

  QList<QAction*> MoveTool::actions()
  {
    QList<QAction*> actions;

    if (m_action) {
      actions.append(m_action);
      return actions;
    }

    m_action = new QAction(QIcon(":images/move.png"), QObject::tr("Move Tool"), 0);
    connect(m_action, SIGNAL(triggered()), this, SLOT(actionClicked()));
    actions.append(m_action);

    return actions;
  }

  QString MoveTool::toolbarName(QAction *action)
  {
    Q_UNUSED(action);
    return tr("Tools");
  }

  void MoveTool::activated()
  {
    // Reset moveflag (movebug)
    foreach(QGraphicsItem* item, scene()->items())
      item->setFlag(QGraphicsItem::ItemIsMovable, false);

    // enable moving for all Molecule and atom items
    foreach(QGraphicsItem* item, scene()->items())
      if (item->type() == Molecule::Type || item->type() == Atom::Type)
        item->setFlag(QGraphicsItem::ItemIsSelectable, true);
  }

  void MoveTool::deactivated()
  {
    // Reset moveflag (movebug)
    foreach(QGraphicsItem* item, scene()->items())
      item->setFlag(QGraphicsItem::ItemIsMovable, false);

    // disable moving for all Molecule and atom items
    foreach(QGraphicsItem* item, scene()->items())
      if (item->type() == Molecule::Type || item->type() == Atom::Type)
        item->setFlag(QGraphicsItem::ItemIsSelectable, false);
  }

  void MoveTool::mousePressEvent(QGraphicsSceneMouseEvent *event)
  {
    qDebug() << "mousePressEvent";
    // Check whether to select an item
    QGraphicsItem * item = scene()->itemAt(event->scenePos());
    if (item && !item->isSelected()) {
      scene()->clearSelection();
      item->setSelected(true);
      event->accept();
    }

    // Flag the selected items as moveable
    scene()->clearFocus();
    foreach(QGraphicsItem* item, scene()->selectedItems())
      item->setFlag(QGraphicsItem::ItemIsMovable, true);
    //emit copyAvailable(!selectedItems().isEmpty()); FIXME
  }

  void MoveTool::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
  {
    event->accept();
  }

  void MoveTool::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
  {
    QPointF moveVector = event->scenePos() - event->buttonDownScenePos(event->button());
    if (moveVector.isNull()) {
      scene()->clearFocus();
      return;
    }

    QUndoStack *stack = scene()->stack();

    stack->beginMacro(tr("moving item(s)"));
    foreach(QGraphicsItem* item, scene()->selectedItems()) {
      // reset the movement
      item->moveBy(-moveVector.x(), -moveVector.y());
      // perform the movement as undoable command
      if (!moveVector.isNull())
        stack->push(new MoveItem(item, moveVector, tr("moving item(s)")));
      item->setFlag(QGraphicsItem::ItemIsMovable, false);
    }
    stack->endMacro();

    scene()->clearFocus();
  }





  void MoveTool::actionClicked()
  {
    ToolGroup *toolGroup = static_cast<ToolGroup*>(m_action->property("toolGroup").value<void*>());
    if (!toolGroup)
      return;

    qDebug() << "action triggered";

    toolGroup->setActiveTool(this);
  }





}

