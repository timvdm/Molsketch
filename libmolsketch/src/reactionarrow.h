/***************************************************************************
 *   Copyright (C) 2009 Tim Vandermeersch                                  *
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
#ifndef REACTIONARROW_H
#define REACTIONARROW_H

#include "graphicsitemtypes.h"
#include <QXmlStreamWriter>

#include "graphicsitem.h"

//class QCoreXmlStreamReader;
//class QCoreXmlStreamWriter;

namespace Molsketch {

  class ReactionArrowDialog;

  class ReactionArrow : public arrowGraphicsItem
  {
    public:
      // Public enums
      enum { Type = GraphicsItemTypes::ReactionArrowType };
      /**
       * @return the QGraphicsItem type of the class. Needed for Qt type casting.
       */
      int type() const {return Type; }

      enum ArrowType {
        SingleArrow = 0,
        DoubleArrow,
        Equilibrium,
        EqRightShifted,
        EqLeftShifted
      };

      /**
       * Default constructor.
       */
      ReactionArrow();
      virtual ~ReactionArrow();

      QRectF boundingRect() const;
      void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

      void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
      void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
      void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

      void mousePressEvent(QGraphicsSceneMouseEvent *event);
      void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
      void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

      void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

      void setArrowType(ArrowType type);

  protected:
      void readGraphicAttributes(const QXmlStreamAttributes &attributes) ;
      QXmlStreamAttributes graphicAttributes() const ;

    private:
      ArrowType m_arrowType;
      QPointF m_end;
      bool m_hoverBegin, m_hoverEnd;
      ReactionArrowDialog *m_dialog;
  };

}

#endif // REACTIONARROW_H
