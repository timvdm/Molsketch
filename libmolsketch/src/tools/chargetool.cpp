#include "chargetool.h"

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

  ChargeTool::ChargeTool(MolScene *scene) : QObject(0), Tool(scene)
  {
    m_action = 0;
  }

  ChargeTool::~ChargeTool()
  {
  }

  QList<QAction*> ChargeTool::actions()
  {
    QList<QAction*> actions;

    if (m_action) {
      actions.append(m_action);
      return actions;
    }

    m_action = new QAction(QIcon(":images/charges.png"), QObject::tr("Increment/Decrement Charges Tool"), 0);
    connect(m_action, SIGNAL(triggered()), this, SLOT(actionClicked()));
    actions.append(m_action);

    return actions;
  }

  QString ChargeTool::toolbarName(QAction *action)
  {
    return tr("Tools");
  }

  void ChargeTool::mousePressEvent(QGraphicsSceneMouseEvent *event)
  {
    QPointF downPos = event->buttonDownScenePos(event->button());

    // Check possible targets
    Atom* atom = scene()->atomAt(downPos);
    if (!atom)
      return;

    QUndoStack *stack = scene()->stack();

    if (event->button() == Qt::RightButton) {
      stack->push(new DecCharge(atom, tr("decreasing charge")));
    } else {
      stack->push(new IncCharge(atom, tr("increasing charge")));
    }

  }

  void ChargeTool::actionClicked()
  {
    ToolGroup *toolGroup = static_cast<ToolGroup*>(m_action->property("toolGroup").value<void*>());
    if (!toolGroup)
      return;

    toolGroup->setActiveTool(this);
  }

}
