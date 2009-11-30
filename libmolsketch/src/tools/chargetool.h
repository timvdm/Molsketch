#ifndef MSK_CHARGETOOL_H
#define MSK_CHARGETOOL_H

#include "../tool.h"

#include <QGraphicsItemGroup>

namespace Molsketch {

  class ChargeTool : public QObject, public Tool
  {
    Q_OBJECT

    public:
      ChargeTool(MolScene *scene);
      virtual ~ChargeTool();

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

#endif // MSK_CHARGETOOL_H
