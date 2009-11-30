#ifndef MSK_TOOLGROUP_H
#define MSK_TOOLGROUP_H

#include <QObject>
#include <QList>

class QToolBar;
class QAction;

namespace Molsketch {

  class Tool;
  class MolScene;

  class ToolGroup : public QObject
  {
    Q_OBJECT

    public:
      /**
       * Constructor.
       */
      ToolGroup(MolScene *scene);

      /**
       * Get the tools.
       */
      QList<Tool*> tools() const;
      /**
       * Get the actions from all the tools.
       */
      QList<QAction*> actions() const;
      /**
       * Set the active tool. The deactivated() method will be called for the previously
       * active tool. The activated() method will be called for the new active tool.
       */
      void setActiveTool(Tool *tool);
      /**
       * Get the currently active tool.
       */
      Tool* activeTool() const;


    protected:
      MolScene *m_scene;
      Tool *m_activeTool;
      QList<Tool*> m_tools;
      QList<QAction*> m_actions;
  };

}

#endif // MSK_TOOLGROUP_H
