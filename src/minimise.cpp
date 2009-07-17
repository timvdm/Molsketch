/***************************************************************************
 *   Copyright (C) 2009 by Nicola Zonta                               *
 *                                                                      *
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

#include "minimise.h"

namespace Molsketch {
	void Minimise::minimiseMolecule (Molecule *molecule) {
		initialise (molecule);
		run ();
		finalise ();
		clear ();
	}
	
	void Minimise::initialise (Molecule *molecule) {
		clear ();
		molecule ->numberAtoms ();
		QList <Atom *> ats = molecule ->atoms ();
		for (unsigned int i = 0; i < ats.size (); i++) {
			atoms.push_back (new FFAtom (ats[i]));
		}
		QList <Bond *> bds = molecule ->bonds ();
		for (unsigned int i = 0; i < bds.size (); i++) {
			interactions.push_back (new FFBondstretch (atoms[bds[i]->beginAtom ()->n], atoms[bds[i]->endAtom ()->n]));
			rotations.push_back (new FFBondorient (atoms[bds[i]->beginAtom ()->n], atoms[bds[i]->endAtom ()->n]));

		}	
		
		for (unsigned int i = 0; i < ats.size (); i++) {
			Atom *at = ats[i];
			QList <Atom *> nbr = at ->neighbours ();
			for (unsigned int j = 0; j < nbr.size (); j++) {
				for (unsigned int k = j; k < nbr.size (); k++) {

					if (j == k) continue;
					Atom *at2 = nbr [j];
					Atom *at3 = nbr [k];
					if (at2 == at) continue;
					if (at3 == at) continue;
					interactions.push_back (new FFAngleBend (atoms [at2 ->n], atoms [at ->n], atoms [at3 ->n]));
				}
			}
		}
		
		for (unsigned int i = 0; i < ats.size (); i++) {
			for (unsigned int j = i; j < ats.size (); j++) {
				if (i==j) continue;
				interactions.push_back (new FFclash (atoms[i], atoms[j]));

			}
		}
	}	
	
	void Minimise::run () {
		for (unsigned int j = 0; j < 20; j++) {
			for (unsigned int i = 0; i < 1000; i++) {
				score_rotations ();
				if (!move_atoms ()) break;
			}
			for (unsigned int i = 0; i < 1000; i++) {
				score_interactions ();
				if (!move_atoms ()) break;
			}
		}
	}
	

	
	void Minimise::score_interactions () {
		for (unsigned int i = 0; i < interactions.size (); i++) {
			interactions[i] ->apply ();
		}
	}
	
	void Minimise::score_rotations () {
		for (unsigned int i = 0; i < rotations.size (); i++) {
			rotations[i] ->apply ();
		}
	}
	
	bool Minimise::move_atoms () {
		qreal x = 0;
		qreal y = 0;
		qreal f = 0;
		for (unsigned int i = 0; i < atoms.size (); i++) {
			atoms[i] ->x () += atoms[i] ->force_x ();
			atoms[i] ->y () += atoms[i] ->force_y ();
			x = atoms [i] ->force_x ();
			y = atoms [i] ->force_y ();	
			f += x*x + y*y;
		//	std::cerr << atoms[i] ->force_x () << "  " << atoms[i] -> force_y () << std::endl;

			atoms [i] ->force_x () = 0;
			atoms [i] ->force_y () = 0;
			

		}

		return (f > 0.0005);
	}
	void Minimise::finalise () {
		for (unsigned int i = 0; i < atoms.size (); i++) {
			QPointF point (atoms[i]->x(), atoms[i]->y());
			atoms[i] ->atom ->setPos (point);
		}
		clear ();
	}
	
} //namespace