#ifndef MSK_TOOL_H
#define MSK_TOOL_H

#include <QGraphicsSceneMouseEvent>
#include <QAction>

namespace Molsketch {

  class MolScene;

  class Tool
  {
    public:

      /**
       * Constructor.
       */
      Tool(MolScene *scene);
      virtual ~Tool();

      /**
       * Get the scene, this can only be specified in the constructor.
       */
      MolScene* scene() const;

      /**
       * Get a list of all actions for this tool.
       */
      virtual QList<QAction*> actions() = 0;
      /**
       * Get the toolbar name for the specified action.
       */
      virtual QString toolbarName(QAction *action) = 0;

      /**
       * This method is called when a tool is activated. The default implementation does nothing.
       */
      virtual void activated();
      /**
       * This method is called when a tool is deactivated. The default implementation does nothing.
       */
      virtual void deactivated();

      /**
       * Reimplement this function to handle mouse press events. The default implementation does nothing.
       */
      virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
      /**
       * Reimplement this function to handle mouse move events. The default implementation does nothing.
       */
      virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
      /**
       * Reimplement this function to handle mouse release events. The default implementation does nothing.
       */
      virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
      /**
       * Reimplement this function to handle mouse double click events. The default implementation does nothing.
       */
      virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

    protected:
      MolScene *m_scene;
  };

}

#endif // MSK_TOOL_H
