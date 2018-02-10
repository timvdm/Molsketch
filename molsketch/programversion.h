/***************************************************************************
 *   Copyright (C) 2018 by Hendrik Vennekate, HVennekate@gmx.de            *
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

#ifndef PROGRAMVERSION_H
#define PROGRAMVERSION_H

#include <QString>



class ProgramVersion
{
public:
  ProgramVersion(const QString& version = "");
  ProgramVersion(int major, int minor = 0, int patch = 0, int build = 0);
  ~ProgramVersion();
  unsigned int major() const;
  unsigned int minor() const;
  unsigned int patch() const;
  unsigned int build() const;
  bool operator ==(const ProgramVersion& other) const;
  bool operator !=(const ProgramVersion& other) const;
  bool operator <(const ProgramVersion& other) const;
  bool operator >(const ProgramVersion& other) const;
  bool operator <=(const ProgramVersion& other) const;
  bool operator >=(const ProgramVersion& other) const;
  QString toString() const;
private:
  class ProgramVersionPrivate;
  ProgramVersionPrivate *d;
};

#endif // PROGRAMVERSION_H
