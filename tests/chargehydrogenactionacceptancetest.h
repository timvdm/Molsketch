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

#include <atom.h>
#include <molecule.h>
#include <molscene.h>
#include <molview.h>
#include <cxxtest/TestSuite.h>
#include <actions/incdecaction.h>
#include "utilities.h"

using namespace Molsketch;

class ChargeHydrogenActionAcceptanceTest : public CxxTest::TestSuite {

  template<class T, class ITEM, typename VALUE, VALUE (ITEM::*getter)() const>
  void performIncrementActionTest(Molecule *molecule, ITEM *item, VALUE initialCount) {
    MolScene *scene = new MolScene();
    scene->addItem(molecule);

    T* action = new T(scene);
    action->setChecked(true);
    action->incrementAction()->setChecked(true);

    MolView *view = new MolView(scene);
    view->show();

    TS_ASSERT_EQUALS(action->isChecked(), true);
    TS_ASSERT_EQUALS(action->incrementAction()->isChecked(), true);

    TSM_ASSERT_EQUALS("Count before increment wrong", (item->*getter)(), initialCount);
    const QPoint clickOffset(2,2);
    QTest::mouseClick(view->viewport(), Qt::LeftButton, Qt::NoModifier, view->mapFromScene(item->pos()) + clickOffset);
    TSM_ASSERT_EQUALS("Hydrogen count after increment wrong", (item->*getter)(), initialCount + 1);

    delete view;
    delete scene;
  }

  template<class T, int (Atom::*getter)() const>
  void performIncrementActionTest(int initialCount) {
    Atom *atom = new Atom;
    Molecule *molecule = new Molecule(QSet<Atom*>() << atom, QSet<Bond*>());

    performIncrementActionTest<T, Atom, int, getter>(molecule, atom, initialCount);
  }

public:
  void testChargeIncreasedWithAction() {
    performIncrementActionTest<hydrogenAction, &Atom::numImplicitHydrogens>(0);
  }

  void testHydrogenCountIncreasedWithAction() {
    performIncrementActionTest<chargeAction, &Atom::charge>(0);
  }

  void testZLevelIncreasedWithAction() {
    Atom *atomA = new Atom(QPointF(-10, 0), "C"), *atomB = new Atom(QPointF(10, 0), "C");
    Bond *bond = new Bond(atomA, atomB);
    Molecule *molecule = new Molecule({atomA, atomB}, {bond});
    performIncrementActionTest<ZLevelStepAction, QGraphicsItem, qreal, &QGraphicsItem::zValue>(molecule, bond, 2);
  }
};
