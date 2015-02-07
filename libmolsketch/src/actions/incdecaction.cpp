#include "incdecaction.h"

#include <QUndoCommand>
#include "atom.h"
#include "bond.h"
#include "molscene.h"

namespace Molsketch {

  template<class T>
  class incDecCommand : public QUndoCommand
  {
  private:
    T* t ;
    bool plus ;
    int (T::*getFunction)()const ;
    void (T::*setFunction)(int) ;
  public:
    incDecCommand(T* a,
                  bool increment,
                  int (T::*gf)()const,
                  void (T::*sf)(int),
                  QString text,
                  QUndoCommand* parent = 0)
      : QUndoCommand(text, parent),
        t(a),
        plus(increment),
        getFunction(gf),
        setFunction(sf)
    {
    }

    void redo()
    {
      (t->*setFunction)((t->*getFunction)() + (plus ? 1 : -1)) ;
    }
    void undo()
    {
      (t->*setFunction)((t->*getFunction)() + (plus ? -1 : 1)) ;
    }
    // TODO mergeable
  };

  template <class T>
  struct incDecAction<T>::privateData
  {
    privateData() :
      plusAction(0),
      minusAction(0)
    {}
    void clear()
    {
      delete plusAction ;
      plusAction = 0 ;
      delete minusAction ;
      minusAction = 0 ;
    }
    QAction *plusAction ;
    QAction *minusAction ;
    int (T::*getFunction)() const;
    void (T::*setFunction)(int) ;
  };

  template <class T>
  incDecAction<T>::incDecAction(MolScene *scene)
    : multiAction(scene),
      d(new privateData)
  {}

  template <class T>
  incDecAction<T>::~incDecAction()
  {
    delete d ;
  }

  template <class T>
  void incDecAction<T>::mousePressEvent(QGraphicsSceneMouseEvent *event)
  {
    // Check possible targets
    T* t = getItem(event->buttonDownScenePos(event->button()));
    if (!t) return;

    undoStack()->push(new incDecCommand<T>(t,
                                        activeSubAction() == d->plusAction,
                                        d->getFunction,
                                        d->setFunction,
                                        activeSubAction()->text())) ;
  }

  template <class T>
  void incDecAction<T>::initialize(QIcon UpIcon,
                                   QIcon DownIcon,
                                   QString UpText,
                                   QString DownText,
                                   int (T::*getFunction)()const,
                                   void (T::*setFunction)(int))
  {
    // TODO remove old actions
    d->clear();
    d->plusAction = new QAction(UpIcon, UpText, this) ;
    d->minusAction = new QAction(DownIcon, DownText, this) ;
    addSubAction(d->plusAction);
    addSubAction(d->minusAction);

    d->setFunction = setFunction ;
    d->getFunction = getFunction ;
  }

  template<>
  Atom *incDecAction<Atom>::getItem(const QPointF &p)
  {
    return scene()->atomAt(p) ;
  }

  template<>
  Bond *incDecAction<Bond>::getItem(const QPointF &p)
  {
    return scene()->bondAt(p) ;
  }

  template <class T>
  T *incDecAction<T>::getItem(const QPointF &p)
  {
    return 0 ;
  }

  chargeAction::chargeAction(MolScene *scene)
    : incDecAction(scene)
  {
    initialize(QIcon(":images/incCharge.png"),
               QIcon(":images/decCharge.png"),
               tr("Increase charge"),
               tr("Decrease charge"),
               &Atom::charge,
               &Atom::setCharge);
  }

  hydrogenAction::hydrogenAction(MolScene *scene)
    : incDecAction(scene)
  {
    initialize(QIcon(":images/incHydrogens.png"),
               QIcon(":images/decHydrogens.png"),
               tr("Add implicit hydrogen"),
               tr("Remove implicit hydrogen"),
               &Atom::numImplicitHydrogens,
               &Atom::setNumImplicitHydrogens);
  }

  // instantiation
  template class incDecAction<Atom> ;
  template class incDecAction<Bond> ;


} // namespace
