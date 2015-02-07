#ifndef ARROWACTION_H
#define ARROWACTION_H

#include "multiaction.h"

namespace Molsketch {

  class reactionArrowAction : public multiAction
  {
  public:
    explicit reactionArrowAction(MolScene* scene);
    ~reactionArrowAction();
    void mousePressEvent(QGraphicsSceneMouseEvent *event) ;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) ;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) ;
  private:
    class privateData ;
    privateData *d ;
  };

} // namespace

#endif // ARROWACTION_H
