#include "flipbondaction.h"

#include "bond.h"
#include "commands.h"

namespace Molsketch {

  flipBondAction::flipBondAction(MolScene *scene)
    : abstractItemAction(scene)
  {
    setMinimumItemCount(1);
    setIcon(QIcon(":images/flipbond.png"));
    setText(tr("Flip bond"));
  }

  void flipBondAction::execute()
  {
    attemptBeginMacro(tr("flip bond"));
    foreach(graphicsItem* item, items())
    {
      Bond *bond = dynamic_cast<Bond*>(item);
      if (item->type() != Bond::Type || !bond) continue;
      attemptUndoPush(new Commands::SwapBondAtoms(bond));
    }

    attemptEndEndMacro();
  }

} // namespace Molsketch

