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
#include "math2d.h"

#include "electronsystem.h"

namespace Molsketch {

  template <>
  QString Molecule::moleculeItemListClass<Atom>::xmlName() const
  {
    return "atomArray" ;
  }

  template <>
  QString Molecule::moleculeItemListClass<Bond>::xmlName() const
  {
    return "bondArray" ;
  }

  // Constructors

  Molecule::Molecule(QGraphicsItem* parent GRAPHICSSCENESOURCE )
    : QGraphicsItemGroup (parent GRAPHICSSCENEINIT ),
    m_atomList(this),
    m_bondList(this)
  {
    m_electronSystemsUpdate = true;
    // Setting properties
    setFlags(QGraphicsItem::ItemIsFocusable);
    setAcceptedMouseButtons(Qt::LeftButton|Qt::MidButton);
    setHandlesChildEvents(false);
#if QT_VERSION < 0x050000
    setAcceptsHoverEvents(true);
    if (scene)
      setFlag(QGraphicsItem::ItemIsSelectable, qobject_cast<MolScene*>(scene)->editMode()==MolScene::MoveMode);
#else
    setAcceptHoverEvents(true) ;
    if (qobject_cast<MolScene*>(scene()))
      setFlag(QGraphicsItem::ItemIsSelectable, qobject_cast<MolScene*>(scene())->editMode()==MolScene::MoveMode);
#endif
  }

