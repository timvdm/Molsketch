#ifndef REACTIONARROW_H
#define REACTIONARROW_H

#include "graphicsitemtypes.h"

#include <QGraphicsItemGroup>

namespace Molsketch {

  class ReactionArrow : public QGraphicsItem
  {
    public:
      // Public enums
      enum { Type = GraphicsItemTypes::ReactionArrowType };
      /**
       * @return the QGraphicsItem type of the class. Needed for Qt type casting.
       */
      int type() const {return Type; }

      enum ArrowType {
        FilledArrow = 0,
        Equilibrium,
        EqRightShifted,
        EqLeftShifted
      };

      /**
       * Default constructor.
       */
      ReactionArrow();

      QRectF boundingRect() const;

      void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

      void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
      void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
      void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

      void mousePressEvent(QGraphicsSceneMouseEvent *event);
      void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
      void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    private:
      ArrowType m_arrowType;
      QPointF m_end;
      bool m_hoverBegin, m_hoverEnd;
  };

}

#endif // REACTIONARROW_H
