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

#include <QCoreApplication>
#include <QGraphicsSceneContextMenuEvent>
#include <QKeyEvent>
#include <QMimeData>
#include <molscene.h>
#include <cxxtest/TestSuite.h>

#include "scenesettings.h"
#include "settingsitem.h"

using namespace Molsketch;

// TODO move to settings test
class ScenePropertiesAcceptanceTest : public CxxTest::TestSuite {
  MolScene *scene;
public:
  void setUp() {
    scene = new MolScene;
  }

  void tearDown() {
    delete scene;
  }

  void testPropertyUnchangedOnEscapeKeyPress() {
    // TODO iterate over all properties
    scene->settings()->chargeVisible()->set(true);

    QKeyEvent *event = new QKeyEvent(QKeyEvent::KeyPress, Qt::Key_Escape, Qt::NoModifier);
    QCoreApplication::sendEvent(scene, event);
    delete event;
    TS_ASSERT(scene->settings()->chargeVisible()->get());
  }

  void testPropertyUnchangedOnDragEnterEvent() {
    scene->settings()->chargeVisible()->set(true);

    QGraphicsSceneDragDropEvent *event = new QGraphicsSceneDragDropEvent(QEvent::GraphicsSceneDragEnter);
    QMimeData *data = new QMimeData;
    data->setData(scene->mimeType(), QByteArray());
    event->setMimeData(data);
    event->setProposedAction(Qt::CopyAction);
    QCoreApplication::sendEvent(scene, event);
    delete event;
    TS_ASSERT(scene->settings()->chargeVisible()->get());
  }
};
