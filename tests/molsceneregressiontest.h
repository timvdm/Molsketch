/***************************************************************************
 *   Copyright (C) 2018 by Hendrik Vennekate                               *
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
#include <QGraphicsSceneMouseEvent>
#include <molscene.h>
#include <actions/drawaction.h>
#include <actions/reactionarrowaction.h>

using namespace Molsketch;

class MolSceneRegressionTest : public CxxTest::TestSuite {

  void sendMouseEvent(QGraphicsSceneMouseEvent::Type type,
                      const QPointF& buttonDownScenePos,
                      const QPointF& scenePos,
                      genericAction* action,
                      void (genericAction::*mouseEvent)(QGraphicsSceneMouseEvent *)) {
    auto event = new QGraphicsSceneMouseEvent(type);
    event->setScenePos(scenePos);
    event->setButton(Qt::LeftButton);
    event->setButtonDownScenePos(Qt::LeftButton, buttonDownScenePos);
    (action->*mouseEvent)(event);
    delete event;
  }

  void performPressMoveRelease(genericAction *action) {
    sendMouseEvent(QGraphicsSceneMouseEvent::MouseButtonPress, QPointF(0,0), QPointF(0, 0), action, &genericAction::mousePressEvent);
    sendMouseEvent(QGraphicsSceneMouseEvent::MouseMove, QPointF(0,0), QPointF(30, 30), action, &genericAction::mouseMoveEvent);
    sendMouseEvent(QGraphicsSceneMouseEvent::MouseButtonRelease, QPointF(0,0), QPointF(30, 30), action, &genericAction::mouseReleaseEvent);
  }

public:
  void testCrashAfterUndoingDrawAction() {
    auto scene = new MolScene;
    auto drawingAction = new drawAction(scene);
    drawingAction->toggle();

    qDebug() << scene->items();
    performPressMoveRelease(drawingAction);

    for (auto item : scene->items()) qDebug() << "after drawing" << dynamic_cast<graphicsItem*>(item)->xmlName();

    scene->stack()->undo();

    for (auto item : scene->items()) qDebug() << "after undoing" << dynamic_cast<graphicsItem*>(item)->xmlName();

    auto arrowAction = new reactionArrowAction(scene);
    arrowAction->toggle();

    qDebug() << drawingAction->isChecked() << arrowAction->isChecked();

    performPressMoveRelease(arrowAction);

    for (auto item : scene->items()) qDebug() << "after arrow" << dynamic_cast<graphicsItem*>(item)->xmlName();

    delete scene;
  }
};
