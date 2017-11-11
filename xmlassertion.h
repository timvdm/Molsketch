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
#ifndef XMLASSERTION_H
#define XMLASSERTION_H

#include <QScopedPointer>
#include <QString>
#include <xmlobjectinterface.h>


class XmlAssertionPrivate;

class XmlAssertion
{
  Q_DECLARE_PRIVATE(XmlAssertion)
  QScopedPointer<XmlAssertionPrivate> d_ptr;
public:
  static XmlAssertion *assertThat(const QString& xml);
  static XmlAssertion *assertThat(const Molsketch::XmlObjectInterface &object);
  static QString formatXml(const QString&xml);
  XmlAssertion* contains(const QString& xQuery);
  XmlAssertion* exactlyOnceWithContent(const QString& expected);
  XmlAssertion *never();
private:
  XmlAssertion(const QString& xml);
};

namespace XmlAssert {
  XmlAssertion* assertThat(const QString& xml);
  XmlAssertion* assertThat(const Molsketch::XmlObjectInterface& object);
}

#endif // XMLASSERTION_H
