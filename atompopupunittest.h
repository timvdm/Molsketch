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

#include <QLineEdit>
#include <QSpinBox>
#include <atom.h>
#include <atompopup.h>
#include <coordinatetableview.h>
#include <coordinatemodel.h>
#include <molecule.h>
#include <molscene.h>
#include <commands.h>
#include <molview.h>
#include <cxxtest/TestSuite.h>
#include "utilities.h"

using namespace Molsketch;

class AtomPopupUnitTest : public CxxTest::TestSuite {
  Atom *atom;
  AtomPopup *popup;
  QSpinBox *chargeBox;
  QLineEdit *elementEditor;
  QSpinBox *hydrogenBox;
  CoordinateTableView *coordinateTable;
  MolScene* scene;
public:
  void setUp() {
    atom = new Atom;
    popup = new AtomPopup;
    Molecule *molecule = new Molecule(QSet<Atom*>() << atom, QSet<Bond*>());
    scene = new MolScene();
    scene->addItem(molecule);

    elementEditor = popup->findChild<QLineEdit*>("element");
    chargeBox = popup->findChild<QSpinBox*>("charge");
    hydrogenBox = popup->findChild<QSpinBox*>("hydrogens");
    coordinateTable = popup->findChild<CoordinateTableView*>("coordinates");
  }

  void tearDown() {
    delete popup;
    delete scene;
  }

  void testGuiElementsFound() {
    TS_ASSERT(elementEditor);
    TS_ASSERT(chargeBox);
    TS_ASSERT(hydrogenBox);
    TS_ASSERT(coordinateTable);

    TS_ASSERT(!popup->isEnabled());
  }

  void testGuiElementsFilledOnSetup() {
    const char *element = "Ca";
    const QPolygonF coordinates(QPolygonF() << QPointF(30.5,20.5));
    const int charge = -3;
    const int numImplicitHydrogens = 3;

    atom->setElement(element);
    atom->setCoordinates(coordinates);
    atom->setCharge(charge);
    atom->setNumImplicitHydrogens(numImplicitHydrogens);

    popup->connectAtom(atom);

    QS_ASSERT_EQUALS(elementEditor->text(), element);
    QS_ASSERT_EQUALS(chargeBox->value(), charge);
    QS_ASSERT_EQUALS(hydrogenBox->value(), numImplicitHydrogens);
    QS_ASSERT_EQUALS(coordinateTable->model()->getCoordinates(), coordinates);
    QS_ASSERT_EQUALS(atom->numImplicitHydrogens(), numImplicitHydrogens); // TODO remove exposition of this to GUI or at least make it transparent
    TS_ASSERT(popup->isEnabled());
  }

  void testElementNameTransferFromCommand() {
    QString newElement("Mg");
    popup->connectAtom(atom);
    scene->stack()->push(new Commands::ChangeElement(atom, newElement));
    QS_ASSERT_EQUALS(elementEditor->text(), newElement);
  }

  void testElementNameTransferFromGui() {
    QString expectedSymbol("Ba");
    elementEditor->setText(expectedSymbol);
    QS_ASSERT_EQUALS(atom->element(), expectedSymbol);
    // TODO check refresh while typing -> actually type into widget
  }

  void testChargeTransferFromCommad() {
    int newCharge(-5);
    popup->connectAtom(atom);
    scene->stack()->push(new Commands::setAtomChargeCommand(atom, newCharge));
    TS_ASSERT_EQUALS(chargeBox->value(), newCharge);
  }

  void testChargeTransferFromGui() {
    int newCharge = -5;
    popup->connectAtom(atom);
    chargeBox->setValue(newCharge);
    TS_ASSERT_EQUALS(atom->charge(), newCharge);
  }

  void testCoordinateTransferFromCommand() {
    QPointF originalPosition(5,5), shift(3,4);
    atom->setCoordinate(0, originalPosition);
    popup->connectAtom(atom);
    scene->stack()->push(new Commands::MoveItem(atom, shift));
    QVector<QPointF> expectedCoordinates;
    expectedCoordinates << originalPosition + shift;
    QS_ASSERT_EQUALS(coordinateTable->model()->getCoordinates(), expectedCoordinates);
  }

  void testCoordinateTransferFromGui() {
    atom->setCoordinates({QPointF(0,0)});
    popup->connectAtom(atom);
    enterDataIntoCell(coordinateTable, "2.3", 0, 0);
    enterDataIntoCell(coordinateTable, "5.2", 0, 1);
    QVector<QPointF> expectedCoordinates = {QPointF(2.3, 5.2)};
    QS_ASSERT_EQUALS(coordinateTable->model()->getCoordinates(), expectedCoordinates);
    QS_ASSERT_EQUALS(atom->coordinates(), expectedCoordinates);
  }

  void testCoordinateTransferChangeFromScene() {
    atom->setCoordinates({QPointF(0,0)});
    popup->connectAtom(atom);
    MolView view(scene);

    view.show();
    QTest::mousePress(view.viewport(), Qt::LeftButton, Qt::NoModifier, view.mapFromScene(QPointF(0,0)));
    mouseMoveEvent(view.viewport(), Qt::LeftButton, Qt::NoModifier, view.mapFromScene(QPointF(10,20)));
    QVector<QPointF> expectedCoordinates = {QPointF(10,20)};
    QS_ASSERT_EQUALS(atom->coordinates(), expectedCoordinates);
    QS_ASSERT_EQUALS(coordinateTable->model()->getCoordinates(), expectedCoordinates);
  }

  void testElementNameTransferChangeFromScene() {
    QString expectedSymbol("Ba");
    atom->setCoordinates({QPointF(0,0)});
    popup->connectAtom(atom);
    MolView view(scene);

    view.show();
    QTest::mouseDClick(view.viewport(), Qt::LeftButton, Qt::NoModifier, view.mapFromScene(QPointF(0,0)));
    QTest::keyClicks(view.viewport(), expectedSymbol);
    QTest::mouseClick(view.viewport(), Qt::LeftButton, Qt::NoModifier, view.mapFromScene(QPointF(20,20)));

    QS_ASSERT_EQUALS(atom->element(), expectedSymbol);
    QS_ASSERT_EQUALS(elementEditor->text(), expectedSymbol);
  }

  // TODO Test deletion of atom from scene.
  // TODO extract parent class from this and arrowPopupAcceptanceTest
};
