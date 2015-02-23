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
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
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
