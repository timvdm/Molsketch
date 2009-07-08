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

#include <QString>
#include <QStringList>

#include "element.h"

#include <openbabel/data.h>

OpenBabel::OBElementTable eTable;

QString Molsketch::number2symbol( int number )
{
  return eTable.GetSymbol(number);
}

QString Molsketch::position2symbol( int row, int column )
{
  switch (row)
  {
    case 1 :
      if (column == 1) return eTable.GetSymbol(1);
      if (column == 18) return eTable.GetSymbol(2);
      return "";
    case 2 :
      if (column <= 2) return eTable.GetSymbol(2 + column);
      if (column >= 13) return eTable.GetSymbol(4 + column - 12);
      return "";
    case 3 :
      if (column <= 2) return eTable.GetSymbol(10 + column);
      if (column >= 13) return eTable.GetSymbol(12 + column - 12);
      return "";
    case 4 :
      return eTable.GetSymbol(18 + column);
    case 5 :
      return eTable.GetSymbol(36 + column);
    case 6 :
      if (column <= 2) return eTable.GetSymbol(54 + column);
      if (column >= 4) return eTable.GetSymbol(71 + column - 3);
      return "";
    case 7 :
      if (column <= 2) return eTable.GetSymbol(86 + column);
      if (column >= 4 and column <= 16) return eTable.GetSymbol(103 + column - 3);
      return "";
    case 8 :
      return "";
    case 9 :
      if (column <= 2) return "";
      if (column >= 3 and column < 18) return eTable.GetSymbol(57 + column - 3);
      return "";
    case 10:
      if (column <= 2) return "";
      if (column >= 3 and column < 18) return eTable.GetSymbol(89 + column - 3);
      return "";
    default:
      return "";
  }
}

int Molsketch::symbol2number( const QString &symbol )
{
  return eTable.GetAtomicNum(symbol.toAscii());;
//   for( int i = 0; i < m_size; i++) if (symbols()[i]==symbol) return i+1;
//   return -1;
}

double Molsketch::weightOfElement( int number )
{
  return  eTable.GetMass(number);
}

int Molsketch::numValenceElectrons(int element)
{
  // @todo implement other elements
  switch (element) {
    case 1: // H
    case 11: // Na
    case 19: // K
    case 37: // Rb
    case 87: // Fr
      return 1;
    case 4: // Be
    case 12: // Mg
    case 20: // Ca
    case 38: // Sr
    case 56: // Ba
    case 88: // Ra
      return 2;
    case 5: // B
    case 13: // Al
    case 31: // Ga
    case 49: // In
    case 81: // Tl
    case 113: // Uut 
      return 3;
    case 6: // C
    case 14: // Si
    case 32: // Ge
    case 50: // Sn
    case 82: // Pb
    case 114: // Uuq
      return 4;
    case 7: // N
    case 15: // P
    case 33: // As
    case 51: // Sb
    case 83: // Bi
    case 115: // Uup
      return 5;
    case 8: // O
    case 16: // S
    case 34: // Se
    case 52: // Te
    case 84: // Po
    case 116: // Uuh
      return 6; 
    case 9: // F
    case 17: // Cl
    case 35: // Br
    case 53: // I
    case 85: // At
    case 117: // Uus
      return 7;
    default:
      return 8; // makes charge 0
  }

}

int Molsketch::expectedValence(int element)
{
  // @todo implement other elements
  switch (element) {
    case 1: // H
    case 11: // Na
    case 19: // K
      return 1;
    case 4: // Be
    case 12: // Mg
    case 20: // Ca
      return 2;
    case 5: // B
    case 13: // Al
    case 31: // Ga
      return 3;
    case 6: // C
    case 14: // Si
    case 32: // Ge
      return 4;
    case 7: // N
    case 15: // P
    case 33: // As
      return 3;
    case 8: // O
    case 16: // S
    case 34: // Se
      return 2; 
    case 9: // F
    case 17: // Cl
    case 35: // Br
    case 53: // I
      return 1;
    default:
      return 0;  
  }
}

int Molsketch::getSize()
{
  return 111;
}
