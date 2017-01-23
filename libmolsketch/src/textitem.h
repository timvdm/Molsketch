/***************************************************************************
 *   Copyright (C) 2017 by Hendrik Vennekate                               *
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

#ifndef MOLSKETCH_TEXTITEM_H
#define MOLSKETCH_TEXTITEM_H

#include "abstractxmlobject.h"
#include <QGraphicsTextItem>



namespace Molsketch {

  class TextItem : public QGraphicsTextItem, public abstractXmlObject
  {
  public:
    TextItem();

    // QGraphicsItem interface
  protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override
    {
    }
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override
    {
    }
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override
    {
    }
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override
    {
    }

    // abstractXmlObject interface
  public:
    QString xmlName() const override
    {
    }
  };

} // namespace Molsketch

#endif // MOLSKETCH_TEXTITEM_H
