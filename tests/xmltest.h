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

#include <frame.h>
#include <molscene.h>
#include <molecule.h>
#include <QSet>
#include <cxxtest/TestSuite.h>
#include "utilities.h"

#include "scenesettings.h"
#include "settingsitem.h"

using namespace Molsketch;

class XmlTest : public CxxTest::TestSuite
{
public:
  void testBoundingRect() {
    for(QGraphicsItem* item : testData())
    {
      MolScene originalScene, loadedScene;
      originalScene.addItem(item);
      readRestoreScene(originalScene, loadedScene);

      onlyOneTopLevelItem(loadedScene);
      QGraphicsItem* restoredItem = restoreOnlySceneItem(loadedScene);
      Q_ASSERT(restoredItem);
      TSM_ASSERT_EQUALS("Bond width", loadedScene.settings()->bondWidth()->get(), originalScene.settings()->bondWidth()->get());
      TSM_ASSERT_EQUALS("Bounding rect", restoredItem->boundingRect(), item->boundingRect());
      QSM_ASSERT_EQUALS("SVG", loadedScene.toSvg(), originalScene.toSvg());
    }
  }

  QGraphicsItem* restoreOnlySceneItem(const MolScene& loadedScene)
  {
    for (auto item : loadedScene.items())
      if (!item->parentItem())
        return item;
    TS_ASSERT(false);
    return 0;
  }

  void onlyOneTopLevelItem(const MolScene& scene)
  {
    int count = 0;
    for (auto item : scene.items())
      if (!item->parentItem())
        ++count;
    TS_ASSERT_EQUALS(count, 1);
  }

  QList<QGraphicsItem*> testData() {
    return QList<QGraphicsItem*>()
        << generateCyclopropaneAnalogon("C")
        << generateCyclopropaneAnalogon("N")
        << generateCyclopropaneAnalogon("Si")
        << generateMoleculeWithSingleAtom("C")
        << generateMoleculeWithSingleAtom("Si")
        << generateBracketFrame(generateCyclopropaneAnalogon("C"))
        << generateBracketFrame(generateCyclopropaneAnalogon("Si"))
        << generateBracketFrame();
  }

  void readRestoreScene(MolScene& source, MolScene& target)
  {
    QByteArray xmlData;
    QXmlStreamWriter xml(&xmlData);

    xml.setAutoFormatting(true);
    xml.writeStartDocument();
    xml << source;
    xml.writeEndDocument();

    QXmlStreamReader xmlReader(xmlData);
    while (xmlReader.name().toString() != target.xmlName())
      xmlReader.readNext() ;
    xmlReader >> target ;
    TS_ASSERT(!xmlReader.hasError());
  }

  QGraphicsItem *generateCyclopropaneAnalogon(const QString& element)
  {
    auto atomA = new Atom(QPointF(0,0), element);
    auto atomB = new Atom(QPointF(50,0), element);
    auto atomC = new Atom(QPointF(25,30), element);
    auto bondAB = new Bond(atomA, atomB);
    auto bondBC = new Bond(atomB, atomC);
    auto bondCA = new Bond(atomC, atomA);
    return new Molecule(QSet<Atom*>() << atomA << atomB << atomC,
                        QSet<Bond*>() << bondAB << bondBC << bondCA);
  }

  QGraphicsItem *generateBracketFrame(QGraphicsItem *molecule = 0)
  {
    auto frame = new Frame;
    frame->setFrameString("(r .5,r.5)+(-10,0)-+( 10,0)-(r .5,r-.5)-+(-10,0)"
                          "(r-.5,r.5)+( 10,0)-+(-10,0)-(r-.5,r-.5)-+( 10,0)");
    if (molecule)
      molecule->setParentItem(frame);
    return frame;
  }

  QGraphicsItem *generateMoleculeWithSingleAtom(const QString& element)
  {
    return new Molecule(QSet<Atom*>() << new Atom(QPointF(30,30), element), QSet<Bond*>());
  }
};
