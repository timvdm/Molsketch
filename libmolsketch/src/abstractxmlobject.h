/***************************************************************************
 *   Copyright (C) 2007-2008 by Harm van Eersel                            *
 *   Copyright (C) 2009 Tim Vandermeersch                                  *
 *   Copyright (C) 2009 by Nicola Zonta                                    *
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

#ifndef ABSTRACTXMLOBJECT_H
#define ABSTRACTXMLOBJECT_H

#include <QXmlStreamReader>
#include <QXmlStreamWriter>

namespace Molsketch {
  class abstractXmlObject
  {
  protected:
    virtual abstractXmlObject* produceChild(const QString& name, const QString& type) { Q_UNUSED(name) Q_UNUSED(type) return 0 ; }
    virtual void readAttributes(const QXmlStreamAttributes& attributes) { Q_UNUSED(attributes) }
    virtual QList<const abstractXmlObject*> children() const { return QList<const abstractXmlObject*>() ; }
    virtual QXmlStreamAttributes xmlAttributes() const { return QXmlStreamAttributes() ; }
    virtual QStringList textItemAttributes() const ;
  public:
    virtual QString xmlName() const = 0 ;
    abstractXmlObject();
    QXmlStreamReader& readXml(QXmlStreamReader& in) ;
    QXmlStreamWriter& writeXml(QXmlStreamWriter& out) const ;
    virtual ~abstractXmlObject() {}
  };

} // namespace


QXmlStreamReader& operator>>(QXmlStreamReader& in, Molsketch::abstractXmlObject& object) ;
QXmlStreamWriter& operator<<(QXmlStreamWriter& out, const Molsketch::abstractXmlObject& object) ;

#endif // ABSTRACTXMLOBJECT_H
