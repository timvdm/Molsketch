#ifndef MOLSKETCH_BONDTYPEACTION_H
#define MOLSKETCH_BONDTYPEACTION_H

#include "abstractitemaction.h"

namespace Molsketch {

  class bondTypeAction : public abstractItemAction
  {
    Q_OBJECT
  public:
    explicit bondTypeAction(MolScene* scene = 0);
    ~bondTypeAction();
  private:
    class privateData;
    privateData *d;
    void execute();
  private slots:
    void checkBondType();
  };

} // namespace Molsketch

#endif // MOLSKETCH_BONDTYPEACTION_H
