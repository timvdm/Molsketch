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
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <QPair>
#include <QStringList>
#include "abstractxmlobject.h"

namespace Molsketch {
  QStringList abstractXmlObject::textItemAttributes() const { return QStringList(); }

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
      if (textItemAttributes().contains(in.name().toString())) // TODO actually deal with legacy "W" and "H" stereo bonds
        // TODO get legacy files for reference
      {
        attributes.append(in.name().toString(), in.readElementText()) ;
        continue ;
      }
      XmlObjectInterface* child = produceChild(in.name().toString(), in.attributes().value("type").toString()) ;
      if (!child) continue ;
      child->readXml(in) ;
    }
    afterReadFinalization();
    return in ;
  }

  QXmlStreamWriter &abstractXmlObject::writeXml(QXmlStreamWriter &out) const
  {
    out.writeStartElement(xmlName()) ;
    QXmlStreamAttributes attributes = xmlAttributes() ;
    typedef QPair<QString, QString> stringpair ;
    QList<stringpair> textItems ;
    foreach(const QString& attribute, textItemAttributes()) // This is total nonsense, but was necessary...
    {
      if (!attributes.hasAttribute(attribute)) continue ;
      textItems << qMakePair(attribute, attributes.value(attribute).toString()) ;
      for (int i = 0 ; i < attributes.size() ; ++i)
        if (attributes[i].qualifiedName() == attribute)
          attributes.remove(i--) ;
    }

    out.writeAttributes(xmlAttributes()) ;
    foreach(const XmlObjectInterface* child, children())
      if (child) child->writeXml(out) ;

    foreach(const stringpair& textItem, textItems) // TODO get rid of this!!
      out.writeTextElement(textItem.first, textItem.second) ;
    out.writeEndElement();
    return out ;
  }

} // namespace

