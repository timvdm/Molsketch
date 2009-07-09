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


#include <QtGui>

#include "molecule.h"

#include "atom.h"
#include "bond.h"
#include "molscene.h"
#include "element.h"

namespace Molsketch {

// Constructors

Molecule::Molecule(QGraphicsItem* parent, MolScene* scene) : QGraphicsItemGroup(parent,scene)
{
  // Setting properties
  setFlags(QGraphicsItem::ItemIsFocusable);
  setAcceptedMouseButtons(Qt::LeftButton|Qt::MidButton);
  setAcceptsHoverEvents(true);
  setHandlesChildEvents(false);
  if (scene)
    setFlag(QGraphicsItem::ItemIsSelectable, scene->editMode()==MolScene::MoveMode);
}

Molecule::Molecule(QSet<Atom*> atomSet, QSet<Bond*> bondSet, 
  QGraphicsItem* parent, MolScene* scene) : QGraphicsItemGroup(parent,scene)
{
  // Setting properties
  setFlags(QGraphicsItem::ItemIsFocusable);
  setAcceptedMouseButtons(Qt::LeftButton|Qt::MidButton);
  setAcceptsHoverEvents(true);
  setHandlesChildEvents(false);
  if (scene) setFlag(QGraphicsItem::ItemIsSelectable, scene->editMode()==MolScene::MoveMode);

  // Add the new atoms
  foreach(Atom* atom, atomSet)
    addAtom(new Atom(atom->scenePos(), atom->element(), atom->hasImplicitHydrogens()));

  // ...and bonds
  foreach(Bond* bond, bondSet)
    addBond(new Bond(atomAt(bond->beginAtom()->scenePos()),atomAt(bond->endAtom()->scenePos()),bond->bondOrder(),bond->bondType()));
}

Molecule::Molecule(Molecule* mol, QGraphicsItem* parent, MolScene* scene) : QGraphicsItemGroup(parent,scene)
{
  // Setting properties
  setFlags(QGraphicsItem::ItemIsFocusable);
  setAcceptedMouseButtons(Qt::LeftButton|Qt::MidButton);
  setAcceptsHoverEvents(true);
  setHandlesChildEvents(false);
  if (scene)
    setFlag(QGraphicsItem::ItemIsSelectable, scene->editMode()==MolScene::MoveMode);

  // Add the new atoms
  foreach(Atom* atom, mol->atoms())
  {
    addAtom(new Atom(atom->pos(),atom->element(), atom->hasImplicitHydrogens()));
  }
  // ...and bonds
  foreach(Bond* bond, mol->bonds())
  {
    addBond(new Bond(atomAt(bond->beginAtom()->pos()),atomAt(bond->endAtom()->pos()),bond->bondOrder(),bond->bondType()));
  }
  
  // Set the position
  setPos(mol->pos());
}


// Manipulation methods

Atom* Molecule::addAtom(const QString &element, const QPointF &point, bool implicitHydrogen)
{
  //pre: element is a non-empty string and point is a valid position on the canvas in scene coordinates
  Q_ASSERT(!element.isEmpty());
  //post: an atom of element has been added to the molecule
//   Atom* atom = new Atom(point,element,((element == "C") && !(scene()->getShowCarbon() ) || ((element == "H") && !(scene()->getShowHydrogen()))),this);
  Atom* atom = new Atom(point,element,implicitHydrogen);
  return addAtom(atom);
}

Atom* Molecule::addAtom(Atom* atom)
{
  // pre: atom is a valid pointer to a atom
  Q_CHECK_PTR(atom);

  // Add the atom to the molecule
  m_atomList.append(atom);
  addToGroup(atom);

//  /// Work-around qt-bug
//   if (scene()) scene()->addItem(atom);

  return atom;
}

Bond* Molecule::addBond(Atom* atomA, Atom* atomB, int order, int type)
{
  //pre: atomA and atomB are existing different atoms in the molecule
  Q_ASSERT (m_atomList.contains(atomA));
  Q_ASSERT (m_atomList.contains(atomB));
  Q_ASSERT (atomA != atomB);
  //post: a bond of type has been added between atomA and atomB

  // Creating a new bond
  Bond* bond = new Bond(atomA,atomB,order,type);
  return addBond(bond);
}

Bond* Molecule::addBond(Bond* bond)
{
  //pre(1): bond is a valid pointer to a bond
  Q_CHECK_PTR(bond);
  //pre(2): the bond is between two atoms of this molecule
  Q_ASSERT(m_atomList.contains(bond->beginAtom()));
  Q_ASSERT(m_atomList.contains(bond->endAtom()));


  // Checking if and altering when a bond exists
  Bond* bondX = bondBetween(bond->beginAtom(), bond->endAtom());
  if (bondX) return bondX;
//   {
//     bondX->incOrder();
//     bondX->setType(bond->getType());
//     return bondX;
//   }

  // Setting the valency
  //bond->redoValency();

  // Adding the bond the the molecule
  m_bondList.append(bond);
  addToGroup(bond);

//  /// Work-around qt-bug
//  if (scene()) scene()->addItem(bond);

  return bond;
}

QList<Bond*> Molecule::delAtom(Atom* atom)
{
  //pre: atom is an existing atom in the molecule
  Q_ASSERT(m_atomList.contains(atom));
  
  //post: atom has been removed from the molecule and all bonds to this atom have been removed
  //ret: the former bonds of this atom

  // Remove all connected bonds from the molecule
  QList<Bond*> delList = bonds(atom);
  foreach(Bond* bond, delList) delBond(bond);

  // Remove the atom
  m_atomList.removeAll(atom);
  removeFromGroup(atom);
  if (scene()) scene()->removeItem(atom);
  
  // Return the list of bonds that were connected for undo
  return delList;

}

void Molecule::delBond(Bond* bond)
{
  //pre: bond is an existing bond in the molecule
  Q_ASSERT(m_bondList.contains(bond));

  //post: bond has been removed from the molecule

  // Removing the bond
  m_bondList.removeAll(bond);
  removeFromGroup(bond);
  if (scene()) 
    scene()->removeItem(bond);

//  bond->undoValency();
//  /// Superseded by undo
//  //   delete bond;
}

// void Molecule::addAutoAtom(Atom* startAtom)
// {
//   //pre: atom is part of the molecule
//   //post: the molecule has a new atom with a bond to startAtom
//   Q_ASSERT(m_atomList.contains(startAtom));
//
//     // Calculate a conveniant position
//     QPointF pos;
//     pos.setX( startAtom->pos().x() + 5 );
//     pos.setY( startAtom->pos().y()  + 5 );
//
//     // Add a atom at this position
//     Atom* endAtom = addAtom( scene()->curElement, pos);
//     addBond( startAtom, endAtom );
//
// }

QList<Molecule*> Molecule::split()
{
  //pre: CanSplit
  //post:
  //ret: a list with the two parts of the split molecule

  // Create the return list
  QList<Molecule*> molList;

  // Create sets with the bonds and molcules
  QSet<Atom*> atomSet(m_atomList.toSet());
  QSet<Bond*> bondSet(m_bondList.toSet());
  QSet<Atom*> atomSubSet;
  QSet<Bond*> bondSubSet;

  // Declarations
  int lastSize;
  Atom* atom;
  Bond* bond;

  // Main loop
  while (!atomSet.isEmpty())
    {
      lastSize = 0;
      atomSubSet.clear();
      bondSubSet.clear();

      // Load the first atom
      atomSubSet << atomSet.toList().first();

      while (atomSubSet.size() != lastSize)
        {
          lastSize = atomSubSet.size();
          foreach (atom,atomSubSet)
          {
            foreach(bond,bondSet)
              if (bond->hasAtom(atom)) bondSubSet << bond;
            foreach(bond,bondSubSet)
              atomSubSet << bond->beginAtom() << bond->endAtom();
          }
        }

      atomSet -= atomSubSet;
      bondSet -= bondSubSet;

      molList.append(new Molecule(atomSubSet,bondSubSet));

    }

  return molList;

}



// Query methods

Atom* Molecule::atomAt(const QPointF &pos) const
  {
    //pre: pos is a valid position on the canvas in scene coordinates
    //post: return the atom op position pos or nil

    foreach(Atom* atom, m_atomList)
    {
      if (atom->contains(atom->mapFromScene(pos))) return atom;
    }


    // 	for ( int i = 0; i < children().count() ;i++)
    // 	{
    // 		if ((children()[i]->type() ==  Atom::Type) && (children()[i]->contains(pos)))
    // 		{
    // 			return (Atom*)children()[i];
    // 		}
    // 	}

    return 0;
  }

// Atom* Molecule::atom(int id) const
//   {   //pre: id does not exceed the number of atoms
//     Q_ASSERT(id<m_atomList.size());
//     //post: returns the i-th atom
// 
//     return m_atomList.at(id);
//   }

Bond* Molecule::bondAt(const QPointF &pos) const
  {
    //pre: pos is a valid position on the canvas in scene coordinates
    //post: return the first bond on position pos
    foreach(Bond* bond, m_bondList)
    {
      if (bond->contains(bond->mapFromScene(pos))) return bond;
    }
    return 0;
  }


// Bond* Molecule::bond(int id) const
// {
//     //pre: id does not exceed the number of atoms
//     Q_ASSERT(0 <= id && id < m_bondList.size());
//     //post: returns the i-th atom
//
//     return m_bondList.at(id);
// }

