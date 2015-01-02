/***************************************************************************
 *   Copyright (C) 2014 by Hendrik Vennekate                               *
 *   HVennekate@gmx.de                                                  *
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

#include <QFileInfo>
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QProcess>
#include <QDir>
#if QT_VERSION < 0x050000
#include <QDesktopServices>
#else
#include <QStandardPaths>
#endif

#include "obabeliface.h"
#include "molecule.h"
#include "bond.h"
#include "atom.h"
#include "molscene.h"

#ifdef OPENBABEL2_TRUNK
#include <openbabel/graphsym.h>
#include <openbabel/stereo/stereo.h>
#include <openbabel/graphsym.h>
#else
#include <openbabel/canon.h>
#endif

#include <openbabel/mol.h>
#include <openbabel/data.h>
#include <openbabel/obconversion.h>

OpenBabel::OBElementTable eTable ;

namespace Molsketch
{
  QString number2symbol( int number )
  {
    return eTable.GetSymbol(number);
  }

  int symbol2number( const QString &symbol )
  {
    return eTable.GetAtomicNum(symbol.STRINGCONVERSION) ;
  }
  
  OpenBabel::OBMol toOBMolecule(const Molsketch::Molecule* originalMolecule, unsigned short int dim = 2)
  {
    // Create the output molecule
    OpenBabel::OBMol obmol ;
    obmol.SetDimension(dim) ;
    if (!originalMolecule) return obmol ;
    
    QHash<Atom*,OpenBabel::OBAtom*> hash;

    obmol.BeginModify();
    foreach (Atom* atom, originalMolecule->atoms()) {
      OpenBabel::OBAtom* obatom = obmol.NewAtom();
      obatom->SetVector(atom->scenePos().x()/40,atom->scenePos().y()/40,0);
      std::string element = atom->element().toStdString();
      obatom->SetAtomicNum(Molsketch::symbol2number(atom->element()));
      hash.insert(atom,obatom);
    }
    foreach (Bond* bond, originalMolecule->bonds()) {
      Atom* a1 = bond->beginAtom();
      Atom* a2 = bond->endAtom();

      int flags = 0 ;
      // Setting bondtype
      switch (bond->bondType()) {
      case Bond::InvertedWedge:
        qSwap(a1, a2) ;
      case Bond::Wedge:
        flags |= OB_WEDGE_BOND;
        break;
      case Bond::InvertedHash:
        qSwap(a1, a2) ;
      case Bond::Hash:
        flags |= OB_HASH_BOND;
        break;
      default: break;
      }
      obmol.AddBond(hash[a1]->GetIdx(),
                      hash[a2]->GetIdx(),
                      bond->bondOrder(),
                      flags);
    }
    obmol.EndModify();
    return obmol;
  }
  
  QString smiles(const Molecule* mol)
  {
    OpenBabel::OBConversion conv ;
    if (!conv.SetOutFormat("can"))
      return "Output format 'can' not available." ;
    
    OpenBabel::OBMol obmol(toOBMolecule(mol)) ;
    return conv.WriteString(&obmol).c_str() ;
  }
  
  bool saveFile(const QString &fileName, QGraphicsScene* scene, unsigned short int dim)
  {
    using namespace OpenBabel;
    OBConversion conversion;

    if (!conversion.SetOutFormat(QFileInfo(fileName).suffix(). STRINGCONVERSION))
    {
      qDebug() << "Error while saving #1";
      return false;
    }

    // Create the output molecule
    OBMol obmol;
    obmol.SetDimension(2);

    // Add all molecules on the scene
    foreach(QGraphicsItem* item, scene->items())
      if (item->type() == Molecule::Type)
        obmol += toOBMolecule(static_cast<Molecule*>(item), dim) ;
    // Add hydrogens
    if (3 == dim // TODO also for 2D?
      && dynamic_cast<MolScene*>(scene)
      && dynamic_cast<MolScene*>(scene)->autoAddHydrogen())
        obmol.AddHydrogens(); // TODO check if this works without begin/end modify

    // Checking if the file exists and making a backup
    if (QFile::exists(fileName)) 
    {
      QFile::remove(fileName + "~");
      QFile::copy(fileName,fileName + "~");
    }

    // Writing the final result to the file
    conversion.WriteFile(&obmol,fileName.toStdString());

    return true;
  }
  
  Molecule* loadFile(const QString &fileName)
  {
    // Creating and setting conversion classes
    using namespace OpenBabel;
    OBConversion conversion ;
    conversion.SetInFormat(conversion.FormatFromExt(fileName. STRINGCONVERSION)) ;
    OBMol obmol;

    if (!conversion.ReadFile(&obmol, fileName.toStdString()))
      return 0;

    // Create a new molecule
    Molecule* mol = new Molecule();
    mol->setPos(QPointF(0,0));

    QHash<OBAtom*, Atom*> atomHash ;
    // Add atoms one-by-ons
    using OpenBabel::OBAtomIterator ;
    FOR_ATOMS_OF_MOL(obatom, obmol)
      atomHash[&(*obatom)] = 
        mol->addAtom(Molsketch::number2symbol(obatom->GetAtomicNum()),
                     QPointF(obatom->x()*40,obatom->y()*40), false);

    // Add bonds one-by-one
    /// Mind the numbering!
    FOR_BONDS_OF_MOL(obbond, obmol)
    {
      Bond* bond  = mol->addBond(atomHash[obbond->GetBeginAtom()],
                                 atomHash[obbond->GetEndAtom()],
                                 obbond->GetBondOrder());
      // Set special bond types
      if (obbond->IsWedge()) 
        bond->setType( Bond::Wedge );
      if (obbond->IsHash()) 
        bond->setType( Bond::Hash );
    }

    return mol;
  }
  
  void getSymmetryClasses(const Molecule* molecule, std::vector<unsigned int>& symmetry_classes)
  {
    symmetry_classes.clear() ;
    if (!molecule) return ;
    OpenBabel::OBMol obmol(toOBMolecule(molecule)) ;
#ifdef OPENBABEL2_TRUNK      
    OpenBabel::OBGraphSym graphsym(&obmol);
    graphsym.GetSymmetry(symmetry_classes);
#else
    OpenBabel::OBBitVec fragatoms(obmol.NumAtoms());

    using OpenBabel::OBMolAtomIter;
    FOR_ATOMS_OF_MOL(a, &obmol)
      fragatoms.SetBitOn(a->GetIdx());
    std::vector<unsigned int> canonical_labels;
    OpenBabel::CanonicalLabels(&obmol, symmetry_classes, canonical_labels);
#endif
  }
  
  QList<Atom*> chiralAtoms(const Molecule* molecule)
  {
    QList<Atom*> result ;
    if (!molecule) return result ;
    
    QList<Atom*> atoms(molecule->atoms()) ;
    OpenBabel::OBMol obmol(toOBMolecule(molecule)) ;
#ifdef OPENBABEL2_TRUNK
    // need to calculate symmetry first
    std::vector<unsigned int> symmetry_classes;
    OpenBabel::OBGraphSym graphsym(obmol);
    graphsym.GetSymmetry(symmetry_classes);

    //std::vector<unsigned long> atomIds = FindTetrahedralAtoms(obmol, symmetry_classes);
    OpenBabel::OBStereoUnitSet units = FindStereogenicUnits(obmol, symmetry_classes);
    
    for (unsigned int i = 0; i < units.size(); ++i) {
      if (units.at(i).type == OpenBabel::OBStereo::Tetrahedral) {
        OpenBabel::OBAtom *obatom = obmol->GetAtomById(units.at(i).id);
        result << atoms[obatom->GetIndex()] ;
      } else 
      if (units.at(i).type == OpenBabel::OBStereo::CisTrans) {
        OpenBabel::OBBond *obbond = obmol->GetBondById(units.at(i).id);
        OpenBabel::OBAtom *obatom1 = obbond->GetBeginAtom();
        OpenBabel::OBAtom *obatom2 = obbond->GetEndAtom();
        result << atoms[obatom1->GetIndex()]
               << atoms[obatom2->GetIndex()] ;
      } 
    }
#else
    using OpenBabel::OBMolAtomIter;
    FOR_ATOMS_OF_MOL(atom, obmol)
      if (atom->IsChiral())
        result << atoms[atom->GetIdx()-1] ;
#endif
    return result ;
  }
  
  Molecule* call_osra(QString fileName)
  {
    int n=0;
#if QT_VERSION < 0x050000
    QString tmpresult = QDesktopServices::storageLocation(QDesktopServices::TempLocation) + QDir::separator() + "osra";
#else
    QString tmpresult = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + QDir::separator() + "osra";
#endif
    tmpresult += ".sdf";
    QString command;
    char *env = getenv("OSRA");
    if (env != NULL)
      command = env;
    else 
      command = "osra";
    
    command += " -f sdf " + fileName + ">" + tmpresult;

    QStringList arguments;
    arguments << "-f";
    arguments << "sdf";
    arguments << fileName;
    arguments << ">";
    arguments << tmpresult;

    if (QProcess::execute(command, arguments))
      return 0;

    Molecule* mol = loadFile(tmpresult);
    if (mol) {
      qreal x_avg = 0, y_avg = 0;

      foreach(Atom* atom, mol->atoms()) {
        x_avg += atom->x();
        y_avg += atom->y();
        n++;
      }
      if (n > 0) {
        x_avg /= n;
        y_avg /= n;
        foreach(Atom* atom, mol->atoms()) 
          atom->setPos(atom->x() - x_avg, y_avg - atom->y());
      }
    }
    QFile::remove(tmpresult);
    return mol;
  }
  
} // namespace