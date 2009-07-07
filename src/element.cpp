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

// static const int m_size = 111;
// const QString symbols[number];
const QList<double> weights()
  {
    // Used the values from Kalzium
    QList<double> m_weights;
    m_weights << 0;
    m_weights << 1.008 << 4.003 << 6.941 << 9.021 << 10.81 << 12.01 << 14.01 << 16 << 19 << 20.18;
    m_weights << 22.99 << 24.31 << 26.98 << 30.97 << 32.06 << 35.45 << 39.95 << 39.1 << 40.08 << 44.96; // 20
    m_weights << 44.96 << 47.87 << 50.94 << 52 << 54.94 << 55.85 << 58.93 << 58.69 << 63.55 << 65.39;
    m_weights << 69.72 << 72.64 << 74.92 << 78.96 << 79.9 << 83.8 << 84.47 << 87.62 << 88.91 << 91.22; // 40
    m_weights << 92.91 << 95.94 << 98.91 << 101.1 << 102.9 << 106.4 << 107.9 << 112.4 << 114.8 << 118.7;
    m_weights << 121.8 << 127.6 << 126.9 << 131.3 << 132.9 << 137.2 << 138.9 << 140.1 << 140.9 << 144.2; // 60
    m_weights << 146.9 << 150.4 << 152 << 157.3 << 158.9 << 162.5 << 164.9 << 167.3 << 168.9 << 173;
    m_weights << 175 << 178.5 << 180.9 << 183.8 << 186.2 << 190.2 << 192.2 << 195.1 << 197 << 200.6; // 80
    m_weights << 204.4 << 207.2 << 209 << 209 << 210 << 222 << 223 << 226 << 227 << 232;
    m_weights << 231 << 238 << 237 << 244.1 << 243.1 << 247.1 << 247.1 << 251.1 << 252.1 << 257.1; //100
    m_weights << 258.1 << 259.1 << 262.1 << 261.1 << 262.1 << 263.1 << 262.1 << 265.1 << 266.1 << 271;
    m_weights << 272;							   //111
    
    return m_weights;
  };

const QList<int> oxidation()
 {
  QList<int> m_oxidation;
  m_oxidation << 0;
  m_oxidation << 1 										   << 0;
  m_oxidation << 1 << 2 						  << 3 << 4 << 4 <<-2 <<-1 << 0;
  m_oxidation << 1 << 2 						  << 3 << 4 << 5 << 6 <<-1 << 0;
  m_oxidation << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 3 << 2 << 2 << 2 << 2 << 3 << 4 << 3 << 4 <<-1 << 0;
  m_oxidation << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 4 << 4 << 4 << 1 << 2 << 3 << 4 << 3 << 4 <<-1 << 0;
  m_oxidation << 1 << 2;
  m_oxidation 		<< 3 << 3 << 3 << 3 << 3 << 3 << 3 << 3 << 4 << 3 << 3 << 3 << 3 << 3 << 3;
  m_oxidation 		     << 4 << 5 << 6 << 6 << 4 << 4 << 4 << 3 << 2 << 1 << 2 << 3 << 4 <<-1 << 0;
  m_oxidation << 1 << 2;
  m_oxidation 		<< 3 << 4 << 5 << 6 << 5 << 4 << 3 << 3 << 4 << 3 << 3 << 3 << 3 << 2 << 3;
  m_oxidation 		     << 4 << 5 << 6 << 7 << 3 << 3 << 4 << 3 << 2 << 3 << 4 << 3 << 4 << -1 << 0;

  return m_oxidation;
 }

const QList<int> valency()
{
    QList<int> m_valency;
    m_valency << -1;
    m_valency << 1 										   << 0;
    m_valency << 1 << 2 						  << 3 <<-4 <<-3 <<-2 <<-1 << 0;
    m_valency << 1 << 2 				 		  << 3 << 4 <<-3 <<-2 <<-1 << 0;
    m_valency << 1 << 0 << 1 << 2 << 3 << 4 << 5 << 2 << 2 << 2 << 0 << 1 << 2 << 1 << 4 <<-3 <<-2 <<-1;
    m_valency << 0 << 1 << 2 << 0 << 1 << 2 << 2 << 2 << 2 << 2 << 2 << 2 << 2 << 0 << 1 << 2 << 2 << 2;
    m_valency << 2 << 2;
    m_valency 		<< 2 << 2 << 2 << 0 << 1 << 2 << 2 << 2 << 2 << 2 << 2 << 2 << 2 << 0;
    m_valency << 1 << 2 << 2 << 2 << 2 << 2 << 2 << 2 << 2 << 0;	// 80
    m_valency << 2 << 2 << 2 << 2 << 2 << 2 << 2 << 2 << 2 << 0;
    m_valency << 2 << 2 << 2 << 2 << 2 << 2 << 2 << 2 << 2 << 0;	//100
    m_valency << 2 << 2 << 2 << 2 << 2 << 2 << 2 << 2 << 2 << 0;
    m_valency << 0;							//111
    
    return m_valency;
  };
  
