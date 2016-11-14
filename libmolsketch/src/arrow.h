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
#ifndef ARROW_H
#define ARROW_H

#include "graphicsitem.h"

namespace Molsketch {

  class Arrow : public graphicsItem // TODO introduce dependent coordinates
  {
  public:
    enum {Type = GeneralArrowType} ;
    int type() const {return Type;}

    enum ArrowTypeParts {
      NoArrow = 0,
      UpperBackward = 1,
      LowerBackward = 2,
      UpperForward = 4,
      LowerForward = 8
    };

    Q_DECLARE_FLAGS(ArrowType, ArrowTypeParts)

    explicit Arrow(QGraphicsItem* parent = 0);
    struct Properties
    {
      Arrow::ArrowType arrowType;
      QPolygonF points;
      bool spline ;
    };

    ~Arrow();
    static bool splinePossible(const int& numberOfPoints);

    bool getSpline() const;
    void setSpline(const bool& value);
    void setArrowType(const ArrowType& type);
    void setArrowTipPart(const ArrowTypeParts& part);
    void unsetArrowTipPart(const ArrowTypeParts& part);
    ArrowType getArrowType() const;
    void setCoordinates(const QVector<QPointF> &c) ;
    QPolygonF coordinates() const ;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const ;

    void setPoint(const int& index, const QPointF& p) ;
    void setPoints(const QPolygonF& polygon);
    QPointF getPoint(const int& index) const;
    QPointF lastPoint() const;
    QPointF firstPoint() const;
    int coordinateCount() const ;
    void swapPoint(const int& index, QPointF &p) ;

    QString xmlName() const;

    Properties getProperties() const;
    QWidget* getPropertiesWidget();
    void setProperties(const Properties& p);
  protected:
    void readGraphicAttributes(const QXmlStreamAttributes &attributes) ;
    QXmlStreamAttributes graphicAttributes() const ;
    void prepareContextMenu(QMenu *contextMenu);

  private:
    class privateData ;
    privateData *d ;
    qreal sceneLineWidth(MolScene *scene) const;
  };

} // namespace

Q_DECLARE_OPERATORS_FOR_FLAGS(Molsketch::Arrow::ArrowType)

#endif // ARROW_H
