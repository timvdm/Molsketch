#ifndef MSK_MECHANISMARROWTOOL_H
#define MSK_MECHANISMARROWTOOL_H

#include "../tool.h"

#include <QGraphicsItemGroup>

namespace Molsketch {

  class MechanismArrowTool : public QObject, public Tool
  {
    Q_OBJECT

    public:
      MechanismArrowTool(MolScene *scene);
      virtual ~MechanismArrowTool();

      QList<QAction*> actions();
      QString toolbarName(QAction *action);

      void mousePressEvent(QGraphicsSceneMouseEvent *event);
      void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
      void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    public slots:
      void actionClicked();

    private:
      // QActions
      QAction *m_action;
      QGraphicsLineItem *m_mechanismArrowHint;
  };

}

#endif // MSK_MECHANISMARROWTOOL_H
