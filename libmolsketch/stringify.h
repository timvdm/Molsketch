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
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef STRINGIFY_H
#define STRINGIFY_H

#include <QString>
#include <QDataStream>

#define READSTREAMABLE(STREAMABLE) \
  QByteArray ba(QByteArray::fromBase64(data.toUtf8())); \
  QDataStream in(&ba, QIODevice::ReadOnly); \
  in >> STREAMABLE;

namespace Molsketch {

  template<class QDataStreamable>
  QDataStreamable makeFromString(const QString& data)
  {
    QDataStreamable streamable;
    READSTREAMABLE(streamable)
    return streamable;
  }

  template<class QDataStreamable>
  QString stringify(const QDataStreamable& streamable)
  {
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out << streamable;
    out.setDevice(0);
    return ba.toBase64();
  }

} // namespace


#endif // STRINGIFY_H
