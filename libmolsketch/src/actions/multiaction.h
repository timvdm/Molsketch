#ifndef MULTIACTION_H
#define MULTIACTION_H

#include "genericaction.h"

namespace Molsketch {
  class multiAction : public genericAction
  {
    Q_OBJECT
  public:
    explicit multiAction(MolScene *scene) ;
    ~multiAction() ;
  protected:
    void addSubAction(QAction*) ;
    void addSeparator() ;
    QAction* activeSubAction() const ;
  private:
    class privateData ;
    privateData *d ;
  private slots:
    void changeIcon();
  };

} // namespace

#endif // MULTIACTION_H
