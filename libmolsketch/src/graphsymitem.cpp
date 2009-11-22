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
#include "graphsymitem.h"
#include "molscene.h"
#include "mimemolecule.h"

#include <QPainter>
#include <QGraphicsSceneDragDropEvent>
#include <QDebug>

#ifdef OPENBABEL2_TRUNK
#include <openbabel/graphsym.h>
#else
#include <openbabel/mol.h>
#include <openbabel/canon.h>
#endif

namespace Molsketch {

  GraphSymItem::GraphSymItem() : MolInputItem(MoleculeOutput)
  {
  }

  void GraphSymItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
  {
    Molecule *mol = molecule();
    
    painter->save();
    painter->setPen(Qt::red);

    if (!mol) {
      // not connected: default behaviour (draw connectable box)
      MolInputItem::paint(painter, option, widget);
      painter->restore();
      return;
    }

    const QList<Atom*> &atoms = mol->atoms();

    OpenBabel::OBMol *obmol = mol->OBMol();
    std::vector<unsigned int> symmetry_classes;

#ifdef OPENBABEL2_TRUNK      
    OpenBabel::OBGraphSym graphsym(obmol);
    graphsym.GetSymmetry(symmetry_classes);
#else
    OpenBabel::OBBitVec fragatoms(obmol->NumAtoms());

    using OpenBabel::OBMolAtomIter;
    FOR_ATOMS_OF_MOL(a, obmol)
      fragatoms.SetBitOn(a->GetIdx());


    std::vector<unsigned int> canonical_labels;
    OpenBabel::CanonicalLabels(obmol, fragatoms, symmetry_classes, canonical_labels);
#endif

    for (int i = 0; i < atoms.size(); ++i) {
      painter->drawText(mapFromItem(mol, atoms[i]->pos()), QString::number(symmetry_classes.at(i)));
    }

    // default behavious (draw the label())
    MolInputItem::paint(painter, option, widget);
    painter->restore();
  }

  GraphSymItemFactory theGraphSymItemFactory;

}
