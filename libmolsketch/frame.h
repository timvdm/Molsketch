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
    int type() const override { return Type; }

    explicit Frame(QGraphicsItem* parent = 0);
    ~Frame();
    void setCoordinates(const QVector<QPointF> &c) override;
    QPolygonF coordinates() const override;
    QPolygonF moveablePoints() const override;
    void movePointBy(const QPointF &offset, int pointIndex) override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;
//    QRectF boundingRect() const;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    int coordinateCount() const override;

    void setFrameString(const QString& frameString);
    QString frameString() const;

    QString xmlName() const override;
    static QString xmlClassName();
    QPointF getPoint(const int &index) const override;
  protected:
    void readGraphicAttributes(const QXmlStreamAttributes &attributes) override;
    QXmlStreamAttributes graphicAttributes() const override;
    void prepareContextMenu(QMenu *contextMenu) override;
    abstractXmlObject* produceChild(const QString &name, const QXmlStreamAttributes &attributes) override;
    QList<const XmlObjectInterface *> children() const override;
  private:
    qreal sceneLineWidth(MolScene *scene) const override;
  }; // TODO let classes start with capital letters.

} // namespace Molsketch

#endif // MOLSKETCH_FRAME_H
