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
 * This file is part of molsKetch and contains the bond class.
 *
 * @author Harm van Eersel <devsciurus@xs4all.nl>
 * @since Hydrogen
 */

#ifndef BOND_H
#define BOND_H

#include <QGraphicsItem>

#include "atom.h"
#include "molecule.h"

namespace Molsketch {

/**
 * Represents a bond.
 *
 * @author Harm van Eersel
 */
class MSKBond : public QGraphicsItem
{
  public:
    /**
     * Constructor. Create a new bond between @p atomA and @p atomB. 
     *
     * @param atomA the origin atom of the bond
     * @param atomB the target atom of the bond
     * @param order the bond order (@c MSKBond::Single for single, @c MSKBond::Double for double, @c MSKBond::Triple for tripple)
     * @param type the bond type (@c MSKBond::Normal, @c MSKBond::Up, @c MSKBond::Down, e.g.)
     */
    MSKBond(MSKAtom* atomA, MSKAtom* atomB, int order = 1, int type = 0, QGraphicsItem* parent = 0, QGraphicsScene* scene = 0);
    /**
     * Destructor. 
     */
    virtual ~MSKBond();

    /** Undos the valency change caused by this bond in the two atoms connected to this bond. */
    //  void undoValency();
    /** Redos the valency change caused by this bond in the two atoms connected to this bond. */
    //void redoValency();

    // Inherited methods
    /** Method to paint the bond on a QPainter object. Needed for Qt painting. */
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
    /** Event handler for changes in the bond. Needed for Qt painting. */
    QVariant itemChange(GraphicsItemChange change, const QVariant & value);

    // Inherited query methods
    /** Returns the shape of the bond. Needed for Qt event handling. */
    virtual QPainterPath shape() const;
    /** Returns the bounding rectangle of the bond. Needed for Qt painting. */
    virtual QRectF boundingRect() const;
    /** Returns the type of the class. Needed for Qt type casting. */
    virtual int type() const {return MSKBond::Type;};

    // Manipulation methods
    /** Sets the bond type to @p type. */
    void setType(int type);
    /** Cycle forward through the bond types. */
    void incType();
    /** Cycle backward through the bond types. */
    void decType();

    /** Sets the bond order to @p order. */
    void setOrder(int order);
    /** Cycle forward through the bond orders. */
    void incOrder();
    /** Cycle backward through the bond orders. */
    void decOrder();

    // Query methods
    /** Returns the bond order. */
    int bondOrder() const;
    /** Returns the bond type. */
    int bondType() const;

    /** Returns the origin atom of the bond. */
    MSKAtom* beginAtom() const;
    /** Returns the target atom of the bond. */
    MSKAtom* endAtom() const;
    /** Return @c true if @p atom takes part in this bond and @c false otherwise. */
    bool hasMSKAtom(const MSKAtom* atom) const;

    /** Returns the molecule this bond is part of. */
    Molecule* molecule() const;

    // Public enums
    /** Defines the class type. Needed for Qt typecasting. */
    enum { Type = UserType + 4 };
    /** Enum for the different bond types */
    enum bondTypes { 
      Normal, /**< Normal bond */
      Up, /**< A bond from atomA upto atomB */
      UpR, /**< A bond from atomB upto atomA */
      Down, /**< A bond from atomA downto atomB */
      DownR, /**< A bond from atomB downto atomA */
      Dot /**< A dotted bond */
    };
    /** Enum for the different bondorders. */
    enum bondOrders { 
      Single = 1, /**< Single bond */
      Double = 2, /**< Double bond */
      Triple = 3 /**< Triple bond */
    };

    // Static auxillary methods
    /** 
     * Auxillary method for shifting a bond perpendicular to the original bond.
     * Needed for the drawing of multiple bonds.
     * 
     * @param vector the original vector that is to be shifted
     * @param shift the amount of shifting
     *
     * @return the shifted vector
     */
    static QLineF shiftVector(const QLineF & vector, qreal shift);

  private:

    // Internal representation
    /** Stores the bond type as integer. */
    int m_bondType;
    /** Stores the bond order as integer. */
    int m_bondOrder;
    /** Stores a pointer to the first atom. */
    MSKAtom* m_beginAtom;
    /** Stores a pointer to the second atom. */
    MSKAtom* m_endAtom;

};

} // namespace

#endif
