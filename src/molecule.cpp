/***************************************************************************
 *   Copyright (C) 2007-2008 by Harm van Eersel                            *
 *   Copyright (C) 2009 Tim Vandermeersch                                  *
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
#include "ring.h"
#include "molscene.h"
#include "element.h"

//#include "electronsystem.h"

#include <openbabel/mol.h>
#include <openbabel/obconversion.h>
#include <openbabel/obiter.h>

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
	foreach(Atom* atom, atomSet) {
		Atom *a = new Atom(atom->scenePos(), atom->element(), atom->hasImplicitHydrogens(), this);
		a ->setColor (atom ->getColor ());
    addAtom(a);
	}

  // ...and bonds
	foreach(Bond* bond, bondSet) {
	Bond *b = new Bond(atomAt(bond->beginAtom()->scenePos()),atomAt(bond->endAtom()->scenePos()),bond->bondOrder(),bond->bondType()); 
	b ->setColor (bond ->getColor ());
    addBond(b);
	}
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
	  Atom *a = new Atom(atom->pos(),atom->element(), atom->hasImplicitHydrogens(), this);
	  a ->setColor(atom ->getColor ());
    addAtom(a);
  }
  // ...and bonds
  foreach(Bond* bond, mol->bonds())
  {
	  Bond *b = new Bond(atomAt(bond->beginAtom()->pos()),atomAt(bond->endAtom()->pos()),bond->bondOrder(),bond->bondType());
	  b ->setColor (bond ->getColor ());
	  addBond(b);
  }
  
  // Set the position
  setPos(mol->pos());
}

	void Molecule::numberAtoms () {
		QList <Atom *> ats = atoms ();
		for (int i = 0; i < ats.size (); i++) {
			ats[i] ->setNumber(i);
		}
	}

// Manipulation methods

Atom* Molecule::addAtom(const QString &element, const QPointF &point, bool implicitHydrogen, QColor c)
{
  //pre: element is a non-empty string and point is a valid position on the canvas in scene coordinates
  Q_ASSERT(!element.isEmpty());
  //post: an atom of element has been added to the molecule
//   Atom* atom = new Atom(point,element,((element == "C") && !(scene()->getShowCarbon() ) || ((element == "H") && !(scene()->getShowHydrogen()))),this);
  Atom* atom = new Atom(point,element,implicitHydrogen, this);
	atom ->setColor (c);
  return addAtom(atom);
}

Atom* Molecule::addAtom(Atom* atom)
{
  // pre: atom is a valid pointer to a atom
  Q_CHECK_PTR(atom);

  // Add the atom to the molecule
  m_atomList.append(atom);
  addToGroup(atom);
	if (scene ()) {
		atom ->setColor (dynamic_cast<MolScene *> (scene ()) ->color());
	}

//  /// Work-around qt-bug
//   if (scene()) scene()->addItem(atom);

  return atom;
}

Bond* Molecule::addBond(Atom* atomA, Atom* atomB, int order, int type, QColor c)
{
  //pre: atomA and atomB are existing different atoms in the molecule
  Q_ASSERT (m_atomList.contains(atomA));
  Q_ASSERT (m_atomList.contains(atomB));
  //Q_ASSERT (atomA != atomB);
  if (atomA == atomB)
    return 0;

  //post: a bond of type has been added between atomA and atomB

  // Creating a new bond
  Bond* bond = new Bond(atomA,atomB,order,static_cast<Bond::BondType>(type));
	bond ->setColor(c);
  return addBond(bond);
}

Bond* Molecule::addBond(Bond* bond)
{
  //pre(1): bond is a valid pointer to a bond
  Q_CHECK_PTR(bond);
  //pre(2): the bond is between two atoms of this molecule
  Q_ASSERT(m_atomList.contains(bond->beginAtom()));
  Q_ASSERT(m_atomList.contains(bond->endAtom()));

	if (scene ())	bond ->setColor (dynamic_cast<MolScene *> (scene ()) ->color());
  // Checking if and altering when a bond exists
  Bond* bondX = bondBetween(bond->beginAtom(), bond->endAtom());
	if (bondX) {
		delete bond;
		if (scene ())	bondX ->setColor (dynamic_cast<MolScene *> (scene ()) ->color());
		return bondX;
	}
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

  perceiveRings();
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
  foreach(Bond* bond, delList) {
    //delBond(bond);
    Q_ASSERT(m_bondList.contains(bond));
    m_bondList.removeAll(bond);
    removeFromGroup(bond);
    Atom *begin = bond->beginAtom();
    Atom *end = bond->beginAtom();
    if (begin)
      begin->removeBond(bond);
    if (end)
      end->removeBond(bond);
    if (scene()) 
      scene()->removeItem(bond);
  }

  // Remove the atom
  m_atomList.removeAll(atom);
  removeFromGroup(atom);
  if (scene()) 
    scene()->removeItem(atom);
  
  // Return the list of bonds that were connected for undo
  return delList;
}

  void Molecule::delBond(Bond* bond)
  {
    Q_CHECK_PTR(bond);
    //pre: bond is an existing bond in the molecule
    Q_ASSERT(m_bondList.contains(bond));

    //post: bond has been removed from the molecule

    Atom *begin = bond->beginAtom();
    Atom *end = bond->beginAtom();
    if (begin)
      begin->removeBond(bond);
    if (end)
      end->removeBond(bond);
 
    // Removing the bond
    m_bondList.removeAll(bond);
    removeFromGroup(bond);
    if (scene()) 
      scene()->removeItem(bond);

    perceiveRings();
//  bond->undoValency();
//  /// Superseded by undo
     //delete bond;
     //bond = 0;
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

	
	Atom* Molecule::atomN (const int n) const {
		if (n>= 1 && n <= m_atomList.size ()) {
			return m_atomList[n - 1];
		}
		return NULL;
	}	
	
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
      int hydrogens = atom->numImplicitHydrogens();
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
  /*
  foreach(Bond* bond, m_bondList) 
    removeFromGroup(bond);
  foreach(Atom* atom, m_atomList) 
    removeFromGroup(atom);
  resetTransform();
  foreach(Atom* atom, m_atomList) 
    addToGroup(atom);
  foreach(Bond* bond, m_bondList) 
    addToGroup(bond);
    */
  update();
  
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

  // draw a yellow rectangle if this molecule is selected
  if(isSelected()) {
    painter->setPen(Qt::blue);
    //painter->setBrush(Qt::yellow);
    //painter->setOpacity(0.2);
    painter->drawRect(boundingRect());
  }
  /*
  if(hasFocus()) {
    painter->setBrush(Qt::yellow);
    painter->setPen(Qt::NoPen);
    painter->setOpacity(0.3);
    painter->drawRect(boundingRect());
  }
  */


  // draw the electron systems
  /*
  updateElectronSystems();
  QPen pen = painter->pen();
  pen.setWidth(10);
  pen.setCapStyle(Qt::RoundCap);
  pen.setColor(Qt::green);
  painter->setPen(pen);
//  painter->setOpacity(0.3);
  foreach (ElectronSystem *es, m_electronSystems) {
    foreach (Atom *a, es->atoms()) {
      foreach (Atom *b, es->atoms()) {
        if (bondBetween(a, b))
          painter->drawLine(a->scenePos(), b->scenePos());
//          painter->drawEllipse(atom->scenePos(), 5, 5);
      }
    }
  }
  */


}
  
  QRectF Molecule::boundingRect() const
  {
    return childrenBoundingRect();
  }

  MolScene* Molecule::scene() const
  {
    return static_cast<MolScene*>(QGraphicsItemGroup::scene());
  }

  OpenBabel::OBMol* Molecule::OBMol() const
  {
    // Create the output molecule
    OpenBabel::OBMol* obmol = new OpenBabel::OBMol;
    obmol->SetDimension(2);

    QHash<Atom*,OpenBabel::OBAtom*> hash;

    obmol->BeginModify();
    foreach (Atom* atom, m_atomList) {
      OpenBabel::OBAtom* obatom = obmol->NewAtom();
      obatom->SetVector(atom->scenePos().x()/40,atom->scenePos().y()/40,0);
      std::string element = atom->element().toStdString();
      obatom->SetAtomicNum(Molsketch::symbol2number(atom->element()));
      hash.insert(atom,obatom);
    }
    foreach (Bond* bond, m_bondList) {
      Atom* a1 = bond->beginAtom();
      Atom* a2 = bond->endAtom();

      unsigned int beginIdx = hash.value(a1)->GetIdx();
      unsigned int endIdx = hash.value(a2)->GetIdx();
      unsigned int swapIdx = beginIdx;
      int flags = 0;

      // Setting bondtype
      switch (bond->bondType()) {
        case Bond::Wedge:
          flags |= OB_WEDGE_BOND;
          break;
        case Bond::InvertedWedge:
          flags |= OB_WEDGE_BOND;
          beginIdx = endIdx;
          endIdx = swapIdx;
          break;
        case Bond::Hash:
          flags |= OB_HASH_BOND;
          break;
        case Bond::InvertedHash:
          flags |= OB_HASH_BOND;
          beginIdx = endIdx;
          endIdx = swapIdx;
          break;
        default:
          break;
      }
      obmol->AddBond(beginIdx, endIdx, bond->bondOrder(), flags);
    }
    obmol->EndModify();

    return obmol;
  }

  void Molecule::readXML(QXmlStreamReader &xml)
  {
    QHash<QString, Atom*> atomHash;
    while (!xml.atEnd()) {
      xml.readNext();

      if (xml.name() == "atomArray") {
        /*
         * The atom array
         */
        while (!xml.atEnd()) {
          xml.readNext();
          if (xml.isStartElement()) {
            if (xml.name() == "atom") {
              QXmlStreamAttributes attr = xml.attributes();
              QPointF pos;
              QString element;

              if (attr.hasAttribute("elementType"))
                element = attr.value("elementType").toString();
              if (attr.hasAttribute("x2") && attr.hasAttribute("y2"))
                pos = QPointF(attr.value("x2").toString().toFloat(),
                              attr.value("y2").toString().toFloat());

              //Atom *atom = new Atom(pos, element, true);
              Atom *atom = addAtom(element, pos, true);

              if (attr.hasAttribute("colorR") && attr.hasAttribute("colorG") && attr.hasAttribute("colorB"))
                atom->setColor(QColor(attr.value("colorR").toString().toInt(),
                                      attr.value("colorG").toString().toInt(),
                                      attr.value("colorB").toString().toInt()));

              if (attr.hasAttribute("id"))
                atomHash[attr.value("id").toString()] = atom;
            }
          } else if (xml.isEndElement())
            if (xml.name() == "atomArray")
              break;
        }
      } else if (xml.name() == "bondArray") {
        /*
         * The bond array
         */
        Bond *currentBond = 0;
        while (!xml.atEnd()) {
          xml.readNext();
          if (xml.isStartElement()) {
            if (xml.name() == "bond") {
              QXmlStreamAttributes attr = xml.attributes();
              Atom *begin, *end;
              int order;

              if (attr.hasAttribute("atomRefs2")) {
                QString atomRefs2 = attr.value("atomRefs2").toString();
                QStringList arefs = atomRefs2.split(" ");
                if (arefs.size() != 2)
                  continue;
                qDebug() << "ref1 = " << arefs[0];
                qDebug() << "ref2 = " << arefs[1];
                begin = atomHash.value(arefs[0]);
                end = atomHash.value(arefs[1]);

              }
              if (attr.hasAttribute("order"))
                order = attr.value("order").toString().toInt();

              currentBond = addBond(begin, end, order);

              if (attr.hasAttribute("colorR") && attr.hasAttribute("colorG") && attr.hasAttribute("colorB"))
                currentBond->setColor(QColor(attr.value("colorR").toString().toInt(),
                                             attr.value("colorG").toString().toInt(),
                                             attr.value("colorB").toString().toInt()));


            } else if (xml.name() == "bondStereo") {
              QString bondStereo = xml.readElementText();
              if (bondStereo == "W")
                currentBond->setType(Bond::Wedge);
              if (bondStereo == "H")
                currentBond->setType(Bond::Hash);

            }
          } else if (xml.isEndElement())
            if (xml.name() == "bondArray")
              break;
        }

      } else if (xml.name() == "molecule") {
        if (xml.isEndElement())
          break;
      }


      qDebug() << "text = " << xml.name();
    }

  }
    
  void Molecule::writeXML(QXmlStreamWriter &xml)
  {
    xml.writeStartElement("molecule");
    // write the atoms
    xml.writeStartElement("atomArray");
    //xml.writeAttribute("id", "aa" + QString::number(moleculeCount));
    int atomCount = 0;
    QHash<Atom*,QString> atomHash;
    foreach (Atom *atom, atoms()) {
      atomCount++;
      xml.writeStartElement("atom");
      QString id = "a" + QString::number(atomCount);
      atomHash[atom] = id;
      xml.writeAttribute("id", id);
      xml.writeAttribute("elementType", atom->element());
      xml.writeAttribute("x2", QString::number(atom->pos().x()));
      xml.writeAttribute("y2", QString::number(atom->pos().y()));
      xml.writeAttribute("hydrogenCount", QString::number(atom->numImplicitHydrogens()));
      xml.writeAttribute("colorR", QString::number(atom->getColor().red()));
      xml.writeAttribute("colorG", QString::number(atom->getColor().green()));
      xml.writeAttribute("colorB", QString::number(atom->getColor().blue()));
      xml.writeEndElement(); // atom
    }
    xml.writeEndElement(); // atomArray
    // write the bonds
    xml.writeStartElement("bondArray");
    //xml.writeAttribute("id", "ab" + QString::number(moleculeCount));
    int bondCount = 0;
    foreach (Bond *bond, bonds()) {
      bondCount++;
      xml.writeStartElement("bond");
      QString atomRefs2;
      if ((bond->bondType() == Bond::InvertedHash) || (bond->bondType() == Bond::InvertedWedge))
        atomRefs2 = atomHash[bond->endAtom()] + " " + atomHash[bond->beginAtom()];
      else
        atomRefs2 = atomHash[bond->beginAtom()] + " " + atomHash[bond->endAtom()];
      xml.writeAttribute("atomRefs2", atomRefs2);
      xml.writeAttribute("order", QString::number(bond->bondOrder()));
      xml.writeAttribute("colorR", QString::number(bond->getColor().red()));
      xml.writeAttribute("colorG", QString::number(bond->getColor().green()));
      xml.writeAttribute("colorB", QString::number(bond->getColor().blue()));
      switch (bond->bondType()) {
        case Bond::InvertedWedge:
        case Bond::Wedge:
          //xml.writeStartElement("bondStereo");
          xml.writeTextElement("bondStereo", "W");
          break;
        case Bond::InvertedHash:
        case Bond::Hash:
          xml.writeTextElement("bondStereo", "H");
          break;
        default:
          break;
      }
      xml.writeEndElement(); // bond
    }
    xml.writeEndElement(); // bondArray
    xml.writeEndElement(); // molecule  
  }


  /**
   * Helper function to make sure double bonds are drawn in the ring with the 
   * most double bonds. Looks nicer!
   */
  int numDoubleBondsInRing(const Molecule *molecule, Ring *ring)
  {
    int count = 0;
    QList<Atom*> atoms = ring->atoms();
    for (int i = 0; i < atoms.size(); ++i) {
      Atom *ringAtom = atoms.at(i);

      Bond *ringBond; 
      if (i+1 < atoms.size()) {
        ringBond = molecule->bondBetween(ringAtom, atoms.at(i+1));
      } else {
        ringBond = molecule->bondBetween(ringAtom, atoms.at(0));
      }

      if (ringBond->bondOrder() == 2)
        count++;
    }
  
    return count;
  }

  using OpenBabel::OBRing;

  void Molecule::perceiveRings()
  {
    OpenBabel::OBMol *obmol = OBMol();

    // clear ring info
    foreach (Bond *bond, m_bondList)
      bond->setRing(0);
    foreach (Ring *ring, m_rings)
      delete ring;
    m_rings.clear();

    QHash<Bond*, QList<Ring*> > ringBonds;

    std::vector<OBRing*> rings = obmol->GetSSSR();
    for (std::vector<OBRing*>::iterator r = rings.begin(); r != rings.end(); ++r) {
      unsigned int ringSize = (*r)->_path.size();
      Ring *ring = new Ring;
      QList<Atom*> atoms;
      for (unsigned int i = 0; i < ringSize; ++i) {
        Atom *ringAtom = m_atomList.at((*r)->_path[i] - 1);

        Bond *ringBond; 
        if (i+1 < ringSize) {
          ringBond = bondBetween(ringAtom, m_atomList.at((*r)->_path[i+1] - 1));
        } else {
          ringBond = bondBetween(ringAtom, m_atomList.at((*r)->_path[0] - 1));
        }
        
        if (!ringBonds.contains(ringBond))
          ringBonds[ringBond] = QList<Ring*>();
        if (!ringBonds[ringBond].contains(ring))
          ringBonds[ringBond].append(ring);
        
        atoms.append(ringAtom);


      }

      ring->setAtoms(atoms);
      m_rings.append(ring);
    }

    foreach (Bond *bond, ringBonds.keys()) {
      QList<Ring*> rings = ringBonds.value(bond);

      if (rings.size() == 1) {
        bond->setRing(rings.at(0));
      } else {
        int numDoubleBonds = 0;
        foreach (Ring *r, rings) {
          int n = numDoubleBondsInRing(this, r);
          if (n > numDoubleBonds) {
            numDoubleBonds = n;
            bond->setRing(r);
          }
        }
      }
    }

  }
   
  QString Molecule::smiles() const
  {
    OpenBabel::OBConversion conv;
    if (!conv.SetOutFormat("can"))
      return QString();

    OpenBabel::OBMol *mol = OBMol();
    QString smiles = conv.WriteString(mol).c_str();
    return smiles;
  }

