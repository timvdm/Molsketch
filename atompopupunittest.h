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
public:
  void setUp() {
    atom = new Atom;
    popup = new AtomPopup;

    elementEditor = popup->findChild<QLineEdit*>("element");
    chargeBox = popup->findChild<QSpinBox*>("charge");
    hydrogenBox = popup->findChild<QSpinBox*>("hydrogens");
    coordinateTable = popup->findChild<CoordinateTableView*>("coordinates");
  }

  void tearDown() {
    delete atom;
    delete popup;
  }

  void testGuiElementsFound() {
    TS_ASSERT(elementEditor);
    TS_ASSERT(chargeBox);
    TS_ASSERT(hydrogenBox);
    TS_ASSERT(coordinateTable);
  }

  void testGuiElementsFilledOnSetup() {
    const char *element = "Ca";
    const QPolygonF coordinates(QPolygonF() << QPointF(30.5,20.5));
    const int charge = -3;
    const int numImplicitHydrogens = 2;

    atom->setElement(element);
    atom->setCoordinates(coordinates);
    atom->setCharge(charge);
    atom->setNumImplicitHydrogens(numImplicitHydrogens);

    popup->connectAtom(atom);

    QS_ASSERT_EQUALS(elementEditor->text(), element);
    QS_ASSERT_EQUALS(chargeBox->value(), charge);
    QS_ASSERT_EQUALS(hydrogenBox->value(), numImplicitHydrogens);
    QS_ASSERT_EQUALS(coordinateTable->model()->getCoordinates(), coordinates);
    QS_ASSERT_EQUALS(atom->numImplicitHydrogens(), numImplicitHydrogens); // TODO remove exposition of this to GUI
  }

  void testElementNameTransferFromCommand() {

  }

  void testElementNameTransferFromGui() {


  }

  void testChargeTransferFromCommad() {


  }

  void testChargeTransferFromGui() {


  }

  void testCoordinateTransferFromCommand() {


  }

  void testCoordinateTransferFromGui() {

  }

  void testCoordinateTransferChangeFromScene() {

  }

  void testElementNameTransferChangeFromScene() {

  }
};
