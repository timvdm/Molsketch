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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#include <cxxtest/TestSuite.h>
#include <QApplication>
#include <QClipboard>
#include <molscene.h>
#include <QMimeData>
#include "utilities.h"

using namespace Molsketch;

const QString MIME_TYPE("molecule/molsketch");
const QString CLIPBOARD_CONTENT("<?xml version=\"1.0\" encoding=\"UTF-8\"?><atom id=\"\" elementType=\"Ca\" userCharge=\"0\" disableHydrogens=\"0\" hydrogens=\"0\" hydrogenAlignment=\"0\" colorR=\"0\" colorG=\"0\" colorB=\"0\" scalingParameter=\"1\" zLevel=\"3\" coordinates=\"5,3\"/>\n");
const QPointF ATOM_COORDS(5,3);
const QString ELEMENT("Ca");

class ClipboardTest : public CxxTest::TestSuite {
  MolScene *scene;
public:
  void setUp() {
    scene = new MolScene;
  }

  void tearDown() {
    delete scene;
  }

  void testCopying() {
    Atom *atom = new Atom(ATOM_COORDS, ELEMENT);
    scene->addItem(atom);
    atom->setSelected(true);
    scene->copy();
    QS_ASSERT_EQUALS(QApplication::clipboard()->mimeData()->data(MIME_TYPE), CLIPBOARD_CONTENT);
    TS_ASSERT_EQUALS(scene->items().size(), 1);
    TS_ASSERT_EQUALS(scene->stack()->count(), 0);
  }

  void testCutting() {
    Atom *atom = new Atom(ATOM_COORDS, ELEMENT);
    scene->addItem(atom);
    atom->setSelected(true);
    scene->cut();
    QS_ASSERT_EQUALS(QApplication::clipboard()->mimeData()->data(MIME_TYPE), CLIPBOARD_CONTENT);
    TS_ASSERT_EQUALS(scene->items().size(), 0);
    TS_ASSERT_EQUALS(scene->stack()->count(), 1);
  }

  void testPasting() {
    QMimeData *mimeData = new QMimeData;
    mimeData->setData(MIME_TYPE, CLIPBOARD_CONTENT.toUtf8());
    QApplication::clipboard()->setMimeData(mimeData);
    scene->paste();
    TS_ASSERT_EQUALS(scene->items().size(), 1);
    QS_ASSERT_ON_POINTER(Atom, scene->items().first(), element(), ELEMENT);
    TS_ASSERT_EQUALS(scene->stack()->count(), 1);
  }

  // TODO test cases without selection/clipboard content
};