/*

  bool NumAtomsLessThan(const ElectronSystem *es1, const ElectronSystem *es2)
  {
    return es1->numAtoms() < es2->numAtoms();
  }

  bool canMerge(const Molecule *mol, const ElectronSystem *es1, const ElectronSystem *es2)
  {
    bool result = false;

    foreach (Atom *a1, es1->atoms()) {
      foreach (Atom *a2, es2->atoms()) {
        if (a1 == a2)
          return false; // may not share an atom
        if (mol->bondBetween(a1, a2)) {
          result = true; // can be merged if two atoms in the electron system are next to each other
        }
      }
    }

    return result;
  }

  bool merge(QList<ElectronSystem*> &electronSystems, ElectronSystem *es1, ElectronSystem *es2)
  {
    es1->setAtoms(es1->atoms() + es2->atoms());
    es1->setNumElectrons(es1->numElectrons() + es2->numElectrons());
    electronSystems.removeAll(es2);
    delete es2; 
  }
  
  void Molecule::updateElectronSystems()
  {
    foreach (ElectronSystem *es, m_electronSystems)
      delete es;
    m_electronSystems.clear();

    foreach (Bond *bond, m_bondList) {
      int order = bond->bondOrder();
      QList<Atom*> atoms;
      atoms.append(bond->beginAtom());
      atoms.append(bond->endAtom());

      if (order >= 2) {
        PiElectrons *piEle = new PiElectrons;
        piEle->setAtoms(atoms);
        piEle->setNumElectrons(2);
        m_electronSystems.append(piEle);
      }

      if (order == 3) {
        PiElectrons *piEle = new PiElectrons;
        piEle->setAtoms(atoms);
        piEle->setNumElectrons(2);
        m_electronSystems.append(piEle);
      }
    }
    
    foreach (Atom *atom, m_atomList) {
      int unboundElectrons = atom->numNonBondingElectrons();
      QList<Atom*> atoms;
      atoms.append(atom);
 
      for (int i = 2; i <= unboundElectrons; i+=2) {
        PiElectrons *piEle = new PiElectrons;
        piEle->setAtoms(atoms);
        piEle->setNumElectrons(2);
        m_electronSystems.append(piEle);
        qDebug() << "adding lone pair";
      }

      if (unboundElectrons % 2 == 1) {
        PiElectrons *piEle = new PiElectrons;
        piEle->setAtoms(atoms);
        piEle->setNumElectrons(1);
        m_electronSystems.append(piEle);
        qDebug() << "adding radical";
      }
    }

    qSort(m_electronSystems.begin(), m_electronSystems.end(), NumAtomsLessThan);

    for (int i = 0; i < 1000; ++i) {
      bool restart = false;
      foreach (ElectronSystem *es1, m_electronSystems) {
        foreach (ElectronSystem *es2, m_electronSystems) {
          if (canMerge(this, es1, es2)) {
            merge(m_electronSystems, es1, es2);
            restart = true;
            break;
          }
        }

        if (restart)
          break;
      }
    }


    qDebug() << "# ElectronSystem =" << m_electronSystems.size();
  
  }
*/

} // namespace
