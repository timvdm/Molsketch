/***************************************************************************
 *   Copyright (C) 2007-2008 by Harm van Eersel                            *
 *   Copyright (C) 2009 Tim Vandermeersch                                  *
 *   Copyright (C) 2009 by Nicola Zonta                                    *
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

#include <QPair>
#include <QStringList>
#include "abstractxmlobject.h"
#include <QDebug>

namespace Molsketch {
  QXmlStreamReader &abstractXmlObject::readXml(QXmlStreamReader &in)
  {
    // read own attributes
    QXmlStreamAttributes attributes = in.attributes() ;
    readAttributes(attributes) ;

    // read children
    while (!in.atEnd())
    {
      in.readNext() ; // TODO probably we just want to go to the next start element here
      if (in.isEndElement()) break ;
      if (!in.isStartElement()) continue ;
      XmlObjectInterface* child = produceChild(in.name().toString(), in.attributes()) ;
      if (!child) continue ;
      child->readXml(in) ;
    }
    afterReadFinalization();
    return in ;
  }

  QXmlStreamWriter &abstractXmlObject::writeXml(QXmlStreamWriter &out) const
  {
    out.writeStartElement(xmlName()) ;
    out.writeAttributes(xmlAttributes()) ;
    foreach(const XmlObjectInterface* child, children())
      if (child) child->writeXml(out) ;
    out.writeEndElement();
    return out ;
  }

} // namespace

QDebug operator<<(QDebug debug, const QXmlStreamAttributes &attributes) {
  debug << "Attribute count:" << attributes.size();
  for (auto attribute : attributes) {
    debug << attribute.name() << "=" << attribute.value();
  }
  return debug;
}
