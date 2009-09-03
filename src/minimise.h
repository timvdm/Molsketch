/***************************************************************************
 *   Copyright (C) 2009 by Nicola Zonta                               *
 *                                            *
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

#ifndef MINIMISE_H
#define MINIMISE_H

#define KSTRETCH 0.2
#define KCLASH 1
#define ELONGATION_INCREMENT -5
#define KXSIZE -0.1

#define KBEND 2
#define KORIENT 1


#include "atom.h"
#include "bond.h"

#include "molecule.h"
#include <cmath>
#include <iostream>
#include <assert.h>
#include <math.h>

//contains both a forcefield and a minimisation algorithm. 

namespace Molsketch {
	
	
	//FFAtom	
	class FFAtom {
	public: 
		FFAtom (Atom *at) {atom = at; x() = at ->pos ().x(); y() = at ->pos ().y(); _force_x =0; _force_y = 0; save_pose (); visited = false;}
		qreal &x () {return _x;}
		qreal &y () {return _y;}
		qreal &xbest () {return _bestx;}
		qreal &ybest () {return _besty;}
		qreal &force_x () {return _force_x;}
		qreal &force_y () {return _force_y;}
		void save_pose () {_bestx = _x; _besty = _y;}
		void load_pose () {_x = _bestx; _y = _besty;}

		QPoint qpoint () {return QPoint (x(), y());}
		Atom *atom;
		bool visited;


			
	private:
		qreal _x;
		qreal _y;
		qreal _force_x;
		qreal _force_y;
		qreal _bestx;
		qreal _besty;
	};
	class FFBond {
	public: 
		FFBond (Bond *bo) {bond =bo; at1 = 0; at2 = 0;}
		FFAtom *at1;
		FFAtom *at2;
		Bond *bond;
	};
	
	
	static qreal module (FFAtom &p) {
		return std::sqrt (p.x()*p.x() + p.y()*p.y());
	}
	
	static qreal square_distance (FFAtom &p1, FFAtom &p2) {
		return (p2.x ()-p1.x ())*(p2.x ()-p1.x ()) + (p2.y ()-p1.y ())*(p2.y ()-p1.y ());
	}
	
	static qreal distance (FFAtom &p1, FFAtom &p2) {
		return std::sqrt (square_distance (p1, p2));
	}
	
	static QPointF vect (FFAtom &p1, FFAtom &p2) {
		return p2.qpoint () - p1.qpoint ();
	}
	
	static QPointF symmetric (QPointF p, QPointF a, QPointF b) {
		QPointF diff = b - a;
		if (diff.x () == 0) {
			return QPointF (2*a.x() - p.x (), p.y ());
		}
		else if (diff.y () == 0) {
				return QPointF ( p.x (), 2*a.y() - p.y ());
		}
		else {
		qreal m = diff.y () / diff.x ();
		qreal q = a.y() - m * a.x ();
		// perpendicular line y = -1/m * x + q2
		qreal q2 = p.y () + p.x()/m;
		//intersecting y = mx +q and y = -1/m*x + q2
		qreal x = (q2 - q) / (m + 1/m);
		qreal y = m *x + q;
		return QPointF (2*x-p.x(), 2*y-p.y());
		}	
		return p;
	}
	
	
	static qreal angle (FFAtom &p1, FFAtom &p2, FFAtom &p3) {
		QPointF vec1 = vect (p2, p1);
		QPointF vec2 = vect (p2, p3);
		qreal crossprod = vec1.x () * vec2.y () - vec1.y () * vec2.x ();
		qreal dotprod =   vec1.x () * vec2.x () + vec1.y () * vec2.y ();
		return std::atan2 (crossprod, dotprod);
	}
	

	
	static void multiply (QPointF &p, qreal k) {
		qreal x = p.x();
		qreal y = p.y();
		p.setX(x*k);
		p.setY(y*k);
	}
	
	
	static void normalise (QPointF &p) {
		qreal k = 1/ sqrt (p.x()*p.x() + p.y()*p.y());
		multiply(p, k);

	}
	
	static qreal dot (QPointF &vec1, QPointF &vec2) {
		return vec1.x () * vec2.x () + vec1.y () * vec2.y ();
	} 
	
	//general class for forcefield interaction
	class FFInteraction {
	public:
		FFInteraction (FFAtom *at1, FFAtom *at2) :p1 (at1), p2 (at2) {}
                virtual ~FFInteraction() {}

	//	FFInteraction (): p1 (NULL), p2 (NULL) {};
		virtual void apply () = 0;
		virtual void score (qreal &score) = 0;

	protected:
		FFAtom *p1, *p2;
	};
	
	//bond lengths
	class FFBondstretch : public FFInteraction {
	public:
		FFBondstretch (FFAtom *at1, FFAtom *at2, qreal length = 40) : FFInteraction(at1, at2), len (length) {}
		void apply () {
			assert (!std::isnan (p1 ->force_x()));
			assert (!std::isnan (p1 ->force_y()));
			assert (!std::isnan (p2 ->force_x()));
			assert (!std::isnan (p2 ->force_y()));
			qreal dist = distance (*p1, *p2);
			qreal d = dist - len;
			QPointF v = vect (*p1, *p2);
			normalise (v);
			multiply (v, d * KSTRETCH);
			QPointF v2 (-v.x(), -v.y());

			p1 ->force_x () += v.x();
			p1 ->force_y () += v.y();
			
			p2 ->force_x () += v2.x();
			p2 ->force_y () += v2.y();
			assert (!std::isnan (p1 ->force_x()));
			assert (!std::isnan (p1 ->force_y()));
			assert (!std::isnan (p2 ->force_x()));
			assert (!std::isnan (p2 ->force_y()));
		}
		qreal len;

	void score (qreal &tot) {
		qreal dist = distance (*p1, *p2);
		qreal d = dist - len;
		tot += d*d*KSTRETCH;
		
	}
			
};
	
	//clashes
	class FFclash : public FFInteraction {
	public:
		FFclash (FFAtom *at1, FFAtom *at2, qreal length = 40) : FFInteraction(at1, at2), len (length) {}
		void apply () {
			assert (!std::isnan (p1 ->force_x()));
			assert (!std::isnan (p1 ->force_y()));
			assert (!std::isnan (p2 ->force_x()));
			assert (!std::isnan (p2 ->force_y()));
			qreal dist = distance (*p1, *p2);
			qreal d = len - dist;
			if (d < len / 4) return;
			QPointF v = vect (*p1, *p2);
			normalise (v);
			multiply (v, d * KCLASH);
			QPointF v2 (-v.x(), -v.y());

			p2 ->force_x () += v.x();
			p2 ->force_y () += v.y();
			
			p1 ->force_x () += v2.x();
			p1 ->force_y () += v2.y();
			assert (!std::isnan (p1 ->force_x()));
			assert (!std::isnan (p1 ->force_y()));
			assert (!std::isnan (p2 ->force_x()));
			assert (!std::isnan (p2 ->force_y()));
			
		}
		void score (qreal &tot) {
			qreal dist = distance (*p1, *p2);
			qreal d = len - dist;
			if (d < len /4) return;
			tot += d*d*KCLASH*10;
			
		}
		qreal len;
	};
	
	
	class FFBondorient : public FFInteraction {
	public:
		FFBondorient (FFAtom *at1, FFAtom *at2) : FFInteraction(at1, at2) {}
		void apply () {
			assert (!std::isnan (p1 ->force_x()));
			assert (!std::isnan (p1 ->force_y()));
			assert (!std::isnan (p2 ->force_x()));
			assert (!std::isnan (p2 ->force_y()));
			QPointF v = vect (*p1, *p2);
			qreal ang = atan2 (v.y(), v.x());
			qreal targetang;
			if (ang < 0) ang = 2 * M_PI + ang;
//			if (ang >= 0) {
				int n = ang / (M_PI / 6);
				targetang = n * (M_PI / 6);
				if ((ang - targetang) > (M_PI / 12)) targetang = (n+1) * (M_PI / 6);
//			}
//			else {
//				int n = ang / (M_PI / 3);
//				targetang = n * (M_PI / 3);
//				if ((targetang - ang) > (M_PI / 6)) targetang = (n-1) * (M_PI / 3);
//			}
			
			qreal a = targetang - ang;
			normalise (v);
			
			QPointF n1 (v.y(), -v.x());
			

			
			multiply(n1, a * KORIENT);
			
			p1 ->force_x () += n1.x();
			p1 ->force_y () += n1.y();			
			
			p2 ->force_x () -= n1.x();
			p2 ->force_y () -= n1.y();		
			assert (!std::isnan (p1 ->force_x()));
			assert (!std::isnan (p1 ->force_y()));
			assert (!std::isnan (p2 ->force_x()));
			assert (!std::isnan (p2 ->force_y()));
			
		}
		void score (qreal &tot) {
			QPointF v = vect (*p1, *p2);
			qreal ang = atan2 (v.y(), v.x());
			qreal targetang;
			if (ang < 0) ang = 2 * M_PI + ang;
			//			if (ang >= 0) {
			int n = ang / (M_PI / 6);
			targetang = n * (M_PI / 6);
			if ((ang - targetang) > (M_PI / 12)) targetang = (n+1) * (M_PI / 6);
			//			}
			//			else {
			//				int n = ang / (M_PI / 3);
			//				targetang = n * (M_PI / 3);
			//				if ((targetang - ang) > (M_PI / 6)) targetang = (n-1) * (M_PI / 3);
			//			}
			
			qreal a = targetang - ang;
			tot += a*a*KORIENT;
		}
	};
	

	class FFAngleBend : public FFInteraction {
	public:
		FFAngleBend (FFAtom *at1, FFAtom *at2, FFAtom *at3) : FFInteraction(at1, at2), p3 (at3) {}

		void apply () {
			assert (!std::isnan (p1 ->force_x()));
			assert (!std::isnan (p1 ->force_y()));
			assert (!std::isnan (p2 ->force_x()));
			assert (!std::isnan (p2 ->force_y()));
			assert (!std::isnan (p3 ->force_x()));
			assert (!std::isnan (p3 ->force_y()));
			qreal ang = angle (*p1, *p2, *p3);
			if (ang < 0) ang = -ang;
			qreal a = (2* M_PI / 3) - ang;
			QPointF v = vect (*p2, *p1);
			QPointF v2 = vect (*p2, *p3);
			normalise (v);
			normalise (v2);
			QPointF dir = v + v2;
			
			QPointF n1 (v.y(), -v.x());
			if (dot (n1, dir) > 0)  multiply(n1, -1);
			
			QPointF n2 (v2.y(), -v2.x());
			if (dot (n2, dir) > 0) multiply(n2, -1);
			
			multiply(n1, a * KBEND);
			multiply(n2, a * KBEND);
	//		std::cerr << ang*180/M_PI<< " " << a*180/M_PI << " "<< n1.x()<<"  "<<n1.y()<< " "<<n2.x()<<" " <<n2.y ()<<std::endl;

			p1 ->force_x () += n1.x();
			p1 ->force_y () += n1.y();
			
			p3 ->force_x () += n2.x();
			p3 ->force_y () += n2.y();
			
			p2 ->force_x () -= n1.x();
			p2 ->force_y () -= n1.y();
			
			p2 ->force_x () -= n2.x();
			p2 ->force_y () -= n2.y();
			assert (!std::isnan (p1 ->force_x()));
			assert (!std::isnan (p1 ->force_y()));
			assert (!std::isnan (p2 ->force_x()));
			assert (!std::isnan (p2 ->force_y()));
			assert (!std::isnan (p3 ->force_x()));
			assert (!std::isnan (p3 ->force_y()));
			
			
		}
		void score (qreal &tot) {
			qreal ang = angle (*p1, *p2, *p3);
			if (ang < 0) ang = -ang;
      //qreal a = (2* M_PI / 3) - ang;
		}
		FFAtom *p3;

	};
	
	//class to adjust the geometry of 2D molecules and scenes
	class Minimise  {
	public:
		Minimise (qreal bl=40) : bondLength (bl) { srand((unsigned)time(0)); };
		~Minimise () {clear ();}
		void initialise (Molecule *molecule);
		void minimiseMolecule (Molecule *molecule);
		void conformationalSearchMolecule (Molecule *molecule);
		void mirrorBondInMolecule (Molecule *molecule, Bond *bo);
		void run (int n = 500);
		void mutate ();
		void finalise ();
		void finaliseBest ();
		void score_interactions ();
		void score_rotations ();
		void saveCurrentPose ();
		void loadBestPose ();

		bool move_atoms ();
		qreal total_score ();
		qreal elongation_score ();
		

	private:
		void fixRings (Molecule *mol);

		void mirror (FFAtom *at1, FFAtom*at2);
		void rotate (qreal angle, QPointF center);
		void mirrorAtom (FFAtom *a, FFAtom *at1, FFAtom *at2);
		qreal bestScore;
		qreal bondLength;
		void clear() {
			for (unsigned int i=0; i < atoms.size (); i++) delete atoms[i];
			for (unsigned int i=0; i < bonds.size (); i++) delete bonds[i];

			for (unsigned int i=0; i < interactions.size (); i++) delete interactions[i];	
			for (unsigned int i=0; i < rotations.size (); i++) delete rotations[i];	

			atoms.clear ();
			bonds.clear ();
			interactions.clear ();
			rotations.clear ();
		}
			
		std::vector <FFAtom *> atoms;
		std::vector <FFBond *> bonds;

		std::vector <FFInteraction *> interactions;
		std::vector <FFInteraction *> rotations;

		
		
	};
	
	
	

						  
						  
} //namespace
	

						  
#endif //MINIMISE_H
