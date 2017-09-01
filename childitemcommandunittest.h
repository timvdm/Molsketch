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
#include <commands.h>
#include <molscene.h>
#include <utilities.h>

using namespace Molsketch;

CLASS_FOR_TESTING_WITH_FUNCTIONS(QGraphicsItem, \
                                 QRectF boundingRect() const override {return QRectF();} \
                                 void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override { Q_UNUSED(painter) Q_UNUSED(option) Q_UNUSED(widget) })


class ChildItemCommandUnitTest : public CxxTest::TestSuite {
  MolScene *scene;
  QGraphicsItem *sceneItem, *childItem;
public:
  void setUp() {
    QGraphicsItemForTesting::instanceCounter = 0;
    scene = new MolScene;
    sceneItem = new QGraphicsItemForTesting;
    childItem = new QGraphicsItemForTesting;
    scene->addItem(sceneItem);
  }

  void tearDown() {
    delete scene;
    TSM_ASSERT_EQUALS("Graphics items not cleaned up properly", QGraphicsItemForTesting::instanceCounter, 0);
  }

  void testAddingChild() {
    (new Commands::ChildItemCommand(sceneItem, childItem))->execute();
    QS_ASSERT_EQUALS(childItem->parentItem(), sceneItem);
    QS_ASSERT_EQUALS(childItem->scene(), scene);
  }

  void testRemovingChild() {
    childItem->setParentItem(sceneItem);
    (new Commands::ChildItemCommand(sceneItem, childItem))->execute();
    QS_ASSERT_EQUALS(childItem->parentItem(), nullptr);
    QS_ASSERT_EQUALS(childItem->scene(), nullptr);
  }

  void testAddingChildCycle() {
    (new Commands::ChildItemCommand(sceneItem, childItem))->execute();
    scene->stack()->undo();
    QS_ASSERT_EQUALS(childItem->parentItem(), nullptr);
    QS_ASSERT_EQUALS(childItem->scene(), nullptr);
  }

  void testRemovingChildCycle() {
    childItem->setParentItem(sceneItem);
    (new Commands::ChildItemCommand(sceneItem, childItem))->execute();
    scene->stack()->undo();
    QS_ASSERT_EQUALS(childItem->parentItem(), sceneItem);
    QS_ASSERT_EQUALS(childItem->scene(), scene);
  }
};
