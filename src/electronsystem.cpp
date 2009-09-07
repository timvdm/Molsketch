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
#include "electronsystem.h"

namespace Molsketch {

  class Atom;
  class Molecule;


  ElectronSystem::ElectronSystem() : m_molecule(0), m_numElectrons(0)
  {
  }
  
  ElectronSystem::~ElectronSystem()
  {
  }

  int ElectronSystem::numAtoms() const
  {
    return m_atoms.size();
  }

  QList<Atom*> ElectronSystem::atoms() const
  {
    return m_atoms;
  }

  void ElectronSystem::setAtoms(const QList<Atom*> &atoms)
  {
    m_atoms = atoms;
  }

  int ElectronSystem::numElectrons() const
  {
    return m_numElectrons;
  }

  void ElectronSystem::setNumElectrons(int numElectrons)
  {
    m_numElectrons = numElectrons;
  }




}
