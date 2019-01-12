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

#ifndef ABSTRACTXMLOBJECT_H
#define ABSTRACTXMLOBJECT_H

#include "xmlobjectinterface.h"

namespace Molsketch {
  class abstractXmlObject : public XmlObjectInterface
  {
  protected:
    // TODO should be factory function
    virtual XmlObjectInterface* produceChild(const QString& name, const QXmlStreamAttributes& attributes) { Q_UNUSED(name) Q_UNUSED(attributes) return 0 ; }
    virtual void readAttributes(const QXmlStreamAttributes& attributes) { Q_UNUSED(attributes) }
    virtual QList<const XmlObjectInterface*> children() const { return QList<const XmlObjectInterface*>() ; }
    virtual QXmlStreamAttributes xmlAttributes() const { return QXmlStreamAttributes() ; }
    virtual void afterReadFinalization() {}
  public:
    virtual QString xmlName() const = 0 ;
    abstractXmlObject() {}
    QXmlStreamReader& readXml(QXmlStreamReader& in) override final;
    QXmlStreamWriter& writeXml(QXmlStreamWriter& out) const override final;
    virtual ~abstractXmlObject() {}
  };

} // namespace

class QDebug;
QDebug operator<<(QDebug debug, const QXmlStreamAttributes& attributes);

#endif // ABSTRACTXMLOBJECT_H
