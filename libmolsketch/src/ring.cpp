/***************************************************************************
 *   Copyright (C) 2009 by Tim Vandermeersch                               *
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

#include <QSet>
#include "ring.h"
#include "bond.h"

namespace Molsketch {
  
  QList<Atom*> Ring::atoms() const 
  {
    return m_atoms ;
  }
  
  int Ring::size() const
  {
    return m_atoms.size();
  }
  
  QPointF Ring::center() const
  {
    QPointF c(0.0, 0.0);
    foreach (Atom *atom, m_atoms)
      c += atom->pos();
    c /= m_atoms.size();
    return c;
  }
  
  QSet<Bond*> Ring::bonds() const
  {
    // Determine "our" bonds
    QSet<Bond*> ringBonds ;
    foreach(Atom* atom, m_atoms)
      foreach(Bond* bond, atom->bonds())
        if (m_atoms.contains(bond->otherAtom(atom)))
          ringBonds << bond ;
    return ringBonds ;
  }
  
  int Ring::numberOfDoubleBonds() const
  {
        // Count number of double bonds for this ring
    int numDoubleBonds = 0 ;
    foreach(Bond* bond, bonds())
      if (bond->bondOrder() == 2)
        ++ numDoubleBonds ;
    return numDoubleBonds ;
  }
  
  void Ring::refreshBonds()
  {
    int numDoubleBonds = numberOfDoubleBonds() ;
    // compare to other rings of those bonds
    foreach(Bond* bond, bonds())
      if (!bond->ring() 
        || bond->ring()->numberOfDoubleBonds() < numDoubleBonds)
        bond->setRing(this) ;
  }
  
  void Ring::setAtoms(const QList<Atom*>& atoms)
  {
    m_atoms = atoms;
    refreshBonds() ;
  }
  
  
} // namespace