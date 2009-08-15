/***************************************************************************
 *   Copyright (C) 2007-2008 by Harm van Eersel <devsciurus@xs4all.nl>     *
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

 #include <QGraphicsItem>
#include <QList>

namespace Molsketch {

  class Ring;
  class Molecule;

  /**
   * Atom class
   */
  class Atom : public QGraphicsItem
  {
    friend class Molecule;

    public:
      /**
       * Creates a new atom.
       *
       * @param position the position of the new atom
       * @param element the element symbol of the new atom
       * @param invisible makes the atom invisible if @c true
       */
      Atom(const QPointF & position, const QString & element, 
          bool implicitHydrogens, QGraphicsItem* parent = 0, QGraphicsScene* scene = 0);

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

	  void setColor (QColor col) {m_color = col;}
	  QColor getColor () {return m_color;}

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
       */
      int bondOrderSum() const;
      /** 
       * Returns whether the atom uses implicit hydrogens 
       */
      bool hasImplicitHydrogens() const;
      /** 
       * Returns the number of implicit hydrogens currently associated with the atom. 
       */
      int numberOfImplicitHydrogens() const;
      // Manupilation methods


      /** Adds an external bond to the bondlist of the atom */
      void addNeighbor(Atom * atom);
      /** Removes an external bond from the bondlist of the atom */
      void removeNeighbor(Atom * atom);
      /** Returns the list of connected atom */
      const QList<Atom*>& neighbors() const;
      /**
       * Sets the number of implicit hydrogens of the current atom to @p number.
       *
       * Changing the number of implicit hydrogens will also effect the number
       * of free valency electrons and hence the charge of the atom.
       */
      void setNumberOfImplicitHydrogens(int number);
      /** Sets whether implicit hydrogens should be used */
      void enableImplicitHydrogens(bool enabled);

      // Methods needed for qt typecasting
      /** Defines the type of the class. Needed for Qt typecasting.*/
      enum { Type = UserType + 8 };
      /** Returns the type of the class. Needed fro Qt typecasting. */
      virtual int type() const {return Atom::Type;};
	  int n;
	  
	  //returns a list of atoms connected to atom
	  QList<Atom*> neighbours () {return m_neighbors;};

	  void hoverOut () {m_hidden = true;}
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

    private:
      void drawAtomLabel(QPainter *painter, const QString &lbl, int alignment);

      // Internal representation
      /** Represents the atom's element symbol. */
      QString m_elementSymbol;
      /** Stores whether the atom is hidden. */
      bool m_hidden;
      /** Stores whether the atom is drawn. */
      bool m_drawn;
      /** Stores the charge of the atom. */
      int m_userCharge;
	  
	  //color of the atom
	  QColor m_color;
	  
	  //identifies the atom in a molecule. set with molecule::numberAtoms ()

      /**
       * Stores the list of atoms connected to this atom by a bond
       *
       * Used to determine the position of the implicit H's. The implicit hydrogens
       * are not included.
       */
      QList<Atom*> m_neighbors;
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
