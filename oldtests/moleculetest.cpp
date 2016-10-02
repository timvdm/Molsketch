/***************************************************************************
 *   Copyright (C) 2015 Hendrik Vennekate                                  *
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
#include "moleculetest.h"
#include <molecule.h>
#include <QPair>

using namespace Molsketch;

typedef QVector<QPair<int,int>> connectivityVector;
typedef QPair<int,int> connectivity;

void MoleculeTest::canSplit()
{
  QFETCH(int, atomCount);
  QFETCH(connectivityVector, atomConnectivity);
  QFETCH(int, parts);

  QList<Atom*> atoms = generateAtoms(atomCount);
  QSet<Bond*> bonds;
  foreach(connectivity bond, atomConnectivity)
    bonds << new Bond(atoms[bond.first], atoms[bond.second]);
  Molecule *molecule = new Molecule(atoms.toSet(), bonds);

  Q_ASSERT_X(molecule->split().size() == parts, "Split test", QString("Parts: %1 Expected: %2").arg(molecule->split().size()).arg(parts).toStdString().c_str());
  Q_ASSERT_X(molecule->canSplit() == (parts > 1), "Split test", "Can split and should not");
  int i = 0;
  QStringList atomList, oldAtoms;
  bool sameAtomsInNewMolecule = false;
  foreach(Molecule *splitMolecule, molecule->split())
  {
    QStringList atomsInMolecule, oldAtomsInMolecule;
    foreach(Atom* atom, splitMolecule->atoms())
    {
      atomsInMolecule << QString::number((ulong) atom, 16);
      if (molecule->atoms().contains(atom))
      {
        oldAtomsInMolecule << QString::number((ulong) atom, 16);
        sameAtomsInNewMolecule = true;
      }
    }
    atomList << atomsInMolecule.join(", ");
    oldAtoms << oldAtomsInMolecule.join(", ");
    ++i;
  }
  Q_ASSERT_X(!sameAtomsInNewMolecule, "Split test",
             QString("Old Atoms found in split molecules\n"
                     "Old atoms in new molecules: %1\n"
                     "New molecules:              %2")
             .arg(oldAtoms.join("; "))
             .arg(atomList.join("; "))
             .toStdString().c_str());
}

void MoleculeTest::canSplit_data()
{
  QTest::addColumn<int>("atomCount");
  QTest::addColumn<connectivityVector>("atomConnectivity");
  QTest::addColumn<int>("parts");

  QTest::newRow("cyclopropane") << 3
                                << (connectivityVector()
                                    << qMakePair(0,1)
                                    << qMakePair(1,2)
                                    << qMakePair(2,0))
                                << 1;

  QTest::newRow("atom and bond") << 3
                                << (connectivityVector()
                                    << qMakePair(2,0))
                                << 2;

  QTest::newRow("three atoms") << 3
                               << connectivityVector()
                               << 3;

  QTest::newRow("four atoms and bond and atom") << 7
                                << (connectivityVector()
                                    << qMakePair(1,0)
                                    << qMakePair(2,0)
                                    << qMakePair(1,3)
                                    << qMakePair(4,5))
                                << 3;

  QTest::newRow("ring and bond") << 5
                                << (connectivityVector()
                                    << qMakePair(1,0)
                                    << qMakePair(2,0)
                                    << qMakePair(1,2)
                                    << qMakePair(3,4))
                                << 2;

  QTest::newRow("three atoms") << 0
                               << connectivityVector()
                               << 0;
}

QList<Atom*> MoleculeTest::generateAtoms(int count)
{
  QList<Atom*> atoms;
  for (int i = 0 ; i < count ; ++i)
    atoms << new Atom(QPointF((qreal) qrand()/RAND_MAX,(qreal) qrand()/RAND_MAX), "C");
  return atoms;
}
