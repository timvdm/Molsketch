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

/** @file
 * This file is part of molsKetch and contains some routines to work with
 * elements and translate between the element name, symbol and number.
 *
 * @author Harm van Eersel <devsciurus@xs4all.nl>
 * @since Hydrogen
 */


#ifndef ELEMENT_H
#define ELEMENT_H

#include <QColor>

class QString;

/**
 * Some auxilary functions for elements
 *
 * @author Harm van Eersel
 */
namespace Molsketch {

  namespace Element {
    enum Elements {
      Dummy = 0,
      RGroup = 0,
      H = 1,
      He = 2,
      Li = 3,
      Be = 4,
      B = 5,
      C = 6,
      N = 7,
      O = 8,
      F = 9,
      Ne = 10,
      Na = 11,
      Mg = 12,
      Al = 13,
      Si = 14,
      P = 15,
      S = 16,
      Cl = 17,
      Ar = 18,
      K = 19,
      Ca = 20,
      Sc = 21,
      Ti = 22,
      V = 23,
      Cr = 24,
      Mn = 25,
      Fe = 26,
      Co = 27,
      Ni = 28,
      Cu = 29,
      Zn = 30,
      Ga = 31,
      Ge = 32,
      As = 33,
      Se = 34,
      Br = 35,
      Kr = 36,
      Rb = 37,
      Sr = 38,
      Y = 39,
      Zr = 40,
      Nb = 41,
      Mo = 42,
      Tc = 43,
      Ru = 44,
      Rh = 45,
      Pd = 46,
      Ag = 47,
      Cd = 48,
      In = 49,
      Sn = 50,
      Sb = 51,
      Te = 52,
      I = 53,
      Xe = 54,
      Cs = 55,
      Ba = 56,
      La = 57,
      Ce = 58,
      Pr = 59,
      Nd = 60,
      Pm = 61,
      Sm = 62,
      Eu = 63,
      Gd = 64,
      Tb = 65,
      Dy = 66,
      Ho = 67,
      Er = 68,
      Tm = 69,
      Yb = 70,
      Lu = 71,
      Hf = 72,
      Ta = 73,
      W = 74,
      Re = 75,
      Os = 76,
      Ir = 77,
      Pt = 78,
      Au = 79,
      Hg = 80,
      Tl = 81,
      Pb = 82,
      Bi = 83,
      Po = 84,
      At = 85,
      Rn = 86,
      Fr = 87,
      Ra = 88,
      Ac = 89,
      Th = 90,
      Pa = 91,
      U = 92,
      Np = 93,
      Pu = 94,
      Am = 95,
      Cm = 96,
      Bk = 97,
      Cf = 98,
      Es = 99,
      Fm = 100,
      Md = 101,
      No = 102,
      Lr = 103,
      Rf = 104,
      Db = 105,
      Sg = 106,
      Bh = 107,
      Hs = 108,
      Mt = 109,
      Ds = 110,
      Rg = 111,
      Uub = 112,
      Uut = 113
    };
  };

  /**
   * Color for elements. These colors are used by some of the MolScene's render modes.
   */
  QColor elementColor(int element);
  
/** Returns whether @p symbol is a proper element symbol. */
bool isElementSymbol(const QString &symbol);
/** Returns whether @p name is a proper element name. */
bool isElementName(const QString &name);
/** Returns whether @p number is a proper atom number. */
bool isMsKAtomNumber(int number);

/** Returns the name of the element with atom number @p number. */
// QString number2name(int number);
/** Returns the symbol of the element with atom number @p number. */
QString number2symbol(int number);
/** Returns the atom number of the element with name @p name. */
// int name2number(const QString &name);
/** Returns the element symbol of the element with name @p name. */
// QString name2symbol(const QString &name);
/** 
 * @return The atom number of the element with symbol @p symbol (e.g. 6 for carbon, 
 * 8 for oxygen).
 */
int symbol2number(const QString &symbol);
/** Returns the element name of the element with symbol @p symbol. */
// QString symbol2name(const QString &symbol);

/** Returns the weight of element with atom number @p number. */
qreal weightOfElement(int number);

/**
 * Get the group for the specified element.
 */
int elementGroup(int element);
/** 
 * Get the number of valence electrons of element with atomic number @p element. 
 * 
 * @note: Main groups have 1, 2, 3, 4, 5, 6, 7 & 8 valence electrons respectivly.
 */
int numValenceElectrons(int element);
/**
 * Get the maximum number of expected bonds (or bond order sum) to make the atom 
 * with atomic number @p element charge neutral (i.e. 4 for carbon, 3 for nitrogen, 
 * 2 for oxygen, ...).
 *
 @verbatim
 group 1: 1
 group 2: 2
 group 3-12: 0
 group 13: 3
 group 14: 4
 group 15: 3
 group 16: 2
 group 17: 1
 group 18: 0
 @endverbatim
 *
 */
int expectedValence(int element);




/** Returns the most common oxidation state of element with atom number @p number. */
int oxidationStateOfElement(int number);

}
#endif
