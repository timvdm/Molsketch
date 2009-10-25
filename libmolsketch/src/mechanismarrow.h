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
#ifndef MECHANISMARROW_H
#define MECHANISMARROW_H

#include "graphicsitemtypes.h"

#include <QGraphicsItemGroup>

class QXmlStreamReader;
class QXmlStreamWriter;

namespace Molsketch {

  class MechanismArrowDialog;

  class MechanismArrow : public QGraphicsItem
  {
    public:
      // Public enums
      enum { Type = GraphicsItemTypes::MechanismArrowType };
      /**
       * @return the QGraphicsItem type of the class. Needed for Qt type casting.
       */
      int type() const {return Type; }

      enum ArrowType {
        SingleArrowRight = 0,
        SingleArrowLeft,
        DoubleArrow,
        SingleHookRight,
        SingleHookLeft,
        DoubleHook
      };

      /**
       * Default constructor.
       */
      MechanismArrow();
      virtual ~MechanismArrow();

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

      /**
       * Read arrow data from the specified XML stream.
       */
      void readXML(QXmlStreamReader &xml);
      /**
       * Write this arrow the the specified XML stream.
       */
      void writeXML(QXmlStreamWriter &xml);


    private:
      ArrowType m_arrowType;
      QPointF m_p1, m_p2, m_p3, m_p4;
      bool m_hoverP1, m_hoverP2, m_hoverP3, m_hoverP4;
      MechanismArrowDialog *m_dialog;
  };

}

#endif // REACTIONARROW_H
