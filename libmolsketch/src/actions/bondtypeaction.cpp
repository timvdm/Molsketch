#include "bondtypeaction.h"
#include "bondtypewidget.h"
#include "commands.h"

#include <QMenu>
#include <QVBoxLayout>
#include <QDebug>

namespace Molsketch {

  bondTypeAction::bondTypeAction(MolScene *scene)
    : ItemTypeAction(scene)
  {
    setItemTypeWidget(new bondTypeWidget(false));
    setText(tr("Bond type"));
  }

  Bond *getBond(graphicsItem* item)
  {
    if (item->type() != Bond::Type) return 0;
    Bond *bond = dynamic_cast<Bond*>(item);
    return bond;
  }

  void bondTypeAction::applyTypeToItem(graphicsItem *item, int type) const
  {
    Bond *bond = getBond(item);
    if (!bond) return ;
    attemptUndoPush(new Commands::SetBondType(bond, (Bond::BondType) type));
  }

  bool bondTypeAction::getTypeFromItem(graphicsItem *item, int &type) const
  {
    Bond *bond = getBond(item);
    if (!bond) return false;
    type = bond->bondType();
    return true;
  }

  int bondTypeAction::defaultType() const
  {
    return Bond::Single;
  }

} // namespace Molsketch

