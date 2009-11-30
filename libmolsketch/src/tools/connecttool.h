#ifndef MSK_CONNECTTOOL_H
#define MSK_CONNECTTOOL_H

#include "../tool.h"

#include <QGraphicsItemGroup>

namespace Molsketch {

  class ConnectTool : public QObject, public Tool
  {
    Q_OBJECT

    public:
      ConnectTool(MolScene *scene);
      virtual ~ConnectTool();

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

#endif // MSK_CONNECTTOOL_H
