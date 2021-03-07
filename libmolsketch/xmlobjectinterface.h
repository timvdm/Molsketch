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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/
#ifndef XMLOBJECTINTERFACE_H
#define XMLOBJECTINTERFACE_H

#include <QXmlStreamReader>
#include <QXmlStreamWriter>

namespace Molsketch {
  class XmlObjectInterface {
  public:
    virtual QXmlStreamReader& readXml(QXmlStreamReader& in) = 0;
    virtual QXmlStreamWriter& writeXml(QXmlStreamWriter& out) const = 0;
    virtual ~XmlObjectInterface() {}
  };
  XmlObjectInterface *produceXmlObject(const QString &type);
}

QXmlStreamReader& operator>>(QXmlStreamReader& in, Molsketch::XmlObjectInterface& object);
QXmlStreamWriter& operator<<(QXmlStreamWriter& out, const Molsketch::XmlObjectInterface& object);

#endif // XMLOBJECTINTERFACE_H
