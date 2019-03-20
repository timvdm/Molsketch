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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/
#ifndef MOLSKETCH_GRID_H
#define MOLSKETCH_GRID_H

#include <QGraphicsItem>

namespace Molsketch {

  class SceneSettings;

  class Grid : public QGraphicsItem // TODO this should really be done in QGraphicsScene::drawBackground
  {
  public:
    Grid(SceneSettings *settings);
    ~Grid();
    virtual QPointF alignPoint(const QPointF& point);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;
  private:
    class privateData;
    privateData *d;
  };

} // namespace Molsketch

#endif // MOLSKETCH_GRID_H
