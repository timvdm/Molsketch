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

#include <cxxtest/TestSuite.h>

#include <atom.h>
#include <radicalelectron.h>
#include "utilities.h"

using namespace Molsketch;

const QString ATOM_XML("<atom elementType=\"\" hydrogenCount=\"0\" colorR=\"0\" colorG=\"0\" colorB=\"0\" scalingParameter=\"1\" coordinates=\"0,0\"><radicalElectron diameter=\"3\" colorR=\"0\" colorG=\"0\" colorB=\"0\"><bbLinker originAnchor=\"Top\" targetAnchor=\"Bottom\" xOffset=\"0\" yOffset=\"0\"/></radicalElectron></atom>");
const qreal RADICAL_DIAMETER = 3;

class AtomUnitTest : public CxxTest::TestSuite {
  Atom *sampleAtom;

public:
  void setUp() {
    sampleAtom = new Atom();
  }

  void tearDown() {
    delete sampleAtom;
  }

  void testXmlOutputWithRadicalElectron() {
    RadicalElectron *radicalElectron = new RadicalElectron(RADICAL_DIAMETER);
    radicalElectron->setParentItem(sampleAtom);
    QString output;
    QXmlStreamWriter writer(&output);
    sampleAtom->writeXml(writer);
    QS_ASSERT_EQUALS(output, ATOM_XML);
    delete radicalElectron;
  }

  void testXmlInputWithRadicalElectron() {
    QXmlStreamReader reader(ATOM_XML);
    reader.readNextStartElement();
    sampleAtom->readXml(reader);
    TS_ASSERT_EQUALS(sampleAtom->childItems().size(), 1);
    TS_ASSERT_DIFFERS(dynamic_cast<RadicalElectron*>(sampleAtom->childItems().first()), nullptr);
  }
};
