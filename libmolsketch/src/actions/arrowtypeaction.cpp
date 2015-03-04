#include "arrowtypeaction.h"
#include "arrowtypewidget.h"
#include "arrow.h"
#include "commands.h"

namespace Molsketch {

  arrowTypeAction::arrowTypeAction(MolScene* scene)
    : ItemTypeAction(scene)
  {
    setItemTypeWidget(new arrowTypeWidget);
    setText(tr("Arrow tip"));
  }

  arrowTypeAction::~arrowTypeAction()
  {
  }

  Arrow *getArrow(graphicsItem* item)
  {
    if (item->type() != Arrow::Type) return 0;
    Arrow *a = dynamic_cast<Arrow*>(item);
    return a;
  }

  void arrowTypeAction::applyTypeToItem(graphicsItem *item, int type) const
  {
    Arrow *arrow = getArrow(item);
    if (!arrow) return;
    attemptUndoPush(new Commands::SetArrowType(arrow, (Arrow::ArrowType) type));
  }

  bool arrowTypeAction::getTypeFromItem(graphicsItem *item, int &type) const
  {
    Arrow *arrow = getArrow(item);
    if (!arrow) return false;
    type = arrow->getArrowType();
    return true;
  }

  int arrowTypeAction::defaultType() const
  {
    return Arrow::UpperForward | Arrow::LowerForward;
  }

} // namespace Molsketch

