/***************************************************************************
 *   Copyright (C) 2007 by Harm van Eersel                                 *
 *   devsciurus@xs4all.nl                                                  *
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

#ifndef STRINGIFY_H
#define STRINGIFY_H

#include <QString>
#include <QDataStream>



namespace Molsketch {

  template<class QDataStreamable>
  QDataStreamable makeFromString(const QString& data)
  {
    QByteArray ba(QByteArray::fromBase64(data.toUtf8()));
    QDataStream in(&ba, QIODevice::ReadOnly);
    QDataStreamable streamable;
    in >> streamable;
    return streamable;
  }

  template<class QDataStreamable>
  QString stringify(const QDataStreamable& streamable)
  {
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out << streamable;
    out.setDevice(nullptr);
    return ba.toBase64();
  }

  template<class T>
  QString stringify(const QList<T> list, QString (*transform)(const T&)) {
    QStringList output;
    for(T t : list) output << transform(t);
    return "[" + output.join(", ") + "]";
  }

} // namespace


#endif // STRINGIFY_H
