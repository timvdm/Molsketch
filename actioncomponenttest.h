/***************************************************************************
 *   Copyright (C) 2016 by Hendrik Vennekate                               *
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

#include <molecule.h>
#include <molscene.h>
#include <QSet>
#include <actions/deleteaction.h>
#include <cxxtest/TestSuite.h>

using namespace Molsketch;

class ActionComponentTest : public CxxTest::TestSuite
{
public:
  void testRemovalOfBondSplitsMolecule()
  {
    Atom* a1 = new Atom(QPointF( 0, 0), "C"),
        * a2 = new Atom(QPointF(50,50), "C"),
        * a3 = new Atom(QPointF( 0,50), "C");
    Bond* b12 = new Bond(a1, a2),
        * b23 = new Bond(a2, a3);
    Molecule* m = new Molecule(QSet<Atom*>() << a1 << a2 << a3,
                               QSet<Bond*>() << b12 << b23);

    MolScene scene;
    scene.addItem(m);

    b23->setSelected(true);
    TSM_ASSERT_EQUALS("Number of selected items", scene.selectedItems().size(), 1);
    TSM_ASSERT_EQUALS("Bond is selected", scene.selectedItems().first(), b23);
    TSM_ASSERT_EQUALS("No commands executed before deletion", scene.stack()->index(), 0);

    QAction* delAction = new deleteAction(&scene);
    delAction->trigger();

    QList<Molecule*> molecules;
    for (QGraphicsItem* item : scene.items())
      if (Molecule* molecule = dynamic_cast<Molecule*>(item))
        molecules << molecule;
    TSM_ASSERT_EQUALS("Molecule count after split", molecules.size(), 2);
    TSM_ASSERT_EQUALS("One deletion command executed", scene.stack()->index(), 1);
  }
};
