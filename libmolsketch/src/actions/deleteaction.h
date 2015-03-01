#ifndef MOLSKETCH_DELETEACTION_H
#define MOLSKETCH_DELETEACTION_H

#include "genericaction.h"



namespace Molsketch {

  class deleteAction : public genericAction
  {
    Q_OBJECT
  public:
    explicit deleteAction(MolScene* scene);
    ~deleteAction();
  private slots:
    void deleteSelection();
  };

} // namespace Molsketch

#endif // MOLSKETCH_DELETEACTION_H
