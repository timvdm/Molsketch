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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/


#include <QGraphicsSceneHoverEvent>
#include <QMenu>
#include <QtGui>

#include <actions/frametypeaction.h>
#include <actions/flipstereobondsaction.h>

#include "molecule.h"

#include "atom.h"
#include "bond.h"
#include "molscene.h"
#include "element.h"
#include "math2d.h"

#include "electronsystem.h"
#include "scenesettings.h"
#include "settingsitem.h"

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

  qreal Molecule::toDegrees(const qreal &angle)
  {
    qreal temp = (angle - (int) angle) + ((int)angle % 360) ;
    return temp < 0 ? 360 + temp : temp ;
  }

#define DEFAULTINITIALIZER m_atomList(this), m_bondList(this), m_electronSystemsUpdate(true)

  Molecule::Molecule(QGraphicsItem* parent GRAPHICSSCENESOURCE )
    : graphicsItem(parent GRAPHICSSCENEINIT ),
      DEFAULTINITIALIZER
  {
    setDefaults();
  }

  Molecule::Molecule(QSet<Atom*> atomSet, QSet<Bond*> bondSet,
                     QGraphicsItem* parent GRAPHICSSCENESOURCE)
    : graphicsItem (parent GRAPHICSSCENEINIT ),
      DEFAULTINITIALIZER
  {
    setDefaults();

    foreach(Atom* atom, atomSet) addAtom(atom);

    // ...and bonds
    foreach(Bond* bond, bondSet)
    {
      addBond(bond);
      Atom *begin = bond->beginAtom(), *end = bond->endAtom();
      if (begin && !atomSet.contains(begin)) addAtom(begin);
      if (end && !atomSet.contains(end)) addAtom(end);
    }
  }

  Molecule::Molecule(const Molecule &mol GRAPHICSSCENESOURCE)
    : graphicsItem (mol GRAPHICSSCENEINIT),
      DEFAULTINITIALIZER
  {
    setDefaults();
    clone(mol.atoms().toSet());
    setPos(mol.pos());
  }

  Molecule::Molecule(const Molecule &mol, const QSet<Atom *> &atoms, QGraphicsItem *parent)
    : graphicsItem (mol),
      DEFAULTINITIALIZER
  {
    setParentItem(parent);
    setDefaults();
    clone(atoms);
    setPos(mol.pos()); // TODO check this
  }

  void Molecule::clone(QSet<Atom *> atoms)
  {
    QMap<Atom*, Atom*> oldToNewAtoms;
    QSet<Bond*> oldBonds;
    foreach(Atom* atom, atoms)
    {
      Atom *newAtom = new Atom(*atom);
      oldToNewAtoms[atom] = newAtom;
      addAtom(newAtom);
      foreach(Bond* bond, atom->bonds())
        if (atoms.contains(bond->beginAtom())
            && atoms.contains(bond->endAtom()))
          oldBonds << bond;

    }

    foreach(Bond* bond, oldBonds)
      addBond(new Bond(*bond, oldToNewAtoms[bond->beginAtom()],
              oldToNewAtoms[bond->endAtom()]));
  }

  Molecule* Molecule::combineMolecules(const QSet<Molecule*>& molecules, QMap<Atom *, Atom *> *givenAtomMap, QMap<Bond*,Bond*> *givenBondMap)
  {
    Molecule* result = new Molecule;
    QMap<Atom*,Atom*> atomMap;
    QMap<Bond*,Bond*> bondMap;
    foreach(Molecule* molecule, molecules)
    {
      if (!molecule) continue;
      foreach(Atom* atom, molecule->atoms())
      {
        Atom* newAtom = new Atom(*atom);
        result->addAtom(newAtom);
        atomMap[atom] = newAtom;
      }
      foreach(Bond* bond, molecule->bonds())
      {
        Bond* newBond = new Bond(*bond, atomMap[bond->beginAtom()], atomMap[bond->endAtom()]);
        result->addBond(newBond);
        bondMap[bond] = newBond;
      }
    }
    if (givenAtomMap) atomMap.swap(*givenAtomMap);
    if (givenBondMap) bondMap.swap(*givenBondMap);
    return result;
  }

  void Molecule::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
  {
    foreach (Atom* atom, atoms())
      atom->setHidden(QLineF(event->scenePos(), atom->scenePos()).length() > 10);
    graphicsItem::hoverMoveEvent(event);
  }

  void Molecule::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
  {
    hideAllAtoms();
    graphicsItem::hoverLeaveEvent(event);
  }

  void Molecule::mousePressEvent(QGraphicsSceneMouseEvent *event)
  {
    hideAllAtoms();
    graphicsItem::mousePressEvent(event);
  }

  void Molecule::hideAllAtoms()
  {
    foreach (Atom* atom, atoms())
      atom->setHidden(true);
  }

  // Manipulation methods

  Atom* Molecule::addAtom(const QString &element, const QPointF &point, bool implicitHydrogen, QColor c)
  {
    //pre: element is a non-empty string and point is a valid position on the canvas in scene coordinates
    Q_ASSERT(!element.isEmpty());
    //post: an atom of element has been added to the molecule
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
    atom->setParentItem(this);
    if (scene ()) {
      atom ->setColor (dynamic_cast<MolScene *> (scene ())->settings() ->defaultColor()->get());
    }

    m_electronSystemsUpdate = true;

    return atom;
  }

  Bond* Molecule::addBond(Atom* atomA, Atom* atomB, Bond::BondType type, QColor c)
  {
    //pre: atomA and atomB are existing different atoms in the molecule
    Q_ASSERT (m_atomList.contains(atomA));
    Q_ASSERT (m_atomList.contains(atomB));
    //Q_ASSERT (atomA != atomB);
    if (atomA == atomB)
      return 0;

    //post: a bond of type has been added between atomA and atomB

    // Creating a new bond
    Bond* bond = new Bond(atomA,atomB,static_cast<Bond::BondType>(type));
    bond ->setColor(c);
    return addBond(bond);
  }

  Bond* Molecule::addBond(Bond* bond)
  {
    //pre(1): bond is a valid pointer to a bond
    Q_CHECK_PTR(bond);
    //pre(2): the bond is between two atoms of this molecule
    // TODO this is broken...
//    Q_ASSERT(m_atomList.contains(bond->beginAtom()));
//    Q_ASSERT(m_atomList.contains(bond->endAtom()));

    if (scene ()) bond ->setColor(scene()->settings()->defaultColor()->get()); // TODO ??
    // Checking if and altering when a bond exists
    Bond* bondX = bondBetween(bond->beginAtom(), bond->endAtom());
    if (bondX) {
      delete bond;
      if (scene ()) bondX ->setColor(scene()->settings()->defaultColor()->get());
      return bondX;
    }

    // Adding the bond the the molecule
    m_bondList.append(bond);
    bond->setParentItem(this);

    bond->setAtoms(bond->beginAtom(), bond->endAtom()); // TODO huh?

    m_electronSystemsUpdate = true;
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
      bond->setParentItem(0);
      Atom *begin = bond->beginAtom();
      Atom *end = bond->endAtom();
      if (begin)
        begin->removeBond(bond);
      if (end)
        end->removeBond(bond);
      if (scene())
        scene()->removeItem(bond);
    }

    // Remove the atom
    m_atomList.removeAll(atom);
    atom->setParentItem(0);
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
    Atom *end = bond->endAtom();
    if (begin)
      begin->removeBond(bond);
    if (end)
      end->removeBond(bond);

    // Removing the bond
    m_bondList.removeAll(bond);
    bond->setParentItem(0);
    if (scene())
      scene()->removeItem(bond);

    m_electronSystemsUpdate = true;
  }

  QSet<Atom*> getConnectedAtoms(Atom* startAtom)
  {
    QSet<Atom*> connectedAtoms = {startAtom};
    int lastSize = 0;
    while (lastSize < connectedAtoms.size())
    {
      lastSize = connectedAtoms.size();
      foreach(Atom* atom, connectedAtoms)
        connectedAtoms += atom->neighbours().toSet();
    }
    return connectedAtoms;
  }

  QList<Molecule*> Molecule::split()
  {
    QList<Molecule*> molList;

    QSet<Atom*> atoms = m_atomList.toSet();
    while (!atoms.empty())
    {
      QSet<Atom*> subgroup = getConnectedAtoms(*(atoms.begin()));
      molList << new Molecule(*this, subgroup);
      atoms -= subgroup;
    }

    return molList;
  }



  // Query methods


  Atom* Molecule::atom (const int n) const
  { // TODO start with 0
    if (n>= 1 && n <= m_atomList.size ()) {
      return m_atomList[n - 1];
    }
    return NULL;
  }

  int Molecule::atomIndex(const Atom *atomPointer) const
  {
    return m_atomList.indexOf(const_cast<Atom*>(atomPointer))+1 ;
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

    return 0;
  }

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

  const QList<Atom*>& Molecule::atoms() const
  {
    return m_atomList;
  }

  const QList<Bond*>& Molecule::bonds() const
  {
    return m_bondList;
  }

  QWidget *Molecule::getPropertiesWidget()
  {
    MoleculePopup* widget = new MoleculePopup;
    widget->connectMolecule(this);
    return widget;
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

  bool Molecule::canSplit() const
  {
    if (m_atomList.isEmpty()) return false;
    return getConnectedAtoms(m_atomList.first()) != m_atomList.toSet();
  }

  QVariant Molecule::itemChange(GraphicsItemChange change, const QVariant &value)
  {
    if (change == ItemTransformHasChanged) rebuild();

    return graphicsItem::itemChange(change, value);
  }

  void Molecule::rebuild()
  {
    //pre: true
    //post: the molecule has been rebuild

    // Remove and then readd all elements
    prepareGeometryChange();

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

  // draw a blue rectangle if this molecule is selected
  if(isSelected()) {
    painter->setPen(Qt::blue);
    painter->drawRect(boundingRect()); // TODO simply draw rectangle around own and children!
  } // TODO move this somewhere else, so that it can have a different Z value

  // draw the electron systems

  if (!scene()) return;
  if (!scene()->settings()->electronSystemsVisible()->get()) return;

  updateElectronSystems();
  paintElectronSystems(painter);
}

void Molecule::paintElectronSystems(QPainter *painter) const {
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

  QPolygonF Molecule::coordinates() const
  {
    QVector<QPointF> result ;
    foreach (Atom* atom, m_atomList)
      result << atom->coordinates() ;
    return result ;
  }

  void Molecule::setCoordinates(const QVector<QPointF> &c)
  {
    if (c.size() != m_atomList.size()) return ;
    for (int i = 0 ; i < c.size() ; ++i)
      m_atomList[i]->setCoordinates(c.mid(i,1)) ;
  }

  QRectF Molecule::boundingRect() const
  {
    return childrenBoundingRect();
  }

  MolScene* Molecule::scene() const
  {
    return static_cast<MolScene*>(QGraphicsItem::scene());
  }

  bool NumAtomsMoreThan(const ElectronSystem *es1, const ElectronSystem *es2)
  {
    return es1->numAtoms() > es2->numAtoms();
  }

bool canMerge(const ElectronSystem *es1, const ElectronSystem *es2)
{
  auto firstSetOfAtoms = es1->atoms().toSet();
  auto secondSetOfAtoms = es2->atoms().toSet();
  // may not share an atom
  if (!(firstSetOfAtoms & secondSetOfAtoms).empty()) return false;

  QSet<Atom*> neighborsOfFirstSet;
  std::for_each(firstSetOfAtoms.begin(), firstSetOfAtoms.end(),
                [&] (Atom* a) { neighborsOfFirstSet += a->neighbours().toSet();});
  return !(neighborsOfFirstSet & secondSetOfAtoms).empty();
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

QString Molecule::xmlName() const { return xmlClassName(); }

QString Molecule::xmlClassName() { return "molecule" ; }

// TODO make default function that calls this one instead
void Molecule::prepareContextMenu(QMenu *contextMenu)
{
  MolScene *sc = qobject_cast<MolScene*>(scene());
  if (sc)
  {
    auto action = sc->findChild<FrameTypeAction*>();
    if (action)
    {
      contextMenu->addAction(action);
      QObject::connect(action, SIGNAL(triggered()), contextMenu, SLOT(close()));
    }
    flipStereoBondsAction* flipStereo = sc->findChild<flipStereoBondsAction*>();
    if (flipStereo)
    {
      contextMenu->addAction(flipStereo); // TODO test with bond AND molecule selected simultaneously
      QObject::connect(flipStereo, SIGNAL(triggered()), contextMenu, SLOT(close()));
    }
  }
  graphicsItem::prepareContextMenu(contextMenu);
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
    int piOrder = bond->bondOrder() - 1;
    while (piOrder--) m_electronSystems << new PiElectrons(bond->atoms(), 2);
  }

  foreach (Atom *atom, m_atomList) {
    int unboundElectronPairs = atom->numNonBondingElectrons() / 2;
    while (unboundElectronPairs--) m_electronSystems << new PiElectrons({atom}, 2);
    if (atom->numNonBondingElectrons() % 2) m_electronSystems << new PiElectrons({atom}, 1);
  }

  qSort(m_electronSystems.begin(), m_electronSystems.end(), NumAtomsMoreThan);

  for (int i = 0; i < 1000; ++i) {
    bool restart = false;
    foreach (ElectronSystem *es1, m_electronSystems) {
      foreach (ElectronSystem *es2, m_electronSystems) {
        if (canMerge(es1, es2)) {
          merge(m_electronSystems, es1, es2);
          restart = true;
          break;
        }
      }

      if (restart)
        break;
    }
  }
}

  QList<const XmlObjectInterface *> Molecule::children() const
  {
    return QList<const XmlObjectInterface*>() << &m_atomList << &m_bondList ;
  }

  XmlObjectInterface *Molecule::produceChild(const QString &name, const QString &type)
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

  void Molecule::afterReadFinalization()
  {
    updateElectronSystems();
  }

  void Molecule::readAttributes(const QXmlStreamAttributes &attributes)
  {
    name = attributes.value("name").toString();
  }

  QXmlStreamAttributes Molecule::xmlAttributes() const
  {
    QXmlStreamAttributes attributes;
    attributes.append("name", name);
    return attributes;
  }

  void Molecule::setDefaults()
  {
    setHandlesChildEvents(false);
#if QT_VERSION < 0x050000
    setAcceptsHoverEvents(true);
#else
    setAcceptHoverEvents(true) ;
#endif
  }

  Molecule &Molecule::operator+=(const Molecule &other)
  {
    if (&other == this) return *this;
    int offset = m_atomList.size();
    foreach(const Atom* atom, other.m_atomList)
      addAtom(new Atom(*atom));
    foreach(const Bond* bond, other.m_bondList)
      addBond(m_atomList[offset+other.m_atomList.indexOf(bond->beginAtom())],
          m_atomList[offset+other.m_atomList.indexOf(bond->endAtom())],
          bond->bondType(), bond->getColor());
    return *this;
  }

  Molecule Molecule::operator+(const Molecule &other) const
  {
    Molecule result ;
    result += *this;
    result += other;
    return result;
  }

  template <class T>
  abstractXmlObject *Molecule::moleculeItemListClass<T>::produceChild(const QString &name, const QString &type)
  {
     Q_UNUSED(type)
    if (name == "bond") return new Bond ; // TODO hum?
    return 0 ;
  }

  template <class T>
  QList<const XmlObjectInterface *> Molecule::moleculeItemListClass<T>::children() const
  {
    QList<const XmlObjectInterface*> childrenList ;
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
      append(item) ;\
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
      QList<Atom*> minRing = smallestRing(QList<Atom*>(atomList) << next) ;
      if (minRing.size() < size && !minRing.empty())
      {
        size = minRing.size() ;
        ring = minRing ;
      }
    }
    return ring ;
  }
  QString Molecule::getName() const
  {
    return name;
  }

  void Molecule::setName(const QString &value)
  {
    name = value;
  }

  QPixmap renderMolecule(const Molecule &input) {
    Molecule *molecule = new Molecule(input);
    MolScene renderScene;
    qDebug() << "rendering molecule" << &input;
    if (molecule->atoms().size() > 20)
      renderScene.setRenderMode(MolScene::RenderColoredCircles);
    renderScene.addItem(molecule);
    renderScene.settings()->chargeVisible()->set(true);
    renderScene.setSceneRect(molecule->boundingRect());
    QPixmap pixmap(qCeil(renderScene.width()), qCeil(renderScene.height()));
    if (pixmap.isNull()) return pixmap;

    pixmap.fill();
    // Creating and setting the painter
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    renderScene.render(&painter);
    qDebug() << "rendered molecule" << &input;
    return pixmap;
  }
} // namespace
