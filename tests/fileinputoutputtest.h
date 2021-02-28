/***************************************************************************
 *   Copyright (C) 2021 by Hendrik Vennekate                               *
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
#include "utilities.h"
#include <molecule.h>
#include <molscene.h>

using namespace Molsketch;

const QString &SCENE_WITH_LONE_ATOM(
    "<molscene>"
    "<atom"
    " id=\"a1\""
    " elementType=\"A\""
    " userCharge=\"0\""
    " disableHydrogens=\"0\""
    " hydrogens=\"-1\""
    " shapeType=\"0\""
    " hydrogenAlignment=\"0\""
    " colorR=\"0\""
    " colorG=\"0\""
    " colorB=\"0\""
    " scalingParameter=\"1\""
    " zLevel=\"3\""
    " coordinates=\"-87,-158.76\""
    "/>"
    "</molscene>");

class FileInputOutputTest : public CxxTest::TestSuite {
public:
  void testReadingSceneWithAtomWithoutMolecule_wrapsMoleculeAroundThatAtom() {
    MolScene scene;
    QXmlStreamReader xml(SCENE_WITH_LONE_ATOM);
    xml.readNext();
    xml >> scene;
    TS_ASSERT_EQUALS(scene.items().size(), 2);
    QS_ASSERT_ON_POINTER(Atom, scene.items().first(), molecule(), dynamic_cast<Molecule*>(scene.items().last()));
  }
};
