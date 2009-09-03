
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
