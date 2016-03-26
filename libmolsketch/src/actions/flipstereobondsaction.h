#ifndef FLIPSTEREOBONDSACTION_H
#define FLIPSTEREOBONDSACTION_H

#include "abstractitemaction.h"

namespace Molsketch {

  class flipStereoBondsAction : public abstractItemAction
  {
    Q_OBJECT
  public:
    explicit flipStereoBondsAction(MolScene *scene = 0);
  private:
    void execute();
  };

} //namespace Molsketch

#endif // FLIPSTEREOBONDSACTION_H
