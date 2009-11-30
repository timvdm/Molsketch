#ifndef MSK_HYDROGENTOOL_H
#define MSK_HYDROGENTOOL_H

#include "../tool.h"

#include <QGraphicsItemGroup>

namespace Molsketch {

  class HydrogenTool : public QObject, public Tool
  {
    Q_OBJECT

    public:
      HydrogenTool(MolScene *scene);
      virtual ~HydrogenTool();

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

#endif // MSK_HYDROGENTOOL_H
