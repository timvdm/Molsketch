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

#include "xmlobjectinterface.h"
#include "qtversionmacros.h"
#include <QGraphicsTextItem>

namespace Molsketch {

  class TextItem : public QGraphicsTextItem, public XmlObjectInterface
  {
  public:
    explicit TextItem(GRAPHICSSCENEHEADER);

    // QGraphicsItem interface
  protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void focusInEvent(QFocusEvent *event);

    // XmlObjectInterface interface
  public:
    QXmlStreamReader& readXml(QXmlStreamReader& in);
    QXmlStreamWriter& writeXml(QXmlStreamWriter& out) const;
    static QString xmlClassName();
  private:
    class privateData;
    privateData *d;
  };

} // namespace Molsketch

#endif // MOLSKETCH_TEXTITEM_H
