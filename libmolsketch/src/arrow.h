#ifndef ARROW_H
#define ARROW_H

#include "graphicsitem.h"

namespace Molsketch {

  class Arrow : public arrowGraphicsItem
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

    Arrow();
    ~Arrow();

    void setArrowType(ArrowType type);
    void setCoordinates(const QVector<QPointF> &c) ;
    QPolygonF coordinates() const ;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const ;

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

    void setPoint(const int& index, const QPointF& p) ;
    void setPoints(const QPolygonF&polygon);
    QPointF getPoint(const int& index) const;
    QPointF lastPoint() const;
    QPointF firstPoint() const;
    int coordinateCount() const ;
    void swapPoint(const int& index, QPointF &p) ;

protected:
    void readGraphicAttributes(const QXmlStreamAttributes &attributes) ;
    QXmlStreamAttributes graphicAttributes() const ;

  private:
    class privateData ;
    privateData *d ;
  };

} // namespace

Q_DECLARE_OPERATORS_FOR_FLAGS(Molsketch::Arrow::ArrowType)

#endif // ARROW_H
