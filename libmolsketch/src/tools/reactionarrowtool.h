#ifndef MSK_REACTIONARROWTOOL_H
#define MSK_REACTIONARROWTOOL_H

#include "../tool.h"

#include <QGraphicsItemGroup>

namespace Molsketch {

  class ReactionArrowTool : public QObject, public Tool
  {
    Q_OBJECT

    public:
      ReactionArrowTool(MolScene *scene);
      virtual ~ReactionArrowTool();

      QList<QAction*> actions();
      QString toolbarName(QAction *action);

      void mousePressEvent(QGraphicsSceneMouseEvent *event);


    public slots:
      void actionClicked();

    private:
      // QActions
      QAction *m_action;

  };

}

#endif // MSK_REACTIONARROWTOOL_H
