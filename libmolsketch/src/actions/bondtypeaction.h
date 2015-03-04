#ifndef MOLSKETCH_BONDTYPEACTION_H
#define MOLSKETCH_BONDTYPEACTION_H

#include "itemtypeaction.h"

namespace Molsketch {

  class bondTypeAction : public ItemTypeAction
  {
    Q_OBJECT
  public:
    explicit bondTypeAction(MolScene* scene = 0);
  private:
    void applyTypeToItem(graphicsItem *item, int type) const;
    bool getTypeFromItem(graphicsItem *item, int &type) const;
    int defaultType() const;
  };

} // namespace Molsketch

#endif // MOLSKETCH_BONDTYPEACTION_H
