/***************************************************************************
 *   Copyright (C) 2015 Hendrik Vennekate                                  *
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
#include "xmltest.h"

#include <QXmlStreamWriter>
#include <frame.h>
#include <molscene.h>
#include <atom.h>
#include <bond.h>
#include <QSet>
#include <molecule.h>
#include <QGraphicsItem>

using namespace Molsketch;


void XmlTest::initTestCase()
{
}

void XmlTest::cleanupTestCase()
{
}

void XmlTest::init()
{
}

void XmlTest::cleanup()
{
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
  Q_ASSERT(!xmlReader.hasError());
}

void XmlTest::boundingRectTest()
{
  QFETCH(QGraphicsItem*, sceneObject);

  MolScene originalScene, loadedScene;
  originalScene.addItem(sceneObject);
  readRestoreScene(originalScene, loadedScene);

  QGraphicsItem* restoredItem = 0;
  for (auto item : loadedScene.items())
  {
    if (!item->parentItem())
    {
      Q_ASSERT(!restoredItem);
      restoredItem = item;
    }
  }
  Q_ASSERT(restoredItem);;
  QCOMPARE(loadedScene.bondWidth(), originalScene.bondWidth());
  QCOMPARE(restoredItem->boundingRect(), sceneObject->boundingRect());
  QCOMPARE(loadedScene.toSvg(), originalScene.toSvg());
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

QGraphicsItem *generateBracketFrame(QGraphicsItem *molecule)
{
  auto frame = new Frame;
  frame->setFrameString("(r .5,r.5)+(-10,0)-+( 10,0)-(r .5,r-.5)-+(-10,0)"
                        "(r-.5,r.5)+( 10,0)-+(-10,0)-(r-.5,r-.5)-+( 10,0)");
  molecule->setParentItem(frame);
  return frame;
}

QGraphicsItem *generateMoleculeWithSingleAtom(const QString& element)
{
  return new Molecule(QSet<Atom*>() << new Atom(QPointF(30,30), element), QSet<Bond*>());
}

void XmlTest::boundingRectTest_data()
{
  QTest::addColumn<QGraphicsItem*>("sceneObject");

  QTest::newRow("Cyclopropane") << generateCyclopropaneAnalogon("C");
  QTest::newRow("Cyclic azide") << generateCyclopropaneAnalogon("N");
  QTest::newRow("Cyclopropasilane") << generateCyclopropaneAnalogon("Si");
  QTest::newRow("Carbon atom") << generateMoleculeWithSingleAtom("C");
  QTest::newRow("Silicon atom") << generateMoleculeWithSingleAtom("Si");
  QTest::newRow("Cyclopropane with frame") << generateBracketFrame(generateCyclopropaneAnalogon("C"));
  QTest::newRow("Cyclopropasilane with frame") << generateBracketFrame(generateCyclopropaneAnalogon("Si"));
}