  const QList<Atom*>& Molecule::atoms() const
  {
    return m_atomList;
  }

  const QList<Bond*>& Molecule::bonds() const
  {
    return m_bondList;
  }

  int Molecule::charge() const
  {
    //pre: true
    //ret: total charge of the atoms

    Atom* atom;
    int totalCharge = 0;
    foreach(atom,m_atomList)
      totalCharge += atom->charge();

    /* TODO can be improved */
    return totalCharge < 0 && scene()->autoAddHydrogen() ? 0 : totalCharge;
  }

QString Molecule::formula( ) const
  {
    //pre: true
    //ret: formula of the molecule

    // Create the output object
    QString formula;

    // Analyse the molecule and create a hash with the atoms
    QHash<QString,int> hash;

//     for (int i = 0; i < countAtoms(); i++)
    foreach(Atom* atom, m_atomList)
    {
      QString element = atom->element();
      hash.insert(element, hash.value(element,0) + 1 );

      // Add implicit hydrogens?
      int hydrogens = atom->numberOfImplicitHydrogens();
      if ( hydrogens > 0 ) hash.insert("H", hash.value("H",0) + hydrogens );
    }

    // First Carbon
    if (hash.contains("C"))
      formula += "C" + (hash.value("C") > 1?formula.number(hash.value("C")):"");

    // Then Nitrogen
    if (hash.contains("N"))
      formula += "N" + (hash.value("N") > 1?formula.number(hash.value("N")):"");

    // Compose the formula form the hash
    QHashIterator<QString, int> i(hash);

    // Then other elements
    while (i.hasNext())
      {
        i.next();
        if (!(i.key() == "C" || i.key() ==  "N"  || i.key() ==  "H") )
          {
            formula += i.key() + (i.value() > 1?formula.number(i.value()):"" );
          }
      }

    // Finally Hydrogen
    if (hash.contains("H"))
      formula += "H" + (hash.value("H") > 1?formula.number(hash.value("H")):"");

    // Return the formula
    return formula;
  }

double Molecule::weight( ) const
  {
    //pre: true
    //ret: weigth of the molecule

    double weight = 0;

    foreach(Atom* atom, m_atomList)
    {
      weight += atom->weight();
    }

    return weight;
  }

QString Molecule::chargeID( ) const
  {
    //pre: true
    //ret: textual representation of the charge of the molecule

    // Drawing text
    int c = charge();
    QString chargeId;
    chargeId.setNum(c);
    if (c < -1) chargeId =  chargeId.remove(0,1) + "-";
    if (c == -1) chargeId = "-";
    if (c == 0) chargeId = "";
    if (c == 1) chargeId = "+";
    if (c > 1) chargeId = chargeId + "+";

    return chargeId;
  }

bool Molecule::canSplit() const
  {
    /// TODO Needs improvement
    // Create the return list
    QList<Molecule*> molList;

    // Return false if molecule is empty
    if (m_atomList.isEmpty()) return false;
    // Create sets with the bonds and molcules
    QSet<Atom*> atomSet(m_atomList.toSet());
    QSet<Bond*> bondSet(m_bondList.toSet());
    QSet<Atom*> atomSubSet;
    QSet<Bond*> bondSubSet;

    // Declarations
    int lastSize = 0;
    Atom* atom;
    Bond* bond;

    atomSubSet.clear();
    bondSubSet.clear();

    // Load the first atom
    atomSubSet << atomSet.toList().first();

    while (atomSubSet.size() != lastSize)
      {
        foreach (atom,atomSubSet)
        {
          lastSize = atomSubSet.size();
          foreach(bond,bondSet)
            if (bond->hasAtom(atom)) bondSubSet << bond;
          foreach(bond,bondSubSet)
            atomSubSet << bond->beginAtom() << bond->endAtom();
        }
      }

    atomSet -= atomSubSet;
    bondSet -= bondSubSet;

    return !atomSet.isEmpty();
  }

QVariant Molecule::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemTransformHasChanged) rebuild();

    return QGraphicsItem::itemChange(change, value);
}

