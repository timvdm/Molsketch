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
#include "stereocenteritem.h"
#include "molscene.h"
#include "mimemolecule.h"

#include <QPainter>
#include <QGraphicsSceneDragDropEvent>
#include <QDebug>

#ifdef OPENBABEL2_TRUNK
#include <openbabel/stereo/stereo.h>
#include <openbabel/graphsym.h>
#include <openbabel/mol.h>
#else
#include <openbabel/mol.h>
#endif

namespace Molsketch {

  StereoCenterItem::StereoCenterItem() : MolInputItem(MoleculeOutput)
  {
  }

  void StereoCenterItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
  {
    Molecule *mol = molecule();
    
    painter->save();
    painter->setPen(Qt::green);

    if (!mol) {
      // not connected: default behaviour (draw connectable box)
      MolInputItem::paint(painter, option, widget);
      painter->restore();
      return;
    }

    const QList<Atom*> &atoms = mol->atoms();
    OpenBabel::OBMol *obmol = mol->OBMol();
    QPointF offset(-5.0, 5.0);

#ifdef OPENBABEL2_TRUNK
    // need to calculate symmetry first
    std::vector<unsigned int> symmetry_classes;
    OpenBabel::OBGraphSym graphsym(obmol);
    graphsym.GetSymmetry(symmetry_classes);

    //std::vector<unsigned long> atomIds = FindTetrahedralAtoms(obmol, symmetry_classes);
    OpenBabel::OBStereoUnitSet units = FindStereogenicUnits(obmol, symmetry_classes);
    
    for (unsigned int i = 0; i < units.size(); ++i) {
      if (units.at(i).type == OpenBabel::OBStereo::Tetrahedral) {
        OpenBabel::OBAtom *obatom = obmol->GetAtomById(units.at(i).id);
        painter->drawEllipse(mapFromItem(mol, atoms[obatom->GetIndex()]->pos()), 10, 10);
      } else 
      if (units.at(i).type == OpenBabel::OBStereo::CisTrans) {
        OpenBabel::OBBond *obbond = obmol->GetBondById(units.at(i).id);
        OpenBabel::OBAtom *obatom1 = obbond->GetBeginAtom();
        OpenBabel::OBAtom *obatom2 = obbond->GetEndAtom();
        painter->drawEllipse(mapFromItem(mol, atoms[obatom1->GetIndex()]->pos()), 10, 10);
        painter->drawEllipse(mapFromItem(mol, atoms[obatom2->GetIndex()]->pos()), 10, 10);
      } 
 
    }
#else
    using OpenBabel::OBMolAtomIter;
    FOR_ATOMS_OF_MOL(atom, obmol)
      if (atom->IsChiral())
        painter->drawEllipse(mapFromItem(mol, atoms[atom->GetIdx()-1]->pos()), 10, 10);
#endif

    // default behavious (draw the label())
    MolInputItem::paint(painter, option, widget);
    painter->restore();
  }
  
  StereoCenterItemFactory theStereoCenterItemFactory;

}
