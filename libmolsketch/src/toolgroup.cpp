#include "toolgroup.h"

#include <QToolBar>
#include <QDebug>

#include "tool.h"
#include "tools/drawtool.h"
#include "tools/movetool.h"
#include "tools/lassotool.h"
#include "tools/rotatetool.h"
#include "tools/minimizetool.h"
#include "tools/connecttool.h"
#include "tools/hydrogentool.h"
#include "tools/chargetool.h"
#include "tools/electrontool.h"
#include "tools/reactionarrowtool.h"
#include "tools/mechanismarrowtool.h"


namespace Molsketch {

  ToolGroup::ToolGroup(MolScene *scene) : QObject()
  {
    m_scene = scene;
    QActionGroup *actionGroup = new QActionGroup(this);

    m_tools.append(new DrawTool(m_scene));
    m_tools.append(new MoveTool(m_scene));
    m_tools.append(new LassoTool(m_scene));
    m_tools.append(new RotateTool(m_scene));
    m_tools.append(new MinimizeTool(m_scene));
    m_tools.append(new ConnectTool(m_scene));
    m_tools.append(new ChargeTool(m_scene));
    m_tools.append(new HydrogenTool(m_scene));
    m_tools.append(new ReactionArrowTool(m_scene));
    m_tools.append(new MechanismArrowTool(m_scene));


    foreach(Tool *tool, m_tools) {
      foreach(QAction *action, tool->actions()) {
        action->setProperty("tool", qVariantFromValue((void*)tool));
        action->setProperty("toolGroup", qVariantFromValue((void*)this));

        action->setCheckable(true);
        actionGroup->addAction(action);
        m_actions.append(action);
      }
    }

    m_activeTool = m_tools.first();
  }

  QList<Tool*> ToolGroup::tools() const
  {
    return m_tools;
  }

  void ToolGroup::setActiveTool(Tool *tool)
  {
    m_activeTool->deactivated();
    m_activeTool = tool;
    m_activeTool->activated();
  }

  Tool* ToolGroup::activeTool() const
  {
    return m_activeTool;
  }

  QList<QAction*> ToolGroup::actions() const
  {
    return m_actions;
  }

}
