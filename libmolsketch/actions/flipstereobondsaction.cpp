#include "flipstereobondsaction.h"

#include <bond.h>
#include <commands.h>

namespace Molsketch {

  flipStereoBondsAction::flipStereoBondsAction(Molsketch::MolScene *scene)
    : abstractRecursiveItemAction(scene)
  {
    setMinimumItemCount(1);
    setIcon(QIcon(":images/flip-stereo.png"));
    setText(tr("Flip stereo bond(s)"));
    setCheckable(false);
  }

  void flipStereoBondsAction::execute()
  {
    attemptBeginMacro(tr("flip stereo bonds")); // TODO make sure at least one bond was flipped
    foreach (graphicsItem* item, items())
    {
      Bond *bond = dynamic_cast<Bond*>(item);
      if (item->type() != Bond::Type || !bond)
        continue;
      if (bond->bondType() == Bond::Hash)
        attemptUndoPush(new Commands::SetBondType(bond, Bond::Wedge));
      else if (bond->bondType() == Bond::Wedge)
        attemptUndoPush(new Commands::SetBondType(bond, Bond::Hash));
    }
    attemptEndMacro();
  }

} // namespace Molsketch

