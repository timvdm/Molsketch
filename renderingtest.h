/***************************************************************************
 *   Copyright (C) 2016 Hendrik Vennekate                                  *
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
#ifndef RENDERINGTEST_H
#define RENDERINGTEST_H

#include <QObject>
#include <QTest>

namespace Molsketch {
  class MolScene;
}

class RenderingTest : public QObject
{
  Q_OBJECT
public:
  explicit RenderingTest(QObject *parent = 0);
  ~RenderingTest();

private slots:
  void initTestCase();
  void cleanupTestCase();
  void init();
  void cleanup();

  void testRenderBonds();
  void testRenderBonds_data();
private:
  Molsketch::MolScene *scene;

};

#endif // RENDERINGTEST_H
