#ifndef MOLSKETCH_PLUGINACTION_H
#define MOLSKETCH_PLUGINACTION_H

#include "multiaction.h"

namespace Molsketch {

  class pluginAction : public multiAction
  {
  public:
    explicit pluginAction(MolScene* scene);
    ~pluginAction();
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
  private:
    class privateData;
    privateData *d;
  };

} // namespace Molsketch

#endif // MOLSKETCH_PLUGINACTION_H
