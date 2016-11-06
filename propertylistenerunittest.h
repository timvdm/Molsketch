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

class PropertyListenerForTesting : public PropertyWidget
{
  void reactToPropertyChange() { ++ timesPropertyChanged; }
  void transferProperty() {
    TS_ASSERT(item());
    ((GraphicsItemForTesting*) item())->pushToUndoStack();
    ++ timesPropertyApplied;
  }
public:
  int timesPropertyChanged;
  int timesPropertyApplied;
  PropertyListenerForTesting() : timesPropertyChanged(0), timesPropertyApplied(0) {}
  graphicsItem* getItem() { return item(); }
};

class PropertyListenerUnitTest : public CxxTest::TestSuite
{
private:
  PropertyListenerForTesting* listener;
  GraphicsItemForTesting* item;

  void assertItemPropertyChangedPropertyApplied( graphicsItem* item,
                                         int changeCount,
                                         int appliedCount) {
    TS_ASSERT_EQUALS(listener->getItem(), item);
    TS_ASSERT_EQUALS(listener->timesPropertyChanged, changeCount);
    TS_ASSERT_EQUALS(listener->timesPropertyApplied, appliedCount);
  }

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
    assertItemPropertyChangedPropertyApplied(nullptr, 0, 0);
  }

  void testRegisteringNullListener() {
    item->registerPropertyListener(listener);
    item->registerPropertyListener(0);
    item->pushToUndoStack();
    assertItemPropertyChangedPropertyApplied(item, 2, 0);
  }

  void testRegisteredListenerIsCalledWhenPushedToUndoStack() {
    item->registerPropertyListener(listener);
    item->pushToUndoStack();
    assertItemPropertyChangedPropertyApplied(item, 2, 0);
  }

  void testRegisteringTwiceLeadsToOnlyOneCallToListener() {
    item->registerPropertyListener(listener);
    item->registerPropertyListener(listener);
    item->pushToUndoStack();
    assertItemPropertyChangedPropertyApplied(item, 2, 0);
  }

  void testListenerKnowsWhatItemToListenTo() {
    item->registerPropertyListener(listener);
    assertItemPropertyChangedPropertyApplied(item, 1, 0);
  }

  void testUnregisteringViaItem() {
    item->registerPropertyListener(listener);
    item->unregisterPropertyListener(listener);
    item->pushToUndoStack();
    assertItemPropertyChangedPropertyApplied(nullptr, 2, 0);
  }

  void testUnregisteringViaListener() {
    listener->registerItem(item);
    listener->registerItem(0);
    item->pushToUndoStack();
    assertItemPropertyChangedPropertyApplied(nullptr, 2, 0);
  }

  void testUnregisteringNullListener() {
    item->registerPropertyListener(listener);
    item->registerPropertyListener(0);
    item->pushToUndoStack();
    assertItemPropertyChangedPropertyApplied(item, 2, 0);
  }

  void testRegisteringItemWithListenerLeadsToCallbackUponPushToUndoStack() {
    listener->registerItem(item);
    item->pushToUndoStack();
    assertItemPropertyChangedPropertyApplied(item, 2, 0);
  }

  void testDoubleRegistrationViaListenerLeadsToOnlyOneCallback() {
    listener->registerItem(item);
    listener->registerItem(item);
    item->pushToUndoStack();
    assertItemPropertyChangedPropertyApplied(item, 2, 0);
  }

  void testReregisteringWorksViaListener() {
    listener->registerItem(item);
    item->pushToUndoStack();
    assertItemPropertyChangedPropertyApplied(item, 2, 0);

    GraphicsItemForTesting otherItem;
    listener->registerItem(&otherItem);
    item->pushToUndoStack();
    assertItemPropertyChangedPropertyApplied(&otherItem, 3, 0);

    otherItem.pushToUndoStack();
    assertItemPropertyChangedPropertyApplied(&otherItem, 4, 0);
  }

  void testReregisteringWorksViaItem() {
    item->registerPropertyListener(listener);
    item->pushToUndoStack();
    assertItemPropertyChangedPropertyApplied(item, 2, 0);

    GraphicsItemForTesting otherItem;
    otherItem.registerPropertyListener(listener);
    item->pushToUndoStack();
    assertItemPropertyChangedPropertyApplied(&otherItem, 3, 0);

    otherItem.pushToUndoStack();
    assertItemPropertyChangedPropertyApplied(&otherItem, 4, 0);
  }

  void testUnregistrationWhenItemDeleted() {
    item->registerPropertyListener(listener);
    delete item;
    item = 0;
    assertItemPropertyChangedPropertyApplied(nullptr, 2, 0);
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
    assertItemPropertyChangedPropertyApplied(item, 1, 0);

    MolScene scene;
    scene.addItem(item);
    TS_ASSERT_EQUALS(item->scene(), &scene);
    assertItemPropertyChangedPropertyApplied(nullptr, 2, 0);
    scene.removeItem(item);
  }

  void testListenerDisconnectsUponRemovalFromScene() {
    MolScene scene;
    scene.addItem(item);
    item->registerPropertyListener(listener);
    TS_ASSERT_EQUALS(item->scene(), &scene);
    assertItemPropertyChangedPropertyApplied(item, 1, 0);

    scene.removeItem(item);
    TS_ASSERT_EQUALS(item->scene(), nullptr);
    assertItemPropertyChangedPropertyApplied(nullptr, 2, 0);
  }

  void testListenerDisconnectsUponChangeOfScene() {
    MolScene scene;
    scene.addItem(item);
    item->registerPropertyListener(listener);
    TS_ASSERT_EQUALS(item->scene(), &scene);
    assertItemPropertyChangedPropertyApplied( item, 1, 0);

    MolScene otherScene;
    otherScene.addItem(item);
    TS_ASSERT_EQUALS(item->scene(), &otherScene);
    assertItemPropertyChangedPropertyApplied(nullptr, 2, 0);
    otherScene.removeItem(item);
  }

  void testApplyingProperty() {
    item->registerPropertyListener(listener);
    assertItemPropertyChangedPropertyApplied(item, 1, 0);

    listener->applyProperty();
    assertItemPropertyChangedPropertyApplied(item, 1, 1);
  }

  void testApplyingPropertyWithNullItem() {
    listener->registerItem(0);
    listener->applyProperty();
    assertItemPropertyChangedPropertyApplied(nullptr, 0, 0);
  }
};
