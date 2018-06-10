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
#include "utilities.h"
#include <molscene.h>
#include <molecule.h>
#include <typeindex>
#include <frame.h>
#include <arrow.h>
#include <textitem.h>
#include <QDebug>
#include <scenesettings.h>

using namespace Molsketch;

const qreal BOND_ANGLE_FROM_SETTINGS = 4.5;
const QString SCENE_XML_WITH_ATTRIBUTE("<molscene MolsceneBondAngle=\"" + QString::number(BOND_ANGLE_FROM_SETTINGS) + "\"/>");

struct MolSceneForTest : public MolScene {
  XmlObjectInterface* produceChild(const QString &childName, const QString &type) {
    return MolScene::produceChild(childName, type);
  }
  QList<const XmlObjectInterface*> children() const {
    return MolScene::children();
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
      const char* toString() {
        return (name + type).toStdString().c_str();
      }
    };
    ExpectedChild children[] = {
      {"frame", "", typeid(Frame)},
      {"molecule", "", typeid(Molecule)},
      {"molecule", "test", typeid(Molecule)},
      {"arrow", "", typeid(Arrow)},
      {"object", "ReactionArrow", typeid(Arrow)},
      {"object", "MechanismArrow", typeid(Arrow)},
      {"textItem", "", typeid(TextItem)},
      {"settings", "", typeid(SceneSettings)},
      };
    for (auto child : children) {
      XmlObjectInterface *sceneChild = scene->produceChild(child.name, child.type);
      TSM_ASSERT(child.toString(), sceneChild);
      TSM_ASSERT_EQUALS(child.toString(), child.expectedType, std::type_index(typeid(*(sceneChild))));
      TSM_ASSERT(child.toString(), scene->children().contains(sceneChild));
      if (QGraphicsItem *item = dynamic_cast<QGraphicsItem*>(sceneChild))
        TSM_ASSERT(child.toString(), scene->items().contains(item));
    }
  }

  void testInitializingSettingsFromAttributes() {
    QXmlStreamReader reader(SCENE_XML_WITH_ATTRIBUTE);
    reader.readNextStartElement();
    scene->readXml(reader);
    TS_ASSERT_EQUALS(scene->bondAngle(), BOND_ANGLE_FROM_SETTINGS);
  }
};
