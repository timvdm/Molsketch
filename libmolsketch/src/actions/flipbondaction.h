#ifndef MOLSKETCH_FLIPBONDACTION_H
#define MOLSKETCH_FLIPBONDACTION_H

#include "abstractitemaction.h"



namespace Molsketch {

  class flipBondAction : public abstractItemAction
  {
    Q_OBJECT
  public:
    explicit flipBondAction(MolScene *scene = 0);
  private:
    void execute();
  };

} // namespace Molsketch

#endif // MOLSKETCH_FLIPBONDACTION_H
