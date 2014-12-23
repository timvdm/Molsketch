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

#include "atom.h"
#include "molecule.h"
#include <graphicsitemtypes.h>
#include <QGraphicsItem>


namespace Molsketch {

/**
 * Represents a bond.
 *
 * @author Harm van Eersel
 */
class Bond : public QGraphicsItem
{
    friend class Molecule;

  public:
    // Public enums
    enum { Type = GraphicsItemTypes::BondType };
    /**
     * @return the type of the class. Needed for Qt type casting.
     */
    int type() const {return Type; }

    /** 
     * Enum for the different bond types 
     */
    enum BondType { 
      InPlane = 0,
      Wedge,
      InvertedWedge,
      Hash,
      InvertedHash,
      WedgeOrHash,
      CisOrTrans,
      NoType,
    };
    /** Enum for the different bondorders. */
    enum bondOrders { 
      Single = 1, /**< Single bond */
      Double = 2, /**< Double bond */
      Triple = 3 /**< Triple bond */
    };



    /**
     * Constructor. Create a new bond between @p atomA and @p atomB. 
     *
     * @param atomA the origin atom of the bond
     * @param atomB the target atom of the bond
     * @param order the bond order (@c Bond::Single for single, @c Bond::Double for double, @c Bond::Triple for tripple)
     * @param type the bond type (@c Bond::Normal, @c Bond::Up, @c Bond::Down, e.g.)
     */
    Bond(Atom* atomA, Atom* atomB, int order = 1, Bond::BondType type = InPlane, QGraphicsItem* parent = 0
#if QT_VERSION < 0x050000
				      , QGraphicsScene *scene = 0
#endif
	 );
    /**
     * Destructor. 
     */
    virtual ~Bond();

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


    // Manipulation methods
    /** Sets the bond type to @p type. */
    void setType(Bond::BondType type);
    /** Cycle forward through the bond types. */
    //void incType();
    /** Cycle backward through the bond types. */
    //void decType();

    /** Sets the bond order to @p order. */
    void setOrder(int order);
    /** Cycle forward through the bond orders. */
    void incOrder();
    /** Cycle backward through the bond orders. */
    void decOrder();
	
	void setColor (QColor col) {m_color = col;}
	QColor getColor () {return m_color;}

    // Query methods
    /** Returns the bond order. */
    int bondOrder() const;
    /** Returns the bond type. */
    Bond::BondType bondType() const;

    /** Returns the origin atom of the bond. */
    Atom* beginAtom() const;
    /** Returns the target atom of the bond. */
    Atom* endAtom() const;
    /** Return @c true if @p atom takes part in this bond and @c false otherwise. */
    bool hasAtom(const Atom* atom) const;

    Atom* otherAtom(const Atom *atom) const;

    /** Returns the molecule this bond is part of. */
    Molecule* molecule() const;

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

    /**
     * Get the ring for this bond. If the bond is part of multiple rings,
     * this method returns the ring that contains the most number of double 
     * bonds. This ensures that double bonds are drawn inside the correct 
     * ring.
     */
    Ring* ring() const { return m_ring; }


  protected:
    void setRing(Ring *ring) { m_ring = ring; }

  private:
    void drawSimpleBond(QPainter *painter);
    void drawRingBond(QPainter *painter);
    void drawHashBond(QPainter *painter, bool inverted);
    void drawWedgeBond(QPainter *painter, bool inverted);

    // Internal representation
    /** Stores the bond type as integer. */
    Bond::BondType m_bondType;
    /** Stores the bond order as integer. */
    int m_bondOrder;
	//color of the bond
	QColor m_color;
    /** Stores a pointer to the first atom. */
    Atom* m_beginAtom;
    /** Stores a pointer to the second atom. */
    Atom* m_endAtom;

    Ring *m_ring;      

};

} // namespace

#endif
