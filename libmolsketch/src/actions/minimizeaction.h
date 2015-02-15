#ifndef MOLSKETCH_MINIMIZEACTION_H
#define MOLSKETCH_MINIMIZEACTION_H

#include "genericaction.h"

namespace Molsketch {

  class minimizeAction : public genericAction
  {
    Q_OBJECT
  public:
    explicit minimizeAction(MolScene* scene);
    void mousePressEvent(QGraphicsSceneMouseEvent *event) ;
  };

} // namespace Molsketch

#endif // MOLSKETCH_MINIMIZEACTION_H
