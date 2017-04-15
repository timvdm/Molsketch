#ifndef FLIPSTEREOBONDSACTION_H
#define FLIPSTEREOBONDSACTION_H

#include "abstractrecursiveitemaction.h"

namespace Molsketch {

  class flipStereoBondsAction : public abstractRecursiveItemAction
  {
    Q_OBJECT
  public:
    explicit flipStereoBondsAction(MolScene *scene = 0);
  private:
    void execute();
  };

} //namespace Molsketch

#endif // FLIPSTEREOBONDSACTION_H
