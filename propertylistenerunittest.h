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

#include <QUndoCommand>
#include <graphicsitem.h>
#include <molscene.h>
#include <propertylistener.h>
#include <cxxtest/TestSuite.h>
#include <QDebug>

using namespace Molsketch;

class GraphicsItemForTesting : public graphicsItem
{
  QPolygonF coordinates() const {return QPolygonF();}
  void setCoordinates(const QVector<QPointF>& c) {Q_UNUSED(c)}
  QString xmlName() const { return ""; }
  QRectF boundingRect() const { return QRectF(); }
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    Q_UNUSED(painter) Q_UNUSED(option) Q_UNUSED(widget)}
public:
  void pushToUndoStack() { attemptUndoPush(new QUndoCommand()); }
};

class PropertyListenerForTesting : public PropertyListener
{
  void reactToPropertyChange() { ++ timesCalled; }
public:
  int timesCalled;
  PropertyListenerForTesting() : timesCalled(0) {}
  graphicsItem* getItem() { return item(); }
};

class PropertyListenerUnitTest : public CxxTest::TestSuite
{
private:
  PropertyListenerForTesting* listener;
  GraphicsItemForTesting* item;
public:

  void setUp() {
    listener = new PropertyListenerForTesting;
    item = new GraphicsItemForTesting;
  }

  void tearDown() {
    delete listener;
    delete item;
  }

  void testRegisteringNullItem() {
    listener->registerItem(0);
    TS_ASSERT_EQUALS(listener->getItem(), nullptr);
  }

  void testRegisteringNullListener() {
    item->registerPropertyListener(listener);
    item->registerPropertyListener(0);
    item->pushToUndoStack();
    TS_ASSERT_EQUALS(listener->getItem(), item);
    TS_ASSERT_EQUALS(listener->timesCalled, 1);
  }

  void testRegisteredListenerIsCalledWhenPushedToUndoStack() {
    item->registerPropertyListener(listener);
    item->pushToUndoStack();
    TS_ASSERT_EQUALS(listener->timesCalled, 1);
  }

  void testRegisteringTwiceLeadsToOnlyOneCallToListener() {
    item->registerPropertyListener(listener);
    item->registerPropertyListener(listener);
    item->pushToUndoStack();
    TS_ASSERT_EQUALS(listener->timesCalled, 1);
  }

  void testListenerKnowsWhatItemToListenTo() {
    item->registerPropertyListener(listener);
    TS_ASSERT_EQUALS(listener->getItem(), item);
  }

  void testUnregisteringViaItem() {
    item->registerPropertyListener(listener);
    item->unregisterPropertyListener(listener);
    item->pushToUndoStack();
    TS_ASSERT_EQUALS(listener->getItem(), nullptr);
    TS_ASSERT_EQUALS(listener->timesCalled, 0);
  }

  void testUnregisteringViaListener() {
    listener->registerItem(item);
    listener->registerItem(0);
    item->pushToUndoStack();
    TS_ASSERT_EQUALS(listener->getItem(), nullptr);
    TS_ASSERT_EQUALS(listener->timesCalled, 0);
  }

  void testUnregisteringNullListener() {
    item->registerPropertyListener(listener);
    item->registerPropertyListener(0);
    item->pushToUndoStack();
    TS_ASSERT_EQUALS(listener->timesCalled, 1);
    TS_ASSERT_EQUALS(listener->getItem(), item);
  }

  void testRegisteringItemWithListenerLeadsToCallbackUponPushToUndoStack() {
    listener->registerItem(item);
    item->pushToUndoStack();
    TS_ASSERT_EQUALS(listener->getItem(), item);
    TS_ASSERT_EQUALS(listener->timesCalled, 1);
  }

  void testDoubleRegistrationViaListenerLeadsToOnlyOneCallback() {
    listener->registerItem(item);
    listener->registerItem(item);
    item->pushToUndoStack();
    TS_ASSERT_EQUALS(listener->timesCalled, 1);
  }

  void testReregisteringWorksViaListener() {
    listener->registerItem(item);
    item->pushToUndoStack();
    TS_ASSERT_EQUALS(listener->timesCalled, 1);

    GraphicsItemForTesting otherItem;
    listener->registerItem(&otherItem);
    item->pushToUndoStack();
    TS_ASSERT_EQUALS(listener->timesCalled, 1);

    otherItem.pushToUndoStack();
    TS_ASSERT_EQUALS(listener->timesCalled, 2);
  }

  void testReregisteringWorksViaItem() {
    item->registerPropertyListener(listener);
    item->pushToUndoStack();
    TS_ASSERT_EQUALS(listener->timesCalled, 1);

    GraphicsItemForTesting otherItem;
    otherItem.registerPropertyListener(listener);
    item->pushToUndoStack();
    TS_ASSERT_EQUALS(listener->timesCalled, 1);

    otherItem.pushToUndoStack();
    TS_ASSERT_EQUALS(listener->timesCalled, 2);
  }

  void testUnregistrationWhenItemDeleted() {
    item->registerPropertyListener(listener);
    delete item;
    item = 0;
    TS_ASSERT_EQUALS(listener->getItem(), nullptr);
  }

  void testUnregistrationWhenListenerDeleted() {
    item->registerPropertyListener(listener);
    delete listener;
    TS_ASSERT(!item->unregisterPropertyListener(listener)); // TODO add asserts to registering/unregistering
    listener = 0;
  }

  void testListenerDisconnectsUponAddingItemToScene() {
    item->registerPropertyListener(listener);
    TS_ASSERT_EQUALS(item->scene(), nullptr);
    TS_ASSERT_EQUALS(listener->getItem(), item);

    MolScene scene;
    scene.addItem(item);
    TS_ASSERT_EQUALS(item->scene(), &scene);
    TS_ASSERT_EQUALS(listener->getItem(), nullptr);
    scene.removeItem(item);
  }

  void testListenerDisconnectsUponRemovalFromScene() {
    MolScene scene;
    scene.addItem(item);
    item->registerPropertyListener(listener);
    TS_ASSERT_EQUALS(item->scene(), &scene);
    TS_ASSERT_EQUALS(listener->getItem(), item);

    scene.removeItem(item);
    TS_ASSERT_EQUALS(item->scene(), nullptr);
    TS_ASSERT_EQUALS(listener->getItem(), nullptr);
  }

  void testListenerDisconnectsUponChangeOfScene() {
    MolScene scene;
    scene.addItem(item);
    item->registerPropertyListener(listener);
    TS_ASSERT_EQUALS(item->scene(), &scene);
    TS_ASSERT_EQUALS(listener->getItem(), item);

    MolScene otherScene;
    otherScene.addItem(item);
    TS_ASSERT_EQUALS(item->scene(), &otherScene);
    TS_ASSERT_EQUALS(listener->getItem(), nullptr);
    otherScene.removeItem(item);
  }
};
