/***************************************************************************
 *   Copyright (C) 2015 Hendrik Vennekate                                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/
#include "incdecaction.h"

#include <QGraphicsSceneMouseEvent>
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
    void (T::*setFunction)(const int&) ;
  public:
    incDecCommand(T* a,
                  bool increment,
                  int (T::*gf)()const,
                  void (T::*sf)(const int&),
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
    void (T::*setFunction)(const int&) ;
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
    if (event->button() != Qt::LeftButton || event->modifiers()) return;
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
                                   void (T::*setFunction)(const int&))
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
    return scene()->atomNear(p) ;
  }

  template<>
  Bond *incDecAction<Bond>::getItem(const QPointF &p)
  {
    return scene()->bondAt(p) ; // TODO use bondNear()
  }

  template <class T>
  T *incDecAction<T>::getItem(const QPointF &p)
  {
    return 0 ;
  }

  chargeAction::chargeAction(MolScene *scene)
    : incDecAction(scene)
  {
    setText(tr("Charge"));
    initialize(QIcon(":images/incCharge.svg"),
               QIcon(":images/decCharge.svg"),
               tr("Increase charge"),
               tr("Decrease charge"),
               &Atom::charge,
               &Atom::setCharge);
  }

  hydrogenAction::hydrogenAction(MolScene *scene)
    : incDecAction(scene)
  {
    setText(tr("Hydrogens"));
    initialize(QIcon(":images/incHydrogens.svg"),
               QIcon(":images/decHydrogens.svg"),
               tr("Add implicit hydrogen"),
               tr("Remove implicit hydrogen"),
               &Atom::numImplicitHydrogens,
               &Atom::setNumImplicitHydrogens);
  }

  template <class T>

  QAction *incDecAction<T>::decrementAction() const
  {
    return d->minusAction;
  }

  template <class T>
  QAction *incDecAction<T>::incrementAction() const
  {
    return d->plusAction;
  }

  // instantiation
  template class incDecAction<Atom> ;
  template class incDecAction<Bond> ; // TODO is this ever used?
} // namespace
