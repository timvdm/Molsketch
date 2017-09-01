#include "utilities.h"
#include <QGraphicsItem>
namespace Molsketch {
  class Molecule;
  class MoleculeModelItem;
  class LibraryModel;
}
template<> int ForTesting<Molsketch::Molecule>::instanceCounter = 0;
template<> int ForTesting<Molsketch::MoleculeModelItem>::instanceCounter = 0;
template<> int ForTesting<Molsketch::LibraryModel>::instanceCounter = 0;
template<> int ForTesting<QGraphicsItem>::instanceCounter = 0;
