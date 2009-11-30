#include "hydrogentool.h"

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

  HydrogenTool::HydrogenTool(MolScene *scene) : QObject(0), Tool(scene)
  {
    m_action = 0;
  }

  HydrogenTool::~HydrogenTool()
  {
  }

  QList<QAction*> HydrogenTool::actions()
  {
    QList<QAction*> actions;

    if (m_action) {
      actions.append(m_action);
      return actions;
    }

    m_action = new QAction(QIcon(":images/hydrogens.png"), QObject::tr("Increment/Decrement Hydrogens Tool"), 0);
    connect(m_action, SIGNAL(triggered()), this, SLOT(actionClicked()));
    actions.append(m_action);

    return actions;
  }

  QString HydrogenTool::toolbarName(QAction *action)
  {
    return tr("Tools");
  }

  void HydrogenTool::mousePressEvent(QGraphicsSceneMouseEvent *event)
  {
    QPointF downPos = event->buttonDownScenePos(event->button());

    // Check possible targets
    Atom* atom = scene()->atomAt(downPos);
    if (!atom)
      return;

    QUndoStack *stack = scene()->stack();

    if (event->button() == Qt::RightButton) {
      if (atom->numImplicitHydrogens() > 0)
        stack->push(new RemoveImplicitHydrogen(atom, tr("remove implicit hydrogen")));
    } else {
      stack->push(new AddImplicitHydrogen(atom, tr("add implicit hydrogen")));
    }
  }

  void HydrogenTool::actionClicked()
  {
    ToolGroup *toolGroup = static_cast<ToolGroup*>(m_action->property("toolGroup").value<void*>());
    if (!toolGroup)
      return;

    toolGroup->setActiveTool(this);
  }

}
