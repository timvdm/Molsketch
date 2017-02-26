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
#include "utilities.h"
#include <molscene.h>
#include <molecule.h>
#include <typeindex>
#include <frame.h>
#include <arrow.h>
#include <textitem.h>
#include <QDebug>
#include <atomnumberitem.h>
#include <graphsymitem.h>
#include <smilesitem.h>
#include <stereocenteritem.h>

using namespace Molsketch;

struct MolSceneForTest : public MolScene {
  XmlObjectInterface* produceChild(const QString &childName, const QString &type) {
    return MolScene::produceChild(childName, type);
  }
};

class MolSceneUnitTest : public CxxTest::TestSuite {
  MolSceneForTest *scene;
public:
  void setUp() {
    scene = new MolSceneForTest;
  }

  void tearDown() {
    delete scene;
  }

  void testDefaultProduceChild() {
    TS_ASSERT_EQUALS(scene->produceChild("Somestring", ""), nullptr);
    TS_ASSERT(scene->items().isEmpty());
  }

  void testProduceChild() {
    struct ExpectedChild{
      QString name;
      QString type;
      std::type_index expectedType;
    };
    ExpectedChild children[] = {
      {"frame", "", typeid(Frame)},
      {"molecule", "", typeid(Molecule)},
      {"molecule", "test", typeid(Molecule)},
      {"arrow", "", typeid(Arrow)},
      {"object", "ReactionArrow", typeid(Arrow)},
      {"object", "MechanismArrow", typeid(Arrow)},
      {"textItem", "", typeid(TextItem)},
      {"plugin", "Atom Numbers", typeid(AtomNumberItem)},
      {"plugin", "Symmetry Classes", typeid(GraphSymItem)},
      {"plugin", "SMILES", typeid(SmilesItem)},
      {"plugin", "Stereogenic Atoms", typeid(StereoCenterItem)},
      };
    for (auto child : children) {
      XmlObjectInterface *sceneChild = scene->produceChild(child.name, child.type);
      TS_ASSERT_EQUALS(child.expectedType, std::type_index(typeid(*(sceneChild))));
      TS_ASSERT(scene->items().contains(dynamic_cast<QGraphicsItem*>(sceneChild)));
    }
  }
};
