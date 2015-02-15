#include "mechanismarrowaction.h"

namespace Molsketch {

  mechanismArrowAction::mechanismArrowAction(MolScene* scene)
    : reactionArrowAction(scene)
  {
    // TODO icons
  }

  mechanismArrowAction::~mechanismArrowAction()
  {
  }

  QPolygonF mechanismArrowAction::makePolygon(const QLineF &line)
  {
    return QPolygonF() << line.p1() << line.normalVector().p2()
                       << line.normalVector().translated(line.p2()-line.p1()).p2() << line.p2();
  }

} // namespace Molsketch

