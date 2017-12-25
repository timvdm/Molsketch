#include "utilities.h"
#include <QGraphicsItem>
#include <radicalelectron.h>
namespace Molsketch {
  class Molecule;
  class MoleculeModelItem;
  class LibraryModel;
  class PiElectrons;
}
template<> int ForTesting<Molsketch::Molecule>::instanceCounter = 0;
template<> int ForTesting<Molsketch::MoleculeModelItem>::instanceCounter = 0;
template<> int ForTesting<Molsketch::LibraryModel>::instanceCounter = 0;
template<> int ForTesting<QGraphicsItem>::instanceCounter = 0;
template<> int ForTesting<Molsketch::RadicalElectron>::instanceCounter = 0;
template<> int ForTesting<Molsketch::PiElectrons>::instanceCounter = 0;