  Molecule::Molecule(QSet<Atom*> atomSet, QSet<Bond*> bondSet,
                     QGraphicsItem* parent GRAPHICSSCENESOURCE)
    : QGraphicsItemGroup (parent GRAPHICSSCENEINIT ),
    m_atomList(this),
    m_bondList(this)
  {
    m_electronSystemsUpdate = true;
    // Setting properties
    setFlags(QGraphicsItem::ItemIsFocusable);
    setAcceptedMouseButtons(Qt::LeftButton|Qt::MidButton);

    setHandlesChildEvents(false);
#if QT_VERSION < 0x050000
    setAcceptsHoverEvents(true);
    if (scene) setFlag(QGraphicsItem::ItemIsSelectable, qobject_cast<MolScene*>(scene)->editMode()==MolScene::MoveMode);
#else
    setAcceptHoverEvents(true) ;
    if (qobject_cast<MolScene*>(scene()))
        setFlag(QGraphicsItem::ItemIsSelectable, qobject_cast<MolScene*>(scene())->editMode()==MolScene::MoveMode);
#endif

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

  Molecule::Molecule(Molecule* mol, QGraphicsItem* parent GRAPHICSSCENESOURCE)
    : QGraphicsItemGroup (parent GRAPHICSSCENEINIT),
      m_atomList(this),
      m_bondList(this)
  {
    m_electronSystemsUpdate = true;
    // Setting properties
    setFlags(QGraphicsItem::ItemIsFocusable);
    setAcceptedMouseButtons(Qt::LeftButton|Qt::MidButton);
    setHandlesChildEvents(false);
#if QT_VERSION < 0x050000
    setAcceptsHoverEvents(true);
    if (scene)
      setFlag(QGraphicsItem::ItemIsSelectable, qobject_cast<MolScene*>(scene)->editMode()==MolScene::MoveMode);
#else
    setAcceptHoverEvents(true) ;
    if (qobject_cast<MolScene*>(scene()))
      setFlag(QGraphicsItem::ItemIsSelectable, qobject_cast<MolScene*>(scene())->editMode()==MolScene::MoveMode);
#endif

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
    m_electronSystemsUpdate = true;

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

    m_electronSystemsUpdate = true;
    refreshRings();
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

    m_electronSystemsUpdate = true;
    // Return the list of bonds that were connected for undo
    return delList;
  }

  QPointF Molecule::graphicalCenterOfMass() const
  {
    QPointF com ;
    foreach(Atom* atom, m_atomList)
      com += atom->pos() ;
    return com / m_atomList.size() ;
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

    m_electronSystemsUpdate = true;
    refreshRings();
    //  bond->undoValency();
    //  /// Superseded by undo
    //delete bond;
    //bond = 0;
  }

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


  Atom* Molecule::atom (const int n) const
  {
    if (n>= 1 && n <= m_atomList.size ()) {
      return m_atomList[n - 1];
    }
    return NULL;
  }

  int Molecule::atomIndex(const Atom *atomPointer) const
  {
    return m_atomList.indexOf(const_cast<Atom*>(atomPointer)) ;
  }

  QString Molecule::atomId(const Atom *atomPointer) const
  {
    return "a" + QString::number(atomIndex(atomPointer)) ;
  }

  Atom *Molecule::atom(const QString &atomID) const
  {
    return atom(atomID.mid(1).toInt()) ;
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

    if (scene()) {
      if (!scene()->electronSystemsVisible())
        return;

      updateElectronSystems();
      QPen pen = painter->pen();
      pen.setWidth(10);
      pen.setCapStyle(Qt::RoundCap);
      pen.setColor(QColor(255, 200, 0));
      painter->setPen(pen);
      //  painter->setOpacity(0.3);

      QList<Atom*> doneList;

      foreach (ElectronSystem *es, m_electronSystems) {
        QPointF midPoint(0.0, 0.0);

        if (es->atoms().size() == 1) {
          QPointF dir(0.0, 0.0), orthogonal;
          Atom *atom = es->atoms().at(0);
          doneList.append(atom);
          foreach (Atom *nbr, atom->neighbours())
            dir += normalized(nbr->pos() - atom->pos());
          dir /= atom->numBonds();
          dir = normalized(dir);
          if (!atom->numBonds()) {
            dir = QPointF(0.0, 1.0);
            orthogonal = QPointF(1.0, 0.0);
          } else {
            orthogonal = QPointF(dir.y(), -dir.x());
          }
          switch (doneList.count(atom)) {
            case 1:
              switch (atom->numBonds()) {
                case 1:
                  painter->drawEllipse(atom->scenePos() - 15 * orthogonal, 5, 5);
                  break;
                default:
                  painter->drawEllipse(atom->scenePos() - 15 * dir, 5, 5);
                  break;
              }
              break;
            case 2:
              switch (atom->numBonds()) {
                case 1:
                  painter->drawEllipse(atom->scenePos() + 15 * orthogonal, 5, 5);
                  break;
                default:
                  painter->drawEllipse(atom->scenePos() + 15 * dir, 5, 5);
                  break;
              }
              break;
            case 3:
              switch (atom->numBonds()) {
                case 1:
                  painter->drawEllipse(atom->scenePos() - 15 * dir, 5, 5);
                  break;
                default:
                  painter->drawEllipse(atom->scenePos() - 15 * orthogonal, 5, 5);
                  break;
              }
              break;
            case 4:
              painter->drawEllipse(atom->scenePos() - 15 * orthogonal, 5, 5);
              break;
            default:
              painter->drawEllipse(atom->scenePos(), 5, 5);
              break;

          }

        } else {
          foreach (Atom *a, es->atoms()) {
            foreach (Atom *b, es->atoms()) {
              if (bondBetween(a, b))
                painter->drawLine(a->scenePos(), b->scenePos());
              //          painter->drawEllipse(atom->scenePos(), 5, 5);
            }

            midPoint += mapToParent(a->scenePos());
          }
        }

        if (es->numAtoms() < 2)
          continue;
        midPoint /= es->numAtoms();

        painter->save();
        painter->setPen(Qt::black);
        QPointF offset(20.0, 20.0);
        painter->drawText(QRectF(midPoint - offset, midPoint + offset), Qt::AlignCenter, QString("%1pi").arg(es->numElectrons()));
        painter->restore();


      }
    }





  }
  
  QRectF Molecule::boundingRect() const
  {
    return childrenBoundingRect();
  }

  MolScene* Molecule::scene() const
  {
    return static_cast<MolScene*>(QGraphicsItemGroup::scene());
  }

  void Molecule::refreshRings()
  {
    // clear ring info
    foreach (Bond *bond, m_bondList)
      bond->setRing(0);
    foreach (Ring *ring, m_rings)
      delete ring;
    m_rings.clear();
    
    // create minimum list of smallest rings
    QList<Atom*> atomList = m_atomList ;
    while (!atomList.empty()) {
      QList<Atom*> minRing = smallestRing(QList<Atom*>() << atomList.takeLast()) ;
      if (minRing.empty()) continue ; // atom not part of a ring
      Ring *ring = new Ring;
      ring->setAtoms(minRing) ;
      m_rings << ring ;
    }
  }

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

  void merge(QList<ElectronSystem*> &electronSystems, ElectronSystem *es1, ElectronSystem *es2)
  {
    es1->setAtoms(es1->atoms() + es2->atoms());
    es1->setNumElectrons(es1->numElectrons() + es2->numElectrons());
    electronSystems.removeAll(es2);
    delete es2;
  }
  
  void Molecule::invalidateElectronSystems()
  {
    m_electronSystemsUpdate = true;
  }

  void Molecule::updateElectronSystems()
  {
    if (!m_electronSystemsUpdate)
      return;
    m_electronSystemsUpdate = false;

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

  QList<const abstractXmlObject *> Molecule::children() const
  {
    return QList<const abstractXmlObject*>() << &m_atomList << &m_bondList ;
  }

  abstractXmlObject *Molecule::produceChild(const QString &name, const QString &type)
  {
    Q_UNUSED(type)
    if (m_atomList.xmlName() == name){
      m_atomList.clear();
      return &m_atomList ;
    }

    if (m_bondList.xmlName() == name)
    {
      m_bondList.clear();
      return &m_bondList ;
    }

    return 0 ;
  }

  template <class T>
  abstractXmlObject *Molecule::moleculeItemListClass<T>::produceChild(const QString &name, const QString &type)
  {
     Q_UNUSED(type)
    if (name == "bond") return new Bond ;
    return 0 ;
  }

  template <class T>
  QList<const abstractXmlObject *> Molecule::moleculeItemListClass<T>::children() const
  {
    QList<const abstractXmlObject*> childrenList ;
    foreach (T* item, *this)
      childrenList << item ;
    return childrenList ;
  }

#define PRODUCECHILDTEMPLATEMACRO(ITEMTYPE, ITEMTYPENAME) \
  template<> \
  abstractXmlObject *Molecule::moleculeItemListClass<ITEMTYPE>::produceChild(const QString &name, const QString &type) \
  { \
    Q_UNUSED(type) \
    if (name == ITEMTYPENAME) \
    {\
      ITEMTYPE *item = new ITEMTYPE ;\
      item->setParentItem(p) ;\
      return item ;\
    }\
    return 0 ;\
  }
  PRODUCECHILDTEMPLATEMACRO(Atom, "atom")
  PRODUCECHILDTEMPLATEMACRO(Bond, "bond")
  
  QList<Atom*> Molecule::smallestRing(QList<Atom*> atomList) const // TODO test
  { // TODO JAVA-style iterators for clarity
    if (atomList.empty()) return atomList ;
    int size = INFINITY ;
    QList<Atom*> ring ;
    foreach (Atom* next, atomList.last()->neighbours())
    {
      if (atomList.size() > 2 && atomList.first() == next) return atomList ;
      if (atomList.contains(next)) continue ;
      QList<Atom*> minRing = smallestRing(atomList << next) ;
      if (minRing.size() < size && !minRing.empty())
      {
        size = minRing.size() ;
        ring = minRing ;
      }
    }
    return ring ;
  }


} // namespace
