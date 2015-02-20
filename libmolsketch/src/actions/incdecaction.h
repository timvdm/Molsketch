#ifndef INCDECACTION_H
#define INCDECACTION_H

#include "multiaction.h"

namespace Molsketch {
  class Atom ;

  template <class T>
  class incDecAction : public multiAction
  {
  public:
    explicit incDecAction(MolScene* scene);
    ~incDecAction() ;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) ;
  protected:
    void initialize(QIcon UpIcon,
                    QIcon DownIcon,
                    QString UpText,
                    QString DownText,
                    int (T::*getFunction)()const,
                    void (T::*setFunction)(int)) ;
  private:
    class privateData;
    privateData *d ;
    T* getItem(const QPointF& p) ;
  };

  class chargeAction : public incDecAction<Atom>
  {
  public:
    explicit chargeAction(MolScene *scene) ;
  } ;

  class hydrogenAction : public incDecAction<Atom>
  {
  public:
    explicit hydrogenAction(MolScene *scene) ;
  } ;
} //namespace

#endif // INCDECACTION_H
