#ifndef MIMIMOLECULE_H
#define MIMIMOLECULE_H

#include <QMimeData>

namespace Molsketch {

  class Molecule;

  class MimeMolecule : public QMimeData
  {
    public:
      MimeMolecule();

      void setMolecule(Molecule *molecule);
      Molecule* molecule() const;

    private:
      Molecule *m_molecule;
  };

}

#endif // MIMIMOLECULE_H
