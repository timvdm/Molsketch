/***************************************************************************
 *   Copyright (C) 2009 by Nicola Zonta                                    *
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
#include <queue>
#include "ring.h"


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
		for (int i = 0; i < ats.size (); i++) {
			atoms.push_back (new FFAtom (ats[i]));
		}
		QList <Bond *> bds = molecule ->bonds ();
		for (int i = 0; i < bds.size (); i++) {
			FFBond *b = new FFBond (bds[i]);
			b ->at1 = atoms[bds[i]->beginAtom ()->number()];
			b ->at2 = atoms[bds[i]->endAtom ()->number()];
			bonds.push_back (b);
			interactions.push_back (new FFBondstretch (b->at1, b->at2, bondLength));
			rotations.push_back (new FFBondorient (b->at1, b->at2));

		}	
		
		for (int i = 0; i < ats.size (); i++) {
			Atom *at = ats[i];
			QList <Atom *> nbr = at ->neighbours ();
			for (int j = 0; j < nbr.size (); j++) {
				for (int k = j; k < nbr.size (); k++) {

					if (j == k) continue;
					Atom *at2 = nbr [j];
					Atom *at3 = nbr [k];
					if (at2 == at) continue;
					if (at3 == at) continue;
					interactions.push_back (new FFAngleBend (atoms [at2 ->number()], atoms [at ->number()], atoms [at3 ->number()]));
				}
			}
		}
		
		for (int i = 0; i < ats.size (); i++) {
			for (int j = i; j < ats.size (); j++) {
				bool add = true;
				if (i==j) add = false;
				if (add) {
					Atom *a1 = atoms[i] ->atom;
					Atom *a2 = atoms[j] ->atom;
					foreach (Atom *n, a1 ->neighbours()) {
						if (n == a2) {
							add = false;
							break;
						}
						foreach (Atom *n2, n ->neighbours ()) {
							if (n2 == a2) {
								add = false;
								break;
							}
						}
						if (!add) break;
					}
				}
				if (add) interactions.push_back (new FFclash (atoms[i], atoms[j], bondLength));
	
			}
		}
	}	
	
	void Minimise::run (int n) {
		for (int i = 0; i < n; i++) {
			score_interactions ();
			//	std::cerr << "interactions  " <<total_score()<<std::endl;
			
			if (!move_atoms ()) break;
		}
		for (int j = 0; j < 5; j++) {
			for (unsigned int i = 0; i < 500; i++) {
				score_rotations ();
				//	std::cerr << total_score()<<std::endl;
				
				if (!move_atoms ()) break;
			}
			for (unsigned int i = 0; i < 500; i++) {
				score_interactions ();
		//		std::cerr << total_score()<<std::endl;
				if (!move_atoms ()) break;

				//		if (!move_atoms ()) break;
			}
		}
		for (int i = 0; i < 500; i++) {
			
			//	score_rotations ();
			score_interactions ();
			//	std::cerr << "interactions  " <<total_score()<<std::endl;
			if (!move_atoms ()) break;
		}
		
	}
	
	void Minimise::loadBestPose () {
		for (unsigned int i = 0; i < atoms.size (); i++) {
			atoms[i] ->load_pose ();
		}
	}
	
	
	void Minimise::saveCurrentPose () {
		for (unsigned int i = 0; i < atoms.size (); i++) {
			atoms[i] ->save_pose ();
		}
	}
	
	void Minimise::rotate (qreal angle, QPointF center) {
		for (unsigned int i = 0; i < atoms.size (); i++) {
			qreal x = atoms[i] ->x () - center.x ();
			qreal y = atoms[i] ->y () - center.y ();
			qreal nx = cos(angle) * x - sin(angle) * y;
			qreal ny = sin(angle) * x + cos(angle) * y;
			atoms [i] ->x () = center.x() + nx;
			atoms [i] ->y () = center.y() + ny;
		}
	}
	
	void Minimise::mirror (FFAtom *at1, FFAtom*at2) {
		for (unsigned int i = 0; i < atoms.size (); i++) {
			atoms[i] ->visited = false;
		}
		unsigned int nvisited = 1;
		std::queue <FFAtom *> queue;
		Atom *a1 = at1 ->atom;
		Atom *a2 = at2 ->atom;		
		Bond *b = a1 ->molecule ()->bondBetween (a1, a2);
		if (b ->ring ()) return;
		at1 ->visited = true;
		at2 ->visited = true;

		queue.push (at1);
		while (queue.size ()) {
			FFAtom *a = queue.front ();
			queue.pop ();
			Atom *at = a ->atom;
			foreach (Atom *na, at ->neighbours()) {
				FFAtom *ffna = atoms [na ->number()];
				if (!ffna ->visited) {
					nvisited++;
					ffna ->visited = true;
					queue.push (ffna);
				}
			}
		}
		bool targetVisited = true;
		if (nvisited > atoms.size () /2) targetVisited = false;
	//	std::cerr << "mutate "<<nvisited<<std::endl;

		for (unsigned int i =0; i < atoms.size (); i++) {
			if (atoms [i] ->visited == targetVisited) {
				mirrorAtom (atoms [i], at1, at2);
			}
		}

	}
	
	void Minimise::mirrorAtom (FFAtom *at, FFAtom *at1, FFAtom *at2) {
		QPointF p (at ->qpoint());
		p = symmetric(p, at1 ->qpoint(), at2 ->qpoint());
		at ->x() = p.x();
		at ->y() = p.y ();
		
	}
	
	void Minimise::mutate () {

		qreal r1 = (((qreal) rand()) / RAND_MAX);
		int numberofMutations = r1 * bonds.size () * 0.6;

		if (!numberofMutations) {
			numberofMutations = 1;
			qreal r2 = (((qreal) rand()) / RAND_MAX);
			int ang = r2 * 12;
			qreal angle = ang * M_PI / 6;
			if (atoms.size ()) rotate (angle, atoms[0] ->qpoint ());
		}
		for (int i = 0; i < numberofMutations; i++) {
			qreal r2 = (((qreal) rand()) / RAND_MAX);

			//int bond = r2 * bonds.size ();

			int n = r2 *bonds.size ();
		//	std::cerr << "mutate bond" <<n<<"  ";

			mirror (bonds[n]->at1, bonds[n] ->at2);
		}		
	//	std::cerr << std::endl;
	}
	
	
	void Minimise::fixRings (Molecule *mol) {
		foreach (Ring *r, mol ->rings ()) {
			QList <Atom *> ats = r->atoms ();
			QPointF c = r ->center ();
			
			for (int i = 0; i < ats.size(); i++) { 
				Atom *a = ats[i];
				QPointF rad= a ->pos () - c;
				normalise (rad);
				rad *= bondLength;
				atoms [a ->number()] ->x() = rad.x() + c.x();
				atoms [a ->number()] ->y() = rad.y() + c.y();
			}
		}
	}
	
	void Minimise::conformationalSearchMolecule (Molecule *molecule) {
		initialise (molecule);
		run ();
		fixRings (molecule);
		run ();
		saveCurrentPose();
		bestScore = total_score ();
		for (unsigned int i = 0; i < 500; i++) {
			loadBestPose ();
			mutate ();
			qreal s = total_score();
			//run (5);

	//		std::cerr<<s<<"    "<<bestScore<<std::endl;
			if (s < bestScore) {
				run ();

				bestScore = s;
				saveCurrentPose ();
			}
		}
	 
		finaliseBest ();
		clear ();
	}
	
	
	void Minimise::mirrorBondInMolecule (Molecule *molecule, Bond *bo) {
		initialise (molecule);
		int n = -1;
		for (unsigned int i = 0; i < bonds.size (); i++) {
			if (bonds[i]->bond == bo) {
				n = i;
				break;
			}

		}
		if (n != -1) {
			mirror (bonds[n]->at1, bonds[n] ->at2);

		}
		
		finalise ();
		clear ();
	}

	
	qreal Minimise::total_score () {
		qreal e = 0;
		for (unsigned int i = 0; i < interactions.size (); i++) {
			interactions[i] ->score (e);
		}
		for (unsigned int i = 0; i < rotations.size (); i++) {
			rotations[i] ->score (e);
		}
		e+= elongation_score ();
		return e;
	}
	
	qreal Minimise::elongation_score () {
		qreal score = 0;
		for (unsigned int i = 0; i < atoms.size (); i++) {
				for (unsigned int j = i; j < atoms.size (); j++) {
					if (i == j) continue;
					qreal x = atoms[i] ->x() - atoms[j] ->x();
					qreal y = atoms[i] ->y() - atoms[j] ->y();
					score -= x*x* 2 + y*y * KXSIZE;
				}
		}
		/*
		qreal xmin = 999999;
		qreal xmax = -999999;
		for (unsigned int i = 0; i < atoms.size (); i++) {
			if (atoms[i] ->x () < xmin) xmin = atoms[i] ->x();
			else if (atoms[i] ->x () > xmax) xmax = atoms[i] ->x();
		}
		score += (xmax - xmin) * KXSIZE;
		
		for (unsigned int i = 0; i < bonds.size (); i++) {
			Atom *a1 = bonds[i] ->at1 ->atom;
			Atom *a2 = bonds[i] ->at2 ->atom;
			if (a1 ->neighbours ().size () > 2) continue;
			if (a2 ->neighbours ().size () > 2) continue;
			foreach (Atom *n1, a1 ->neighbours()) {
				if (n1 == a2) continue;
				foreach (Atom *n2, a2 ->neighbours()) {
					if (n2 == a1) continue;
					QPointF v1 = atoms[a1 ->n] ->qpoint () - atoms[n1 ->n] ->qpoint ();
					QPointF v2 = atoms[n2 ->n] ->qpoint () - atoms[a2 ->n] ->qpoint ();
					normalise (v1);
					normalise (v2);
					qreal dotprod =dot (v1, v2);
					if (dotprod < 0) dotprod = -dotprod;
					if (dotprod > 0.85) score += ELONGATION_INCREMENT;
				}
			}
		}
		 */
		 
		return score;
	}
	
	void Minimise::score_rotations () {
		for (unsigned int i = 0; i < rotations.size (); i++) {
			rotations[i] ->apply ();
		}
	}
	
	void Minimise::score_interactions () {
		for (unsigned int i = 0; i < interactions.size (); i++) {
		//	std::cerr << "applying interaction "<<i<<std::endl;
			interactions[i] ->apply ();
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
	}
	void Minimise::finaliseBest () {
		for (unsigned int i = 0; i < atoms.size (); i++) {
			QPointF point (atoms[i]->xbest(), atoms[i]->ybest());
			atoms[i] ->atom ->setPos (point);
		}
	}
	
} //namespace
