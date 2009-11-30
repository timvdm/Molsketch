#ifndef MSK_ROTATETOOL_H
#define MSK_ROTATETOOL_H

#include "../tool.h"

#include <QGraphicsItemGroup>

namespace Molsketch {

  class RotateTool : public QObject, public Tool
  {
    Q_OBJECT

    public:
      RotateTool(MolScene *scene);
      virtual ~RotateTool();

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
      //Item that is being rotated
      QGraphicsItem *m_rotationItem;
      /** Used for rotations. Stores the last vector from mouse pointer to center of rotation */
      QPointF m_lastRotationVect;
      QPointF  m_rotatePointAbs;
      QPointF m_rotationCenter;
  };

}

#endif // MSK_ROTATETOOL_H
