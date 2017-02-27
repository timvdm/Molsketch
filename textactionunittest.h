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

#include <actions/textaction.h>
#include <cxxtest/TestSuite.h>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QTest>
#include <commands.h>
#include <functional>
#include <molscene.h>
#include <textitem.h>
#include "utilities.h"

using namespace Molsketch;


class TextActionUnitTest : public CxxTest::TestSuite {
private:
  MolScene *scene;
  TextAction *textAction;
  QEvent *event;

  QGraphicsSceneEvent* processMousePressEvent(Qt::MouseButton button, Qt::KeyboardModifiers modifiers = Qt::NoModifier) {
    QGraphicsSceneMouseEvent *event = new QGraphicsSceneMouseEvent(QGraphicsSceneMouseEvent::MouseButtonPress);
    event->setButton(button);
    event->setModifiers(modifiers);
    event->setAccepted(false);
    textAction->mousePressEvent(event);
    return event;
  }

  QGraphicsSceneEvent* processMouseMoveEvent(Qt::MouseButton button, Qt::KeyboardModifiers modifiers = Qt::NoModifier) {
    QGraphicsSceneMouseEvent *event = new QGraphicsSceneMouseEvent(QGraphicsSceneMouseEvent::MouseMove);
    event->setButton(button);
    event->setModifiers(modifiers);
    event->setAccepted(false);
    textAction->mouseMoveEvent(event);
    return event;
  }

  QGraphicsSceneEvent* processMouseReleaseEvent(Qt::MouseButton button, Qt::KeyboardModifiers modifiers = Qt::NoModifier, QPointF position = QPointF()) {
    QGraphicsSceneMouseEvent *event = new QGraphicsSceneMouseEvent(QGraphicsSceneMouseEvent::MouseButtonRelease);
    event->setButton(button);
    event->setModifiers(modifiers);
    event->setScenePos(position);
    event->setAccepted(false);
    textAction->mouseReleaseEvent(event);
    return event;
  }

public:
  void setUp() {
    scene = new MolScene();
    textAction = new TextAction(scene);
    event = 0;
  }

  void tearDown() {
    delete scene;
    delete event;
  }

  void testMousePressingAcceptedForLeftButton() {
    event = processMousePressEvent(Qt::LeftButton);
    TS_ASSERT(event->isAccepted());
  }

  void testMousePressingRejectedForRightButton() {
    event = processMousePressEvent(Qt::RightButton);
    TS_ASSERT(!event->isAccepted());
  }

  void testMousePressingRejectedWithModifier() {
    event = processMousePressEvent(Qt::LeftButton, Qt::ShiftModifier);
    TS_ASSERT(!event->isAccepted());
  }

  void testMouseMovingAcceptedForLeftButton() {
    event =processMouseMoveEvent(Qt::LeftButton);
    TS_ASSERT(event->isAccepted());
  }

  void testMouseMovingRejectedForRightButton() {
    event = processMouseMoveEvent(Qt::RightButton);
    TS_ASSERT(!event->isAccepted());
  }

  void testMouseMovingRejectedForModifier() {
    event = processMouseMoveEvent(Qt::LeftButton, Qt::ShiftModifier);
    TS_ASSERT(!event->isAccepted());
  }

  void testMouseReleasingAcceptedForLeftButton() {
    event = processMouseReleaseEvent(Qt::LeftButton, Qt::NoModifier, QPointF(10.3, -15.1));
    TS_ASSERT(event->isAccepted());
    TS_ASSERT_EQUALS(1, scene->items().size());
    if (!scene->items().empty()) {
      TextItem* item = dynamic_cast<TextItem*>(scene->items().first());
      TS_ASSERT(item);
      QS_ASSERT_EQUALS(QPointF(10.3, -15.1), item->pos());
    }
    TS_ASSERT_EQUALS(1, scene->stack()->index());
    TS_ASSERT(dynamic_cast<const Commands::AddItem*>(scene->stack()->command(0)));
  }

  void testMouseReleasingRejectedForRightButton() {
    event = processMouseReleaseEvent(Qt::RightButton);
    TS_ASSERT(!event->isAccepted())
  }

  void testMouseReleasingRejectedForModifier() {
    event = processMouseReleaseEvent(Qt::LeftButton, Qt::ShiftModifier);
    TS_ASSERT(!event->isAccepted());
  }
};
