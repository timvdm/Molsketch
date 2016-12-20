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
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <QSet>
#include <atom.h>
#include <molecule.h>
#include <molscene.h>
#include <molview.h>
#include <cxxtest/TestSuite.h>
#include <actions/drawaction.h>
#include "utilities.h"

using namespace Molsketch;

class DrawActionAcceptanceTest : public CxxTest::TestSuite {

public:
  void testHintPoints() {
    QPointF originalAtom(20,10),
        drawingStart(19,10),
        drawingStop(-19,10),
        targetPosition(-20,10);

    Atom *atom = new Atom(originalAtom, "C");
    Molecule *molecule = new Molecule(QSet<Atom*>() << atom, QSet<Bond*>());
    MolScene *scene = new MolScene;
    MolView view(scene);
    drawAction *action = new drawAction(scene);
    action->setChecked(true);
    scene->addItem(molecule);

    view.show();

    QTest::mousePress(view.viewport(), Qt::LeftButton, Qt::NoModifier, view.mapFromScene(drawingStart));
    QTest::mouseMove(view.viewport(), view.mapFromScene(drawingStop));
    QTest::mouseRelease(view.viewport(), Qt::LeftButton, Qt::NoModifier, view.mapFromScene(drawingStop));

    QGraphicsItem* newAtom = scene->itemAt(targetPosition, QTransform());
    TS_ASSERT(newAtom); // TODO macro for assert present or return // TODO QSM_ASSERT with message as QString
    if (!newAtom) return;
    QS_ASSERT_EQUALS(newAtom->pos(), targetPosition);
  }
};
