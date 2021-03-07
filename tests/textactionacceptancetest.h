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
#include <molscene.h>

using namespace Molsketch;

class TextActionAcceptanceTest : public CxxTest::TestSuite {
public:
  void setUp() {
  }

  void tearDown() {
  }

  void testInsertingTextItem() {
    TS_SKIP("TODO");
//    QGraphicsView view(scene);
//    view.show();
//    view.activateWindow();
//    view.setFocus();
//    event = processMouseReleaseEvent(Qt::LeftButton, Qt::NoModifier, QPointF(10.3, -15.1));
//    TS_ASSERT(event->isAccepted());
//    TS_ASSERT_EQUALS(1, scene->items().size());
//    if (!scene->items().empty()) {
//      TextItem* item = dynamic_cast<TextItem*>(scene->items().first());
//      TS_ASSERT(item);
//      if (item) TS_ASSERT(item->hasFocus()); // TODO check why this does not work
//      QS_ASSERT_EQUALS(QPointF(10.3, -15.1), item->pos());
//    }
//    TS_ASSERT_EQUALS(1, scene->stack()->index());
//    TS_ASSERT(dynamic_cast<const Commands::AddItem*>(scene->stack()->command(0)));
//    qDebug() << view.hasFocus() << view.isActiveWindow() << view.focusPolicy();
  }
};
