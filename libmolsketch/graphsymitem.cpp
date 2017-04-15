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
#include "molecule.h"

#include <QPainter>
#include <QDebug>
#include <QLibrary>

#include "obabeliface.h"

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

	std::vector<unsigned int> symmetry_classes;
        QLibrary obabeliface("obabeliface" QTVERSIONSUFFIX) ;
	obabeliface.load() ;
	getSymmetryClassesFunctionPointer getSymmetryClassesPtr = (getSymmetryClassesFunctionPointer) obabeliface.resolve("getSymmetryClasses") ;
	if (getSymmetryClassesPtr)
	{
	  getSymmetryClassesPtr(mol, symmetry_classes) ;
	  for (int i = 0; i < atoms.size(); ++i) {
		painter->drawText(mapFromItem(mol, atoms[i]->pos()), QString::number(symmetry_classes.at(i)));
	  }
	}
	else
	  painter->drawText(mapFromItem(mol, mol->graphicalCenterOfMass()), "OpenBabel unavailable") ;

	// default behaviour (draw the label())
	MolInputItem::paint(painter, option, widget);
	painter->restore();
  }

  GraphSymItemFactory theGraphSymItemFactory;

}
