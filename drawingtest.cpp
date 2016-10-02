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
#include <QObject>
#include <QtTest>

#include <molscene.h>
#include <molview.h>
#include <actions/drawaction.h>
#include <QVector>

#include "drawingtest.h"

Q_DECLARE_METATYPE(QVector<QLineF>)


using namespace Molsketch;

DrawingTest::DrawingTest(QObject *parent) : QObject(parent)
{
}

DrawingTest::~DrawingTest()
{

}

void DrawingTest::initTestCase()
{
}

void DrawingTest::cleanupTestCase()
{
}

void DrawingTest::init()
{
  scene = new MolScene;
  view = new MolView(scene);
  action = new drawAction(scene);
  action->setChecked(true);
}

void DrawingTest::cleanup()
{
  delete view;
  delete scene;
}

void DrawingTest::testDrawing()
{
  QFETCH(QVector<QLineF>, lines);

  foreach(const QLineF& line, lines)
  {
    QTest::mousePress(view, Qt::LeftButton, 0,
                      view->mapFromScene(line.p1()));
    QTest::mouseRelease(view, Qt::LeftButton, 0,
                        view->mapFromScene(line.p2()));
  }

  // now do the checking...
}

void DrawingTest::testDrawing_data()
{
  QTest::addColumn<QVector<QLineF> >("lines");

  QTest::newRow("single atom") << (QVector<QLineF>()
                                   << QLineF(0,0,0,0)
                                   );
}
