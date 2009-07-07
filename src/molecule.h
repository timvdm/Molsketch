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
* This file is part of molsKetch and defines the class Molecule.
*
* @author Harm van Eersel <devsciurus@xs4all.nl>
*/


#ifndef MOLECULE_H
#define MOLECULE_H

#include <QList>

#include "atom.h"
#include "bond.h"
#include "molscene.h"

class QString;
class QPoint;
class QPainter;

/**
 * Represents a molecule on the scene. It can be created either as an empty molecule, 
 * with a set of atoms and bonds or as a copy of another molecule.
 *
 * @author Harm van Eersel <devsciurus@xs4all.nl>
 * @since Hydrogen
 */
class Molecule : public QGraphicsItemGroup
  {
  public:
    // Constructors and destructor
    /** Creates a molecule with @p parent on MolScene @p scene. */
    Molecule(QGraphicsItem* parent = 0, MolScene* scene = 0);
    /** Creates a molecule from the atoms and bonds of the sets with @p parent on MolScene @p scene. */
    Molecule(QSet<MsKAtom*>, QSet<MsKBond*>, QGraphicsItem* parent = 0, MolScene* scene = 0);
    /** Creates a copy of molecule @p mol with @p parent on MolScene @p scene. */
    Molecule(Molecule* mol, QGraphicsItem* parent = 0, MolScene* scene = 0);

    // Enabling typecasting
    /** Enum with the type of the class. Needed for Qt typecasting. */
    enum { Type = UserType + 1 };
    /** Returns the type of the class. Needed for Qt typecasting. */
    int type() const
      {
        return Type;
      };
    /** Paint method to draw the atom onto a QPainter. Needed for Qt painting.*/
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);


    // Manipulation methods

    /**
    * This method add an atom to the molecule with @p element as element at position @p position.
    *
    * @param element element of the atom to be added to the molecule
    * @param position the position where the atom should be added to the molecules in scene coordinates
    * 
    * @return return a pointer to the new atom which has been added to the molecule
    */
    MsKAtom* addMsKAtom(const QString &element, const QPointF &position, bool implicitHydrogen);

    /**
    * This method adds an existing atom to the molecule.
    * 
    * @param atom a pointer to the atom that should be added to the molecule.
    *
    * @return return a pointer to the atom just added to the molecule.
    */
    MsKAtom* addMsKAtom(MsKAtom* atom);

    /**
    * This method removes an atom from the molecule. The bonds connected to this atom will be removed as well. 
    * The atom and bonds will be removed from the scene, but the ownership will be passed on to the caller. This 
    * can be neccecary for undo operations. 
    * 
    * @param atom a pointer to the atom that should be removed from the molecule.
    *
    * @return a list with the bonds which were connected to the removed atom..
    */
    QList<MsKBond*> delAtom(MsKAtom* atom);

    /** Adds a bond between @p atomA and @p atomB with @p order and @p type. */
    MsKBond* addBond(MsKAtom* atomA, MsKAtom* atomB, int order = 1, int type = 0);
    /** Adds existing bond @p bond to the molecule. */
    MsKBond* addBond(MsKBond* bond);
    /** Deletes @p bond from the molecule. */
    void delBond(MsKBond* bond);

//    /** Automaticly adds an atom with a bond to @p startMsKAtom at a convenient position. */
//   void addAutoMsKAtom(MsKAtom* startMsKAtom);

    /**
      * Splits the molecule up in different seperate molecules. Used to clean up the molecule after removing the connection 
    * between two or more parts of the molecule. 
    *
    * @return a list of the submolecules of which this molecule exists.
    */
    QList<Molecule*> split();


    /**
    * This method rebuilds the atom by removing all atoms and bonds from the molecule and consequently readding them. 
    * This is a hack to reset the boundingbox of the molecule after moving one of the atoms and is this method is in need 
    * of a proper solution.
    */
    void rebuild();

//   void normalize();
//   void setMsKAtomSize(qreal pt);

    // Query methods
    /** Returns a pointer to the atom at position @p pos, or NULL id none. */
    MsKAtom* atomAt(const QPointF &pos) const;
//    /** Returns a pointer to the atom with @p id. */
//     MsKAtom* atom(int id) const;

    /** Returns a pointer to the bond at position @p pos or NULL id none. */
    MsKBond* bondAt(const QPointF &pos) const;
//    /** Returns a pointer to bond with @p id. */
//     MsKBond* bond(int id) const;
    /** Returns a list of the bonds connected to @p atom. */
    QList<MsKBond*> bondsOfMsKAtom(MsKAtom* atom);
    /** Returns a pointer to the bond between @p atomA and @p atomB, or a NULL if none. */
    MsKBond* bondBetween(MsKAtom* atomA, MsKAtom* atomB) const;

    /** Returns @c true if the molecule exists of two seperate submolecules, and @c false otherwise. */
    bool canSplit() const;

    /** Returns the list of atoms in the molecule. */
    QList<MsKAtom*> atoms() const;
    /** Returns the list of bonds in the molecule */
    QList<MsKBond*> bonds() const;

    /** Returns the MolScene of the molecule. */
    virtual MolScene* scene() const
      {
        return static_cast<MolScene*>(QGraphicsItemGroup::scene());
      };

    /** Returns the molecule formula. */
    QString formula() const;
    /** Returns the weigth of the molecule. */
    double weight() const;
    /** Returns the charge of the molecule. */
    int charge() const;
    /** Returns the charge as a string with an appropiate + or - sign. */
    QString chargeID() const;

  protected:
//    /** Event handler for mouse press events on the molecule. */
//     virtual void mousePressEvent(QGraphicsSceneMouseEvent* event);
//    /** Event handler for mouse release events on the molecule.*/
//    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
   /** Event handler for changes of the molecule. Needed for rotation handling.*/
   QVariant itemChange(GraphicsItemChange change, const QVariant &value);

  private:
    // Internal representation
    /** A list of pointers to the atoms of the molecule. Used as internal reprentation. */
    QList<MsKAtom*> m_atomList;
    /** A list of pointers to the bonds of the molecule. Used as internal representation. */
    QList<MsKBond*> m_bondList;

  };



#endif
