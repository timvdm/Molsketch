#ifndef MOLSKETCH_MECHANISMARROWACTION_H
#define MOLSKETCH_MECHANISMARROWACTION_H

#include "reactionarrowaction.h"



namespace Molsketch {

  class mechanismArrowAction : public reactionArrowAction
  {
  public:
    explicit mechanismArrowAction(MolScene* scene);
    ~mechanismArrowAction();
  private:
    QPolygonF makePolygon(const QLineF &line);
  };

} // namespace Molsketch

#endif // MOLSKETCH_MECHANISMARROWACTION_H
