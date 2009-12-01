#include "reactionarrowtool.h"

#include "../toolgroup.h"
#include "../atom.h"
#include "../bond.h"
#include "../molscene.h"
#include "../commands.h"
#include "../reactionarrow.h"

#include <cmath>
#include <QDebug>

namespace Molsketch {

  using namespace Commands;

  ReactionArrowTool::ReactionArrowTool(MolScene *scene) : QObject(0), Tool(scene)
  {
    m_action = 0;
  }

  ReactionArrowTool::~ReactionArrowTool()
  {
  }

  QList<QAction*> ReactionArrowTool::actions()
  {
    QList<QAction*> actions;

    if (m_action) {
      actions.append(m_action);
      return actions;
    }

    m_action = new QAction(QIcon(":images/reactionarrow.png"), QObject::tr("Reaction Arrow Tool"), 0);
    connect(m_action, SIGNAL(triggered()), this, SLOT(actionClicked()));
    actions.append(m_action);

    return actions;
  }

  QString ReactionArrowTool::toolbarName(QAction *action)
  {
    Q_UNUSED(action);
    return tr("Reaction");
  }

  void ReactionArrowTool::mousePressEvent(QGraphicsSceneMouseEvent *event)
  {
    QPointF downPos = event->buttonDownScenePos(event->button());
    QUndoStack *stack = scene()->stack();

    ReactionArrow *arrow = new ReactionArrow;
    stack->push(new AddItem(arrow, scene()));
    arrow->setPos(downPos);
  }

  void ReactionArrowTool::actionClicked()
  {
    ToolGroup *toolGroup = static_cast<ToolGroup*>(m_action->property("toolGroup").value<void*>());
    if (!toolGroup)
      return;

    toolGroup->setActiveTool(this);
  }

}
