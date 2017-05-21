/***************************************************************************
 *   Copyright (C) 2017 by Hendrik Vennekate                               *
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

#include "utilities.h"
#include <cxxtest/TestSuite.h>
#include <QMimeData>
#include <QModelIndex>
#include <librarymodel.h>
#include <constants.h>
#include <molecule.h>
#include <QList>

using namespace Molsketch;

CLASS_FOR_TESTING(Molecule)

class LibraryModelUnitTest : public CxxTest::TestSuite {
  LibraryModel* model;
public:
  void setUp() {
    model = new LibraryModel;
    MoleculeForTesting::instanceCounter = 0;
  }

  void tearDown() {
    delete model;
    model = 0;
    TSM_ASSERT_EQUALS("Molecules not cleaned up!", MoleculeForTesting::instanceCounter, 0);
  }

  void testRowCount() {
    Molecule *m1 = new MoleculeForTesting;
    Molecule *m2 = new MoleculeForTesting;
    Molecule *m3 = new MoleculeForTesting;
    model->setMolecules(QList<Molecule*>()<< m1 << m2 << m3);
    TS_ASSERT_EQUALS(model->rowCount(), 3);
  }

  void testRowCountSameMoleculeMultipleTimes() {
    Molecule *m = new MoleculeForTesting;
    model->setMolecules(QList<Molecule*>() << m << m << m);
    TS_ASSERT_EQUALS(model->rowCount(), 3);
    model->setMolecules(QList<Molecule*>() << new MoleculeForTesting);
    TS_ASSERT_EQUALS(model->rowCount(), 1);
  }

  void testRowCountByAddingIndividually() {
    model->addMolecule(new MoleculeForTesting);
    model->addMolecule(new MoleculeForTesting);
    TS_ASSERT_EQUALS(model->rowCount(), 2);
  }

  void testMoleculesAreAddedTwice() {
    Molecule *m = new MoleculeForTesting;
    model->addMolecule(m);
    model->addMolecule(m);
    TS_ASSERT_EQUALS(model->rowCount(), 2);
  }

  void testDeletingOldMolecules() {
    model->setMolecules(QList<Molecule*>() << new MoleculeForTesting);
    model->setMolecules(QList<Molecule*>() << new MoleculeForTesting);
    TS_ASSERT_EQUALS(MoleculeForTesting::instanceCounter, 1);
  }

  void testMimeData() {
    Molecule *m = new MoleculeForTesting;
    QByteArray expectedXml;
    QXmlStreamWriter xmlWriter(&expectedXml);
    xmlWriter.writeStartDocument();
    xmlWriter << *m;
    xmlWriter.writeEndDocument();
    model->addMolecule(m);
    QByteArray obtainedData = model->mimeData(QModelIndexList() << model->index(0,0))->data(moleculeMimeType);
    QS_ASSERT_EQUALS(obtainedData, expectedXml);
  }

  void testDataForDecorationRole() {
    Molecule *m = new MoleculeForTesting;
    Atom *a1 = new Atom(QPointF(), "C");
    Atom *a2 = new Atom(QPointF(10, 10), "N");
    m->addAtom(a1);
    m->addAtom(a2);
    m->addBond(new Bond(a1, a2));
    QPixmap expectedIcon = renderMolecule(*m);

    model->addMolecule(m);
    QPixmap actualIcon = model->data(model->index(0), Qt::DecorationRole).value<QPixmap>();
    QS_ASSERT_EQUALS(actualIcon.toImage(), expectedIcon.toImage());
  }

  void testDataForDisplayRole() {
    Molecule *m = new MoleculeForTesting;
    m->setName("testname");
    model->addMolecule(m);
    QS_ASSERT_EQUALS(model->data(model->index(0)).toString(), "testname");
  }

  void testEmptyReturns() {
    TS_ASSERT_EQUALS(model->rowCount(), 0);
    TS_ASSERT(model->data(QModelIndex()).isNull());
    QMimeData *mimeData = model->mimeData(QModelIndexList());
    TS_ASSERT(mimeData->hasFormat(moleculeMimeType));
    QS_ASSERT_EQUALS(mimeData->data(moleculeMimeType), "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    delete mimeData;
  }

  // TODO: thread safety
};