void Molecule::rebuild()
{
  //pre: true
  //post: the molecule has been rebuild

  // Remove and then readd all elements
  prepareGeometryChange();
  foreach(Bond* bond, m_bondList) removeFromGroup(bond);
  foreach(Atom* atom, m_atomList) removeFromGroup(atom);
  resetTransform();
  foreach(Atom* atom, m_atomList) addToGroup(atom);
  foreach(Bond* bond, m_bondList) addToGroup(bond);
}


Bond* Molecule::bondBetween(Atom* atomA, Atom* atomB) const
{
//     for (int i = 0; i < countBonds(); i++)
  foreach(Bond* bond, m_bondList)
    if (((bond->beginAtom() == atomA) || (bond->beginAtom() == atomB )) && ((bond->endAtom() == atomA) || (bond->endAtom() == atomB )))
      return bond;

  return 0;
}

// Event handlers

// void Molecule::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
// {
// //   if (scene() && scene()->getEditMode()==MolScene::MoveMode)
// //   {
// //   setFlag(QGraphicsItem::ItemIsSelectable,false);
// //    setSelected(true);
// //    }
//   QGraphicsItem::mouseReleaseEvent(event);
// }

// void Molecule::mousePressEvent(QGraphicsSceneMouseEvent * event)
// {
// //     if (scene() && scene()->getEditMode()==MolScene::MoveMode)
// //     {
// //         setFlag(QGraphicsItem::ItemIsSelectable,true);
// //         setSelected(true);
// //     }
// QGraphicsItem::mousePressEvent(event);
// }

// QVariant Molecule::itemChange(GraphicsItemChange change, const QVariant & value)
// {
// //     if (change == ItemSelectedChange && isSelected())
// //     {
// //         setFlag(QGraphicsItem::ItemIsSelectable,false);
// //     }
//
//     return QGraphicsItem::itemChange(change, value);
// }

  QList< Bond * > Molecule::bonds(const Atom * atom)
  {
    QList<Bond*> bondList;

    foreach(Bond* bond, m_bondList)
      if (bond->hasAtom(atom)) 
        bondList << bond;

    return bondList;
  }

void Molecule::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
  Q_UNUSED(option)
  Q_UNUSED(widget)

  if(isSelected()) {
//     painter->setBrush(Qt::blue);
    painter->setPen(QPen(Qt::black, 2));
//     painter->setOpacity(0.5);
    painter->drawRect(boundingRect());
  }
  if(hasFocus()) {
    painter->setBrush(Qt::blue);
    painter->setPen(Qt::NoPen);
    painter->setOpacity(0.04);
    painter->drawRect(boundingRect());
  }
}

  MolScene* Molecule::scene() const
  {
    return static_cast<MolScene*>(QGraphicsItemGroup::scene());
  }


} // namespace
