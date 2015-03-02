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
#include "graphicsitem.h"


namespace Molsketch {

/**
 * Represents a bond.
 *
 * @author Harm van Eersel
 */
class Bond : public graphicsItem
{
	friend class Molecule;

  public:
	// Public enums
	enum { Type = graphicsItem::BondType };
	/**
	 * @return the type of the class. Needed for Qt type casting.
	 */
	int type() const {return Type; }

	/**
	 * Enum for the different bond types
	 */
        enum BondType
        {
          Invalid = 0,
          DativeDot = 1,
          DativeDash = 2,
          Single = 10,
          Wedge  = 11,
          Hash   = 12,
          WedgeOrHash = 13,
          SingleBroken = 14,
          Double = 20,
          CisOrTrans = 21,
          DoubleAsymmetric = 22,
          DoubleBroken = 23,
          Triple = 30,
          TripleAsymmetric = 31,
          TripleBroken = 32,
        };

        static int orderFromType(const BondType& type);

	static qreal defaultLength ;
	static qreal defaultAngle ;

	/**
	 * Constructor. Create a new bond between @p atomA and @p atomB.
	 *
	 * @param atomA the origin atom of the bond
	 * @param atomB the target atom of the bond
	 * @param order the bond order (@c Bond::Single for single, @c Bond::Double for double, @c Bond::Triple for tripple)
	 * @param type the bond type (@c Bond::Normal, @c Bond::Up, @c Bond::Down, e.g.)
	 */
        explicit Bond(Atom* atomA = 0, // TODO check usage
                      Atom* atomB = 0,
                      Bond::BondType type = Single,
                      QGraphicsItem* parent = 0 GRAPHICSSCENEHEADER ) ;
	/**
	 * Destructor.
	 */
	virtual ~Bond();

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
	/** Returns the angle of this bond from atom @p origin */
	qreal bondAngle(const Atom* origin) const ;
	/** Returns the main bond axis */
	QLineF bondAxis() const ;

	// Manipulation methods
	/** Sets the bond type to @p type. */
	void setType(Bond::BondType type);

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

	void setAtoms(Atom* A, Atom* B) ;

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

	QString xmlName() const { return "bond" ; }
	void setRing(Ring *ring) { m_ring = ring; }
	/** set the coordinates of the two atoms */
	void setCoordinates(const QVector<QPointF> &c) ;
	/** get the coordinates of the two atoms */
	QPolygonF coordinates() const ;

  protected:
	QXmlStreamAttributes graphicAttributes() const ;
	void readGraphicAttributes(const QXmlStreamAttributes &attributes) ;
	QStringList textItemAttributes() const ;

  private:
	void drawSimpleBond(QPainter *painter);
	void drawRingBond(QPainter *painter);
        void drawHashBond(QPainter *painter);
        void drawWedgeBond(QPainter *painter);

	// Internal representation
	/** Stores the bond type as integer. */
	Bond::BondType m_bondType;
	/** Stores a pointer to the first atom. */
	Atom* m_beginAtom;
	/** Stores a pointer to the second atom. */
	Atom* m_endAtom;

	Ring *m_ring;

	void *pseudoStereoPointer ;

};

} // namespace

#endif
