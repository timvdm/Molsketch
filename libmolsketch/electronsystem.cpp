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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/
#include "electronsystem.h"

namespace Molsketch {

  class Atom;
  class Molecule;

  ElectronSystem::ElectronSystem(const QList<Atom *> &atoms, int numElectrons)
    : m_molecule(0),
      m_atoms(atoms),
      m_numElectrons(numElectrons)
  {}

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

  void ElectronSystem::setAtoms(const QPair<Atom *, Atom *> &atoms) {
    setAtoms(QList<Atom*>{atoms.first, atoms.second});
  }

  int ElectronSystem::numElectrons() const
  {
    return m_numElectrons;
  }

  void ElectronSystem::setNumElectrons(int numElectrons)
  {
    m_numElectrons = numElectrons;
  }

  PiElectrons::PiElectrons(QList<Atom *> atoms, int numElectrons)
    : ElectronSystem(atoms, numElectrons) {}

  PiElectrons::PiElectrons(const QPair<Atom *, Atom *> &atoms, int numElectrons)
    : PiElectrons(QList<Atom*>{atoms.first, atoms.second}, numElectrons)
  {}

}
