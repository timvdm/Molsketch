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
#include "graphicsitem.h"

#ifndef MOLSKETCH_FRAME_H
#define MOLSKETCH_FRAME_H

namespace Molsketch {

  class Frame : public graphicsItem
  {
  private:
    class privateData;
    privateData *d;
  public:
    enum { Type = FrameType };
    int type() const { return Type; }

    explicit Frame(QGraphicsItem* parent = 0);
    ~Frame();
    void setCoordinates(const QVector<QPointF> &c);
    QPolygonF coordinates() const;
    QPolygonF moveablePoints() const;
    void movePointBy(const QPointF &offset, int pointIndex);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;
//    QRectF boundingRect() const;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    int coordinateCount() const;

    void setFrameString(const QString& frameString);
    QString frameString() const;

    QString xmlName() const;
    QPointF getPoint(const int &index) const;
  protected:
    void readGraphicAttributes(const QXmlStreamAttributes &attributes);
    QXmlStreamAttributes graphicAttributes() const;
    void prepareContextMenu(QMenu *contextMenu);
    abstractXmlObject* produceChild(const QString &name, const QString &type);
    QList<const abstractXmlObject*> children() const;
  private:
    qreal sceneLineWidth(MolScene *scene) const;
  }; // TODO let classes start with capital letters.

} // namespace Molsketch

#endif // MOLSKETCH_FRAME_H
