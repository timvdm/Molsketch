#include "mimemolecule.h"

namespace Molsketch {

  MimeMolecule::MimeMolecule()
  {
  }

  void MimeMolecule::setMolecule(Molecule *molecule)
  {
    m_molecule = molecule;
  }

  Molecule* MimeMolecule::molecule() const
  {
    return m_molecule;
  }

}
