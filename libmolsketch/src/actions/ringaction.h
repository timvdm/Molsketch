#ifndef MOLSKETCH_RINGACTION_H
#define MOLSKETCH_RINGACTION_H

#include "multiaction.h"


namespace Molsketch {

  class ringAction : public multiAction
  {
    Q_OBJECT
  public:
    explicit ringAction(MolScene* scene);
    ~ringAction();

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void leaveSceneEvent(QEvent *event);
    void enterSceneEvent(QEvent *event);

  private:
    class privateData;
    privateData *d;
  private slots:
    void changeRing();
  };

} // namespace Molsketch

#endif // MOLSKETCH_RINGACTION_H
