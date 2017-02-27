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

#include "textitem.h"

namespace Molsketch {

  TextItem::TextItem(GRAPHICSSCENESOURCE)
    : QGraphicsTextItem(GRAPHICSSCENEINIT)
  {
    setFlags(flags()
             | ItemAcceptsInputMethod
             | ItemIsFocusable
             | ItemIsMovable
             | ItemIsSelectable);
    setTextInteractionFlags(Qt::TextEditorInteraction);
  }

  QXmlStreamReader &TextItem::readXml(QXmlStreamReader &in)
  {
    QString coordString = in.attributes().value("coordinates").toString();
    setPos(coordString.section(",",0,0).toDouble(),
           coordString.section(",",1,1).toDouble()); // TODO static utility function
    setHtml(in.readElementText());
  }

  QXmlStreamWriter &TextItem::writeXml(QXmlStreamWriter &out) const
  {
    out.writeStartElement("textItem");
    out.writeAttribute("coordinates", QString::number(pos().x()) +"," + QString::number(pos().y())); // TODO static utility function for this
    out.writeCDATA(toHtml());
    out.writeEndElement();
  }
} // namespace Molsketch
