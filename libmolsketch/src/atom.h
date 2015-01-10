/***************************************************************************
 *   Copyright (C) 2007-2008 by Harm van Eersel                            *
 *   Copyright (C) 2009 by Tim Vandermeersch                               *
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

#ifndef MSK_ATOM_H
#define MSK_ATOM_H

#include <QList>

#include "graphicsitem.h"

namespace Molsketch {

  class Bond;
  class Ring;
  class Molecule;

  /**
   * Atom class
   */
  class Atom : public graphicsItem
  {
    friend class Molecule;

    public:
      // Methods needed for qt typecasting
      enum { Type = graphicsItem::AtomType };
      /**
       * @return The QGraphicsItem type of the class. Needed fro Qt typecasting. */
      int type() const { return Type; }

      /**
       * Creates a new atom.
       *
       * @param position the position of the new atom
       * @param element the element symbol of the new atom
       * @param invisible makes the atom invisible if @c true
       */
      Atom(const QPointF & position = QPointF(), const QString & element = QString(),
          bool implicitHydrogens = true, QGraphicsItem* parent = 0 GRAPHICSSCENEHEADER ) ;

      ~Atom() ;
      //@name Inherited drawing methods
      //@{
      /** 
       * Returns the bounding rectangle of the atom. Needed for Qt painting. 
       */
      virtual QRectF boundingRect() const;
      /** 
       * Paint method to draw the atom onto a QPainter. Needed for Qt painting.
       */
      void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
      //@}

      // Query methods
      /** Returns whether the atom is drawn. */
      bool isDrawn() const;
      /** Returns whether the atom is hidden. */
      bool isHidden() const;

      //@name Chemistry methods
      //@{
      /**
       * Get the molecule of the atom or NULL if there is none. 
       */
      virtual Molecule* molecule() const;
      /**
       * Set the molecule of the atom. 
       */
      void setMolecule(Molecule *molecule);
      /** 
       * Get the element symbol of the atom, including the implicit hydrogens. 
       */
      QString element() const;
      /** 
       * Set the element symbol of the current atom to @p element (e.g. "C", "N").
       */
      void setElement(const QString & element);
      /** 
       * Get the weight of the atom and it's implicit hydrogens. 
       */
      qreal weight() const;
      /** 
       * Get the valency of the atom (i.e. the number of explicit bonds). 
       */
      int valency() const;
      /**
       * Returns the charge of the atom.
       *
       * FC = # valency electrons - 0.5 * # shared electrons - # unpaired electrons + user specified contribution
       */
      int charge() const;
      /**
       * Set the user specified contribution to the atom charge.
       *
       * FC = # valency electrons - 0.5 * # shared electrons - # unpaired electrons + user specified contribution
       */
      void setCharge(int charge);
      /**
       * @return The number of unpaired electrons (radicals).
       */
      int numUnpairedElectrons() const
      {
        return m_userElectrons;
      }
      void setNumUnpairedElectrons(int n)
      {
        m_userElectrons = n;
      }
      /** 
       * Returns the string for the superscript charge (e.g. "3-", "2-", "-", "", "+", "2+", ...).
       */
      QString chargeString() const;


      bool hasLabel() const;
	  //Returns full atom string . e.g. CH3    Fe2+ etc
	  QString string ()const;


      /**
       * Get the number of bonds to this atom.
       */
      int numBonds() const;
      /**
       * Get the sum of the bond orders of all bonds to this atom.
       *
       * @note Requires molecule() to be set.
       */
      int bondOrderSum() const;
      /**
       * Get the number of non-bonding electrons (e.g. 4 for O=, 2 for NH3, 1 for radicals).
       */
      int numNonBondingElectrons() const;
      /** 
       * Returns whether the atom uses implicit hydrogens 
       */
      bool hasImplicitHydrogens() const;
      /** 
       * Returns the number of implicit hydrogens currently associated with the atom. 
       *
       * @note Requires molecule() to be set.
       */
      int numImplicitHydrogens() const;
      // Manupilation methods


      /** Adds an external bond to the bondlist of the atom */
      void addBond(Bond *bond);
      /** Removes an external bond from the bondlist of the atom */
      void removeBond(Bond *bond);
      QList<Bond*> bonds() const;
      /** Returns the list of connected atom */
      QList<Atom*> neighbours() const;
      /**
       * Sets the number of implicit hydrogens of the current atom to @p number.
       *
       * Changing the number of implicit hydrogens will also effect the number
       * of free valency electrons and hence the charge of the atom.
       */
      void setNumImplicitHydrogens(int number);
      /** Sets whether implicit hydrogens should be used */
      void enableImplicitHydrogens(bool enabled);





      /**
       * Get the number for this atom.
       *
       * @sa Molecule::numberAtoms Atom::setNumber
       */       
      int number() const { return m_number; }

	  void hoverOut () {m_hidden = true;}
          QString xmlName() const { return "atom" ; }
    protected:
      // Event handlers
      /** Event handler to show hidden atoms when the mouse hovers over them. */
      void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
      /** Event handler to hide hidden atoms again after a mouse hovering event. */
      void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);
      /** Event handler to handle atom clicks. */
      void mousePressEvent(QGraphicsSceneMouseEvent* event);
      /** Event handler to handle element changes. */
      QVariant itemChange(GraphicsItemChange change, const QVariant & value);

      /**
       * Set the number for this atom.
       *
       * @sa Molecule::numberAtoms Atom::setNumber
       */
      void setNumber(int number) { m_number = number; }

      void readGraphicAttributes(const QXmlStreamAttributes &attributes) ;
      QXmlStreamAttributes graphicAttributes() const ;

    private:
      void drawAtomLabel(QPainter *painter, const QString &lbl, int alignment);

      /**
       * Compute the boudning rect for this atom. This function needs to be called after changing
       * the elelement symbol, adding bonds, ...
       */
      void computeBoundingRect();

      // Internal representation
      /** Represents the atom's element symbol. */
      QString m_elementSymbol;
      /** Stores whether the atom is hidden. */
      bool m_hidden;
      /** Stores whether the atom is drawn. */
      bool m_drawn;
      /** Stores the charge of the atom. */
      int m_userCharge;
      int m_userElectrons;

      int m_number;

      /**
       * Stores the list of atoms connected to this atom by a bond
       *
       * Used to determine the position of the implicit H's. The implicit hydrogens
       * are not included.
       */
      QList<Bond*> m_bonds;
      /**
       * Stores the number of implicit hydrogens
       *
       * The number of implicit hydrogens that currently is
       * associated with this atom is used to calculate
       * the charge of the atom and to determine the index
       * of the Hn appended to the atom symbol is this
       * options is enabled.
       */
      int m_userImplicitHydrogens;
      /** Stores whether implicit hydrogens should be used */
      bool m_implicitHydrogens;
      /** Stores the shape of the atom */
      QRectF m_shape;
  };

} // namespace

#endif
