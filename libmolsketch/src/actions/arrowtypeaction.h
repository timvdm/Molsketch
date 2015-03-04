#ifndef MOLSKETCH_ARROWTYPEACTION_H
#define MOLSKETCH_ARROWTYPEACTION_H

#include "itemtypeaction.h"


namespace Molsketch {

  class arrowTypeAction : public ItemTypeAction
  {
    Q_OBJECT
  public:
    explicit arrowTypeAction(MolScene* scene = 0);
    ~arrowTypeAction();
  private:
    void applyTypeToItem(graphicsItem *item, int type) const;
    bool getTypeFromItem(graphicsItem *item, int &type) const;
    int defaultType() const;
  };

} // namespace Molsketch

#endif // MOLSKETCH_ARROWTYPEACTION_H
