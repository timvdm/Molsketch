/***************************************************************************
 *   Copyright (C) 2009 Tim Vandermeersch                                  *
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

#include "atomnumberitem.h"
#include "molscene.h"
#include "mimemolecule.h"

#include <QPainter>
#include <QGraphicsSceneDragDropEvent>
#include <QDebug>

namespace Molsketch {

  AtomNumberItem::AtomNumberItem() : MolInputItem(MoleculeOutput)
  {
  }

  void AtomNumberItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
  {
    Molecule *mol = molecule();

    painter->save();
    painter->setPen(Qt::blue);

    if (!mol) {
      MolInputItem::paint(painter, option, widget);
      return;
    }
    
    int i = 0 ;
    foreach (Atom* atom, mol->atoms())
      drawTextNearAtom(painter, atom, QString::number(++i), mol) ;
    
    MolInputItem::paint(painter, option, widget);
    painter->restore();
  }
  
  AtomNumberItemFactory theAtomNumberItemFactory;

}
