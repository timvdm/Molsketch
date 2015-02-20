#include "mechanismarrowaction.h"

namespace Molsketch {

  mechanismArrowAction::mechanismArrowAction(MolScene* scene)
    : reactionArrowAction(scene)
  {
    // TODO make arrow action with all arrow tips available
    // TODO arrow action manipulates selected arrow items
    setText(tr("Curved arrow"));
    foreach(QAction* action, findChildren<QAction*>())
    {
      if (action->text() == tr("Single arrow"))
      {
        action->setIcon(QIcon(":images/mechanismarrow.png"));
        action->trigger();
      }
      if (action->text() == tr("Double arrow"))
        action->setIcon(QIcon(":images/curveddoublearrow.png"));
      if (action->text() == tr("Half arrow"))
        action->setIcon(QIcon(":images/curvedhalfarrow.png"));
    }
    setChecked(false);
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

