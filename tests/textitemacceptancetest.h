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
#include <QGraphicsView>
#include <QTest>
#include <commands.h>
#include <molscene.h>
#include <molview.h>
#include <textitem.h>
#include "utilities.h"

using namespace Molsketch;

const QString& initialText("InitialText");
const QPointF& initialPos(QPointF(3.5, 5.3));

class TextItemAcceptanceTest : public CxxTest::TestSuite {
private:
  MolScene *scene;
  MolView *view;
  TextItem *item;

  void moveItemByMouse(QPointF moveBy) {
    QPointF itemCenter(item->boundingRect().center());
    QPointF newItemCenter(itemCenter + moveBy);
    QTest::mousePress(view->viewport(), Qt::LeftButton, Qt::NoModifier, view->mapFromScene(itemCenter));
    mouseMoveEvent(view->viewport(), Qt::LeftButton, Qt::NoModifier, view->mapFromScene(newItemCenter));
    QTest::mouseRelease(view->viewport(), Qt::LeftButton, Qt::NoModifier, view->mapFromScene(newItemCenter));
  }

  void assertItemPosAndTextAndUndo(const QPointF& pos, const QString& plainText, int stackIndex) {
    QS_ASSERT_EQUALS(pos, item->pos());
    QS_ASSERT_EQUALS(plainText, item->toPlainText());
    TS_ASSERT_EQUALS(stackIndex, scene->stack()->index());
    scene->stack()->undo();
    QS_ASSERT_EQUALS(initialPos, item->pos());
    QS_ASSERT_EQUALS(initialText, item->toPlainText());
  }

  void appendTextToItem(const QString& text) {
//    QTest::mouseClick(view->viewport(), Qt::LeftButton, Qt::NoModifier, view->mapFromScene(item->boundingRect().center()),1000);
    QFocusEvent focusIn(QFocusEvent::FocusIn);
    item->setFocus();
    QTest::keyClick(view->viewport(), Qt::Key_End);
    QTest::keyClicks(view->viewport(), text);
//    QTest::mouseClick(view->viewport(), Qt::LeftButton, Qt::NoModifier, view->mapFromScene(item->boundingRect().bottomRight() + QPointF(10,10)),1000);
  }

public:
  void setUp() {
    scene = new MolScene();

    item = new TextItem;
    item->setPlainText(initialText);
    item->setPos(initialPos);
    scene->addItem(item);

    view = new MolView(scene);
    view->show();
  }

  void tearDown() {
    delete view;
    delete scene;
  }

  void testMovingItem() {
    QPointF offset(4,5);
    moveItemByMouse(offset);
    assertItemPosAndTextAndUndo(initialPos + offset, initialText, 1);
  }

  void testEditing() {
    TS_SKIP("TODO");
    qDebug() << item->hasFocus() << view->hasFocus();
    QTest::mouseClick(view->viewport(), Qt::LeftButton, Qt::NoModifier, view->mapFromScene(item->boundingRect().topLeft()));
    item->setFocus();
    qDebug() << item->hasFocus();
    const QString& extension(" extended");
    appendTextToItem("");
    appendTextToItem(extension);
    assertItemPosAndTextAndUndo(initialPos, initialText + extension, 1);
  }
};
