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
#include <molscene.h>
#include <molecule.h>
#include <QSet>
#include "xmlassertion.h"
using namespace Molsketch;
using XmlAssert::assertThat;

const char* XENON = "Xe";

class AtomDrawingTest : public CxxTest::TestSuite {
  MolScene *scene;
  Molecule *molecule;
  Atom *atom;
public:
  void setUp() {
    atom = new Atom(QPointF(), XENON);
    molecule = new Molecule(QSet<Atom*>{atom}, QSet<Bond*>());
    scene = new MolScene;
    scene->addItem(molecule);
  }

  void tearDown() {
    delete scene;
  }

  void testRegularAtomIsDrawn() {
    assertThat(scene->toSvg())->contains("/*:svg/*:g/*:g/*:text/text()/data(.)")->exactlyOnceWithContent("Xe");
  }

  void testNewmanAtomSymbolIsNotDrawn() { // TODO define precedence with regard to colored circle/square/wireframe
    // TODO check color
    atom->setNewmanDiameter(6);
    QString svg = scene->toSvg();
    assertThat(svg)->contains("/*:svg/*:g/*:g/*:text")->never();
    assertThat(svg)->contains("/*:svg/*:g/*:g/*:rect")->never();
    assertThat(svg)->contains("/*:svg/*:g/*:g/*:circle/@r/data(.)")->exactlyOnceWithContent("3");
    assertThat(svg)->contains("/*:svg/*:g/*:g/*:circle/@cx/data(.)")->exactlyOnceWithContent("0");
    assertThat(svg)->contains("/*:svg/*:g/*:g/*:circle/@cy/data(.)")->exactlyOnceWithContent("0");
    // TODO why does cx and cy get moved to 0.5 if diameter is odd?
  }

  void testDrawingColoredCircle() {
    scene->setRenderMode(MolScene::RenderColoredCircles);
    QString svg = scene->toSvg();
    assertThat(svg)->contains("/*:svg/*:g/*:g/*:text")->never();
    assertThat(svg)->contains("/*:svg/*:g/*:g/*:rect")->never();
    assertThat(svg)->contains("/*:svg/*:g/*:g/*:circle/@r/data(.)")->exactlyOnceWithContent("10");
    assertThat(svg)->contains("/*:svg/*:g/*:g/*:circle/@cx/data(.)")->exactlyOnceWithContent("0");
    assertThat(svg)->contains("/*:svg/*:g/*:g/*:circle/@cy/data(.)")->exactlyOnceWithContent("0");
  }

  void testDrawingColoredSquare() {
    scene->setRenderMode(MolScene::RenderColoredSquares);
    QString svg = scene->toSvg();
    assertThat(svg)->contains("/*:svg/*:g/*:g/*:text")->never();
     assertThat(svg)->contains("/*:svg/*:g/*:g/*:circle")->never();
    assertThat(svg)->contains("/*:svg/*:g/*:g/*:rect/@x/data(.)")->exactlyOnceWithContent("-10");
    assertThat(svg)->contains("/*:svg/*:g/*:g/*:rect/@y/data(.)")->exactlyOnceWithContent("-10");
    assertThat(svg)->contains("/*:svg/*:g/*:g/*:rect/@width/data(.)")->exactlyOnceWithContent("20");
    assertThat(svg)->contains("/*:svg/*:g/*:g/*:rect/@height/data(.)")->exactlyOnceWithContent("20");
  }

  void testDrawingColoredWireframe() {
    scene->setRenderMode(MolScene::RenderColoredWireframe);
    QString svg = scene->toSvg();
    assertThat(svg)->contains("/*:svg/*:g/*:g/*:text")->never();
    assertThat(svg)->contains("/*:svg/*:g/*:g/*:rect")->never();
    assertThat(svg)->contains("/*:svg/*:g/*:g/*:circle")->never();
  }

  // TODO check unbound electrons and charge are drawn in the same color
  // TODO check rendering of Hydrogen atoms (including left or right of element)
};
