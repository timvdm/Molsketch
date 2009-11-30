#ifndef MSK_MOVETOOL_H
#define MSK_MOVETOOL_H

#include "../tool.h"

#include <QGraphicsItemGroup>

namespace Molsketch {

  class MoveTool : public QObject, public Tool
  {
    Q_OBJECT

    public:
      MoveTool(MolScene *scene);
      virtual ~MoveTool();

      QList<QAction*> actions();
      QString toolbarName(QAction *action);

      void activated();
      void deactivated();

      void mousePressEvent(QGraphicsSceneMouseEvent *event);
      void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
      void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    public slots:
      void actionClicked();

    private:
      // QActions
      QAction *m_action;
  };

}

#endif // MSK_MOVETOOL_H
