#ifndef MSK_LASSOTOOL_H
#define MSK_LASSOTOOL_H

#include "../tool.h"

#include <QGraphicsItemGroup>

namespace Molsketch {

  class LassoTool : public QObject, public Tool
  {
    Q_OBJECT

    public:
      LassoTool(MolScene *scene);
      virtual ~LassoTool();

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
      void lassoSelect();

      // QActions
      QAction *m_action;
      //lasso polygon
      QVector<QPointF> m_lassoTrail;
      QGraphicsPolygonItem *m_lassoPolygon;
  };

}

#endif // MSK_LASSOTOOL_H
