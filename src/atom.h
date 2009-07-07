/***************************************************************************
 *   Copyright (C) 2007-2008 by Harm van Eersel                            *
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
 * This file is part of molsKetch and defines the class MsKAtom.
 *
 * @author Harm van Eersel <devsciurus@xs4all.nl>
 * @since Hydrogen
 */


#ifndef ATOM_H
#define ATOM_H

#include <QGraphicsItem>
#include <QList>

class Molecule;

/**
 * Represents an atom
 *
 * @author Harm van Eersel
 */
class MsKAtom : public QGraphicsItem
{
public:
  // Constructor
  /**
   * Creates a new atom.
   *
   * @param position the position of the new atom
   * @param element the element symbol of the new atom
   * @param invisible makes the atom invisible if @c true
   */
  MsKAtom(const QPointF & position, const QString & element, 
      bool implicitHydrogens, QGraphicsItem* parent = 0, QGraphicsScene* scene = 0);

  // Inherited drawing methods
  /** Returns the bounding rectangle of the atom. Needed for Qt painting. */
  virtual QRectF boundingRect() const;
  /** Paint method to draw the atom onto a QPainter. Needed for Qt painting.*/
  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

  // Query methods
  /** Returns whether the atom is drawn. */
  bool isDrawn() const;
  /** Returns whether the atom is hidden. */
  bool isHidden() const;
  /** Returns whether the atom uses implicit hydrogens */
  bool implicitHydrogens() const;


  /** Returns the element symbol of the atom, including the implicit hydrogens. */
  QString element() const;
  /** Returns the weight of the atom. */
  qreal weight() const;
  /** Returns the valency of the atom. */
  int valency() const;
  /**
   * Returns the number of bonds to this atom.
   *
   * Double bonds count twice, triple thrice, etc.
   */
  int numberOfBonds() const;
  /**
   * Returns the charge of the atom.
   *
   * The charge is calculated on basis of the */
  int charge() const;
  /** Returns the oxidation state of the atom. */
  int oxidationState() const;
  /**
   * Returns the number of hydrogen needed to make the atom neutral. 
   *
   * This is calculated on basis of the valency of the atom, the current
   * number of bonds and any additional charge applied to the atom.
   */
//   int hydrogenNeeded() const;
  /** Returns the number of implicit hydrogens currently associated with the atom. */
  int numberOfImplicitHydrogens() const;
  /** Returns the string to identify the atom charge. */
  QString chargeID() const;

  /** Returns the molecule of the atom or NULL if none. */
  virtual Molecule* molecule() const;

  // Manupilation methods
  /** Sets the element symbol of the current atom to @p element. */
  void setElement(const QString & element);
  /** Sets the oxidation state of the current atom to @p state. */
  void setOxidationState(int state);
  /**
   * Sets the valency of the current atom to @p valency. 
   *
   * This function sets the valency of the atom.
   * It is used to calculate the charge of the atom and the number
   * of implicit hydrogens associated with this atom.
   */
  void setValency(int valency);
  /**
   * Sets the number of bonds of the current atom to @p number.
   *
   * This sets the number of bonds to the atom. Changing this will
   * change the number of free valency electrons and consequently
   * the number of implicit hydrogens.
   */
  void setNumberOfBonds(int number);
  /** Adds an external bond to the bondlist of the atom */
  void addConnectedAtom(MsKAtom * atom);
  /** Removes an external bond from the bondlist of the atom */
  void removeConnectedAtom(MsKAtom * atom);
  /** Returns the list of connected atom */
  QList<MsKAtom*> connectedAtoms();
  /**
   * Sets the number of implicit hydrogens of the current atom to @p number.
   *
   * Changing the number of implicit hydrogens will also effect the number
   * of free valency electrons and hence the charge of the atom.
   */
  void setNumberOfImplicitHydrogens(int number);
  /** Sets whether implicit hydrogens should be used */
  void setImplicitHydrogens(bool enabled);

  // Methods needed for qt typecasting
  /** Defines the type of the class. Needed for Qt typecasting.*/
  enum { Type = UserType + 8 };
  /** Returns the type of the class. Needed fro Qt typecasting. */
  virtual int type() const {return MsKAtom::Type;};

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
  // Internal representation
  /** Represents the atom's element symbol. */
  QString m_elementSymbol;
  /** Stores whether the atom is hidden. */
  bool m_hidden;
  /** Stores whether the atom is drawn. */
  bool m_drawn;
  /** Stores the weigth of the atom. */
  qreal m_weight;
  /** Stores the charge of the atom. */
  int m_charge;
  /** Stores the valency of the atom. */
  int m_valency;
  /** Stores the oxidation state of the atom */
  int m_oxidationState;
  /**
   * Stores the number of bonds to this atom
   *
   * This number is used to calculate the charge
   * of the atom. Double bonds count twice, triple thrice, etc.
   */
  int m_numberOfBonds;
  /**
   * Stores the list of atoms connected to this atom by a bond
   *
   * Used to determine the position of the implicit H's. The implicit hydrogens
   * are not included.
   */
  QList<MsKAtom*> * m_connectedAtoms;
  /**
   * Stores the number of implicit hydrogens
   *
   * The number of implicit hydrogens that currently is
   * associated with this atom is used to calculate
   * the charge of the atom and to determine the index
   * of the Hn appended to the atom symbol is this
   * options is enabled.
   */
  int m_numberOfImplicitHydrogens;
  /** Stores whether implicit hydrogens should be used */
  bool m_implicitHydrogens;
  /** Stores the shape of the atom */
  QRectF m_shape;
};

#endif