const QStringList symbols() 
{
	QStringList m_symbols;
	m_symbols.append("R");
	// 1
        m_symbols.append("H");
        m_symbols.append("He");
        m_symbols.append("Li");
        m_symbols.append("Be");
        m_symbols.append("B");
        m_symbols.append("C");
        m_symbols.append("N");
        m_symbols.append("O");
        m_symbols.append("F");
        // 10
        m_symbols.append("Ne");
        m_symbols.append("Na");
        m_symbols.append("Mg");
        m_symbols.append("Al");
        m_symbols.append("Si");
        m_symbols.append("P");
        m_symbols.append("S");
        m_symbols.append("Cl");
        m_symbols.append("Ar");
        m_symbols.append("K");
        // 20
        m_symbols.append("Ca");
        m_symbols.append("Sc");
        m_symbols.append("Ti");
        m_symbols.append("V");
        m_symbols.append("Cr");
        m_symbols.append("Mn");
        m_symbols.append("Fe");
        m_symbols.append("Co");
        m_symbols.append("Ni");
        m_symbols.append("Cu");
        // 30
        m_symbols.append("Zn");
        m_symbols.append("Ga");
        m_symbols.append("Ge");
        m_symbols.append("As");
        m_symbols.append("Se");
        m_symbols.append("Br");
        m_symbols.append("Kr");
        m_symbols.append("Rb");
        m_symbols.append("Sr");
        m_symbols.append("Y");
        // 40
        m_symbols.append("Zr");
        m_symbols.append("Nb");
        m_symbols.append("Mo");
        m_symbols.append("Tc");
        m_symbols.append("Ru");
        m_symbols.append("Rh");
        m_symbols.append("Pd");
        m_symbols.append("Ag");
        m_symbols.append("Cd");
        m_symbols.append("In");
        // 50
        m_symbols.append("Sn");
        m_symbols.append("Sb");
        m_symbols.append("Te");
        m_symbols.append("I");
        m_symbols.append("Xe");
        m_symbols.append("Cs");
        m_symbols.append("Ba");
        m_symbols.append("La");
        m_symbols.append("Ce");
        m_symbols.append("Pr");
        // 60
        m_symbols.append("Nd");
        m_symbols.append("Pm");
        m_symbols.append("Sm");
        m_symbols.append("Eu");
        m_symbols.append("Gd");
        m_symbols.append("Tb");
        m_symbols.append("Dy");
        m_symbols.append("Ho");
        m_symbols.append("Er");
        m_symbols.append("Tm");
        // 70
        m_symbols.append("Yb");
        m_symbols.append("Lu");
        m_symbols.append("Hf");
        m_symbols.append("Ta");
        m_symbols.append("W");
        m_symbols.append("Re");
        m_symbols.append("Os");
        m_symbols.append("Ir");
        m_symbols.append("Pt");
        m_symbols.append("Au");
        // 80
        m_symbols.append("Hg");
        m_symbols.append("Tl");
        m_symbols.append("Pb");
        m_symbols.append("Bi");
        m_symbols.append("Po");
        m_symbols.append("At");
        m_symbols.append("Rn");
        m_symbols.append("Fr");
        m_symbols.append("Ra");
        m_symbols.append("Ac");
        // 90
        m_symbols.append("Th");
        m_symbols.append("Pa");
        m_symbols.append("U");
        m_symbols.append("Np");
        m_symbols.append("Pu");
        m_symbols.append("Am");
        m_symbols.append("Cm");
        m_symbols.append("Bk");
        m_symbols.append("Cf");
        m_symbols.append("Es");
        //100
        m_symbols.append("Fm");
        m_symbols.append("Md");
        m_symbols.append("No");
        m_symbols.append("Lr");
        m_symbols.append("Rf");
        m_symbols.append("Db");
        m_symbols.append("Sg");
        m_symbols.append("Bh");
        m_symbols.append("Hs");
        m_symbols.append("Mt");
        //110
        m_symbols.append("Ds");
        m_symbols.append("Rg");
        
        return m_symbols;
}

// bool isName(const QString &name)
// {
//   return molsKetch::name2number( name ) != -1;
// }

bool isSymbol(const QString &symbol)
{
  return symbols().contains(symbol);
}

bool isNumber(int number)
{
  return (0 <= number) && ( number <= symbols().count());
}

// QString molsKetch::number2name( int number )
// {
//   Q_ASSERT(isNumber( number ));
//   return symbols()[number-1];
// }

QString molsKetch::number2symbol( int number )
{
  Q_ASSERT(isNumber( number ));
  return eTable.GetSymbol(number);
}

QString molsKetch::position2symbol( int row, int column )
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

// int molsKetch::name2number( const QString &name )
// {
//   for( int i = 0; i < m_size; i++) if (symbols()[i]==name) return i+1;
//   return -1;
// }

// QString molsKetch::name2symbol( const QString &name )
// {
//   for( int i = 0; i < m_size; i++) if (symbols()[i]==name) return symbols()[i];
//   return "";
// }

int molsKetch::symbol2number( const QString &symbol )
{
  return eTable.GetAtomicNum(symbol.toAscii());;
//   for( int i = 0; i < m_size; i++) if (symbols()[i]==symbol) return i+1;
//   return -1;
}

// QString molsKetch::symbol2name( const QString &symbol )
// {
// //   for( int i = 0; i<size; i++) if (m_symbols[i]==symbol) return m_names[i];
//   return "";
// }

double molsKetch::weightOfElement( int number )
{
  Q_ASSERT(isNumber( number ));
  return  eTable.GetMass(number);
}

int molsKetch::valencyOfElement( int number )
{
  Q_ASSERT(isNumber( number ));
  return eTable.GetMaxBonds(number);
}

int molsKetch::oxidationStateOfElement( int number )
{
  Q_ASSERT(isNumber( number ));
  return oxidation()[number];
}

int molsKetch::getSize()
{
  return symbols().count();
}
