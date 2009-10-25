#ifndef MSK_ELECTRONSYSTEM_H
#define MSK_ELECTRONSYSTEM_H

#include <QList>

namespace Molsketch {

  class Atom;
  class Molecule;

  /**
   * Base class for electron systems.
   *
   * The ElectronSystem class and it's subclasses are an alternative way to
   * store how electrons are distributed in a molecule. It overcomes many 
   * problems associated with connection tables and can be used to represent
   * a wide variety molecules.
   */
  class ElectronSystem
  {
    public:
      enum Type {
        SigmaType,
        PiType,
        CoordType,
        DeficientSigmaType,
      };

      explicit ElectronSystem();
      virtual ~ElectronSystem();

      /**
       * Get the type for this electron system
       */
      virtual Type type() const = 0;


      int numAtoms() const;
      QList<Atom*> atoms() const;
      void setAtoms(const QList<Atom*> &atoms);

      int numElectrons() const;
      void setNumElectrons(int numElectrons);

    protected:
      Molecule     *m_molecule;
      QList<Atom*>  m_atoms;
      int           m_numElectrons;
  };


  class SigmaElectrons : public ElectronSystem
  {
    public:
      enum Geometry {
        TwoCenter, //!< regular 2 electron, 2 center sigma system
        Bridged, //!< bridged sigma electron system (e.g. B-H-B in B5H11, transition states, ...)
        Closed //!< closed sigma electron system (e.g. BBB in B5H11)
      };
      
      SigmaElectrons(Geometry geometry = TwoCenter) : ElectronSystem(), m_geometry(geometry) {}
      virtual ~SigmaElectrons() {}

      Type type() const { return SigmaType; }

      Geometry getGeometry() const { return m_geometry; }
      void setGeometry(Geometry geometry) { m_geometry = geometry; }

    protected:
      Geometry m_geometry;
  };

  class PiElectrons : public ElectronSystem
  {
    public:
      PiElectrons() : ElectronSystem() {}
      virtual ~PiElectrons() {}

      Type type() const { return PiType; }
  };

  class CoordElectrons : public ElectronSystem
  {
    public:
      CoordElectrons() : ElectronSystem() {}
      virtual ~CoordElectrons() {}

      Type type() const { return CoordType; }
  };



}

#endif
