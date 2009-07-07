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

class QString;

/**
 * Some auxilary functions for elements
 *
 * @author Harm van Eersel
 */
namespace molsKetch
{
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
/** Returns the symbol of the element with atom number @p number. */
QString position2symbol(int row, int column);
/** Returns the atom number of the element with name @p name. */
// int name2number(const QString &name);
/** Returns the element symbol of the element with name @p name. */
// QString name2symbol(const QString &name);
/** Returns the atom number of the element with symbol @p symbol. */
int symbol2number(const QString &symbol);
/** Returns the element name of the element with symbol @p symbol. */
// QString symbol2name(const QString &symbol);

/** Returns the weight of element with atom number @p number. */
qreal weightOfElement(int number);
/** Returns the valency of element with atom number @p number. */
int valencyOfElement(int number);
/** Returns the most common oxidation state of element with atom number @p number. */
int oxidationStateOfElement(int number);

/** Returns the number of known elements. */
int getSize();
}
#endif
