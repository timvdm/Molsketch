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

#include <QTransform>
#include <QDebug>

#include "commands.h"

#include "atom.h"
#include "bond.h"
#include "residue.h"

#include "molecule.h"

#include "molscene.h"
#include "graphicsitem.h"

using Molsketch::Atom;
using Molsketch::Bond;
using Molsketch::Molecule;
using namespace Molsketch::Commands;

/////////////////////////////////////////
// AddAtom
/////////////////////////////////////////

AddAtom::AddAtom(Atom * newAtom, Molecule * newMol, const QString & text) : QUndoCommand(text), m_atom(newAtom), m_molecule(newMol)
{}

AddAtom::~AddAtom()
{
  if (m_undone)
    delete m_atom;
}

void AddAtom::undo()
{
  m_molecule->delAtom(m_atom);
  m_undone = true;
}

void AddAtom::redo()
{
  Q_CHECK_PTR(m_atom);
  Q_CHECK_PTR(m_molecule);
  Q_CHECK_PTR(m_molecule->scene());

  m_molecule->addAtom(m_atom);
  m_undone = false;
}

/////////////////////////////////////////
// AddResidue
/////////////////////////////////////////

AddResidue::AddResidue(Residue *newResidue , const QString & text) : QUndoCommand(text), m_residue(newResidue), m_scene (m_residue ->scene ())
{
        qDebug() << "AddResidue::AddResidue";
}

AddResidue::~AddResidue()
{
        if (m_undone)
                delete m_residue;
}

void AddResidue::undo()
{
        m_scene ->removeItem(m_residue);
        m_scene->update();
        m_undone = true;
}

void AddResidue::redo()
{
        Q_CHECK_PTR(m_scene);
        Q_CHECK_PTR(m_residue);
        m_scene ->addItem (m_residue);
        m_undone = false;
}

////////////////////////////////////////
// Change element
////////////////////////////////////////
ChangeElement::ChangeElement(Atom* changeAtom, const QString &newEl, const QString & text) : QUndoCommand(text), m_oldName(changeAtom->element()), m_newName(newEl), m_atom(changeAtom)
{
}

void ChangeElement::undo()
{
  m_atom->setElement(m_oldName);
  m_undone = true;
}

void ChangeElement::redo()
{
  m_atom->setElement(m_newName);
  m_undone = false;
}

////////////////////////////////////////
// IncCharge
////////////////////////////////////////

IncCharge::IncCharge(Atom* atom, const QString & text) : QUndoCommand(text), m_atom(atom)
{
}

void IncCharge::undo()
{
  m_atom->setCharge(m_atom->charge() - 1);
  if (m_atom->scene())
    m_atom->scene()->update();
  m_undone = true;
}

void IncCharge::redo()
{
  m_atom->setCharge(m_atom->charge() + 1);
  if (m_atom->scene())
    m_atom->scene()->update();
  m_undone = false;
}

////////////////////////////////////////
// DecCharge
////////////////////////////////////////

DecCharge::DecCharge(Atom* atom, const QString & text) : QUndoCommand(text), m_atom(atom)
{
}

void DecCharge::undo()
{
  m_atom->setCharge(m_atom->charge() + 1);
  if (m_atom->scene())
    m_atom->scene()->update();
  m_undone = true;
}

void DecCharge::redo()
{
  m_atom->setCharge(m_atom->charge() - 1);
  if (m_atom->scene())
    m_atom->scene()->update();
  m_undone = false;
}

////////////////////////////////////////
// AddImplicitHydrogen
////////////////////////////////////////

AddImplicitHydrogen::AddImplicitHydrogen(Atom* atom, const QString & text) : QUndoCommand(text), m_atom(atom)
{
}

void AddImplicitHydrogen::undo()
{
  m_atom->setNumImplicitHydrogens(m_atom->numImplicitHydrogens() - 1);
  if (m_atom->scene()) m_atom->scene()->update();
  m_undone = true;
}

void AddImplicitHydrogen::redo()
{
  m_atom->setNumImplicitHydrogens(m_atom->numImplicitHydrogens() + 1);
  if (m_atom->scene()) m_atom->scene()->update();
  m_undone = false;
}

// RemoveImplicitHydrogen
RemoveImplicitHydrogen::RemoveImplicitHydrogen(Atom* atom, const QString & text) : QUndoCommand(text), m_atom(atom)
{};
void RemoveImplicitHydrogen::undo()
{
  m_atom->setNumImplicitHydrogens(m_atom->numImplicitHydrogens() + 1);
  if (m_atom->scene()) m_atom->scene()->update();
  m_undone = true;
}
void RemoveImplicitHydrogen::redo()
{
  m_atom->setNumImplicitHydrogens(m_atom->numImplicitHydrogens() - 1);
  if (m_atom->scene()) m_atom->scene()->update();
  m_undone = false;
}

////////////////////////////////////////
// DeleteAtom
////////////////////////////////////////

DelAtom::DelAtom(Atom* delAtom, const QString & text) : QUndoCommand(text), m_atom(delAtom), m_molecule(delAtom->molecule())
{
};

DelAtom::~DelAtom()
{
  if (!m_undone)
  {
    foreach(Bond* bond, m_bondList)
      delete bond;
    delete m_atom;
  }
}

void DelAtom::undo()
{
  m_molecule->addAtom(m_atom);
  for (int i = 0; i < m_bondList.size(); i++) m_molecule->addBond(m_bondList.at(i));
  m_undone = true;
}

void DelAtom::redo()
{
  m_bondList = m_molecule->delAtom(m_atom);
  m_undone = false;
}

// Bond commands

AddBond::AddBond(Bond* newBond, const QString & text) : QUndoCommand(text), m_bond(newBond), m_mol(newBond->beginAtom()->molecule())
{}

AddBond::~AddBond()
{
  if (m_undone)
    delete m_bond;
}

void AddBond::undo()
{
  m_mol->delBond(m_bond);
  Atom *begin = m_bond->beginAtom();
  Atom *end = m_bond->endAtom();
  if (begin)
    begin->removeBond(m_bond);
  if (end)
    end->removeBond(m_bond);
  m_undone = true;
}

void AddBond::redo()
{
  m_mol->addBond(m_bond);
  Atom *begin = m_bond->beginAtom();
  Atom *end = m_bond->endAtom();
  if (begin)
    if (!begin->neighbours().contains(end))
      begin->addBond(m_bond);
  if (end)
    if (!end->neighbours().contains(begin))
      end->addBond(m_bond);
  m_undone = false;
}


DelBond::DelBond(Bond* delBond, const QString & text) : QUndoCommand(text), m_bond(delBond), m_mol(delBond->molecule())
{}
DelBond::~DelBond()
{
  if(!m_undone) delete m_bond;
}
void DelBond::undo()
{
  m_mol->addBond(m_bond);
  m_undone = true;
}
void DelBond::redo()
{
  m_mol->delBond(m_bond);
  m_undone = false;
}

////////////////////////////////////////////////////////////
// Set Bond Type Command
////////////////////////////////////////////////////////////

MergeMol::MergeMol(Molecule* moleculeA, Molecule* moleculeB, Molecule*& mergedMolecule, const QString & text) :  QUndoCommand(text), m_molA(moleculeA), m_molB(moleculeB), m_molC(mergedMolecule), m_scene(moleculeA->scene())
{
  //pre: molA.scene = molB.scene
  Q_ASSERT(moleculeA->scene() == moleculeB->scene());

  mergedMolecule = m_molC = merge(m_molA,m_molB);
}

MergeMol::~MergeMol()
{
  if (!m_undone) delete m_molA;
  if (!m_undone) delete m_molB;
  if (m_undone) delete m_molC;
}

void MergeMol::undo()
{
  m_scene->removeItem(m_molC);
  m_scene->addItem(m_molA);
  m_scene->addItem(m_molB);
  m_undone = true;
}

void MergeMol::redo()
{
  m_scene->removeItem(m_molA);
  m_scene->removeItem(m_molB);
  m_scene->addItem(m_molC);
  m_undone = false;
}

 Molecule * MergeMol::merge( const Molecule * molA, const Molecule* molB )
  {
    // pre: molA and molB are different molecules
    Q_CHECK_PTR(molA);
    Q_CHECK_PTR(molB);
    Q_ASSERT(molA != molB);

    // Creating a new molecule for the merge
    Molecule* molC = new Molecule;

    // Adding the bonds and atoms of the first two molecules
    foreach (Atom* a, molA->atoms())
    {
      Atom* a2 = new Atom(a->scenePos(),a->element(),a->hasImplicitHydrogens());
    a2 ->setColor( a->getColor ());
      molC->addAtom(a2);
    }
    foreach (Bond* b, molA->bonds())
    {
      molC->addBond( molC->atomAt(b->beginAtom()->scenePos()), molC->atomAt(b->endAtom()->scenePos()), b->bondType(), b->getColor ());
    }
    foreach (Atom* a, molB->atoms())
    {
      molC->addAtom( a->element(), a->scenePos(), a->hasImplicitHydrogens(), a->getColor ());
    }
    foreach (Bond* b, molB->bonds())
    {
      molC->addBond( molC->atomAt(b->beginAtom()->scenePos()), molC->atomAt(b->endAtom()->scenePos()), b->bondType(), b->getColor ());
    }

    //         molC->setPos(molA->scenePos());

    return molC;
  }

SplitMol::SplitMol(Molecule* mol, const QString & text) :  QUndoCommand(text), m_oldMol(mol), m_scene(mol->scene())
{
  m_newMolList = m_oldMol->split();
}
SplitMol::~SplitMol()
{
  if (!m_undone) delete m_oldMol;
  if (m_undone) foreach(Molecule* mol,m_newMolList) delete mol;
}
void SplitMol::undo()
{
  foreach(Molecule* mol,m_newMolList) m_scene->removeItem(mol);
  m_scene->addItem(m_oldMol);
  m_undone = true;
}
void SplitMol::redo()
{
  m_scene->removeItem(m_oldMol);
  foreach(Molecule* mol,m_newMolList)
  {
    m_scene->addItem(mol);
  }
  m_undone = false;
}

// Generic item commands

AddItem::AddItem(QGraphicsItem* newItem, MolScene* addScene, const QString & text) : QUndoCommand(text), m_item(newItem), m_scene(addScene)
{}

AddItem::~AddItem()
{
  if (m_undone) delete m_item;
}

void AddItem::undo()
{
  m_scene->removeItem(m_item);
  m_undone = true;
}
void AddItem::redo()
{
  if (m_item->scene() != m_scene) m_scene->addItem(m_item);
  /*
  m_item->setFlag(QGraphicsItem::ItemIsSelectable, m_scene->editMode()==MolScene::MoveMode);
  if (m_item->type() == Molecule::Type) foreach(Atom* atom, dynamic_cast<Molecule*>(m_item)->atoms())
    atom->setFlag(QGraphicsItem::ItemIsSelectable, m_scene->editMode()==MolScene::MoveMode);
    */
  m_scene->update();
  m_undone = false;
}

DelItem::DelItem(QGraphicsItem* delItem, const QString & text) : QUndoCommand(text), m_item(delItem)
{
  m_scene = dynamic_cast<MolScene*>(delItem->scene());
}

DelItem::~DelItem()
{
  if (!m_undone) delete m_item;
}

void DelItem::undo()
{
  m_scene->addItem(m_item);
  m_scene->update();
  m_undone = true;
}
void DelItem::redo()
{
  m_scene->removeItem(m_item);
  m_undone = false;
}

MoveItem::MoveItem(QGraphicsItem* item, const QPointF & pos, const QString & text)
  : Command(text), pos(pos), item(item) {}
void MoveItem::undo()
{
  redo();
}
void MoveItem::redo()
{
  QPointF current = item->pos();
  item->setPos(pos);
  pos = current;
  if (item->type()==Atom::Type && dynamic_cast<Atom*>(item)->molecule())
    dynamic_cast<Atom*>(item)->molecule()->rebuild();
}

bool MoveItem::mergeWith(const QUndoCommand *otherCommand)
{
  const MoveItem* other = dynamic_cast<const MoveItem*>(otherCommand);
  if (!other) return false;
  if (other->item != item) return false;
  return true;
}

MoveItem *MoveItem::relative(QGraphicsItem *item, const QPointF &shift, const QString &text)
{
  return absolute(item, item->pos() + shift, text);
}

MoveItem *MoveItem::absolute(QGraphicsItem *item, const QPointF &newPos, const QString &text)
{
  if (!item) return nullptr;
  return new MoveItem(item, newPos, text);
}

QGraphicsItem *MoveItem::getItem() const
{
  return item;
}


RotateItem::RotateItem(QGraphicsItem* rotateItem, const QTransform & transform, const QString & text) : QUndoCommand(text), m_item(rotateItem), m_transform( transform )
{}
void RotateItem::undo()
{
  m_item -> setTransform( m_transform.inverted(), true );
  m_undone = true;
}
void RotateItem::redo()
{
  m_item -> setTransform( m_transform, true );
  m_undone = false;
}


changeColor::changeColor(QColor color, Molsketch::graphicsItem *item)
  : m_color(color),
    m_item(item)
{
}

void changeColor::undo() // TODO does not seem to refresh properly upon redo
{
  if (!m_item) return ;
  QColor tempColor = m_item->getColor() ;
  m_item->setColor(m_color) ;
  m_color = tempColor ;
}

void changeColor::redo()
{
  undo() ;
}


changeRelativeWidth::changeRelativeWidth(qreal relativeWidth, Molsketch::graphicsItem *item)
  : lw(relativeWidth),
    m_item(item)
{
}

void changeRelativeWidth::undo()
{
  if (!m_item) return ;
  qreal t = m_item->relativeWidth() ;
  m_item->setRelativeWidth(lw);
  lw = t ;
}

void changeRelativeWidth::redo()
{
  undo() ;
}


SwapBondAtoms::SwapBondAtoms(Molsketch::Bond *bond, const QString &text)
  : QUndoCommand(text),
    m_bond(bond)
{
}

void SwapBondAtoms::redo()
{
  m_bond->setAtoms(m_bond->endAtom(), m_bond->beginAtom());
  m_bond->update();
}

void SwapBondAtoms::undo()
{
  redo();
}


SetParentItem::SetParentItem(QGraphicsItem *item, QGraphicsItem *parentItem, const QString &text)
  : QUndoCommand(text),
    item(item),
    parentItem(parentItem) {}

void SetParentItem::redo()
{
  QGraphicsItem* temp = item->parentItem();
  item->setParentItem(parentItem);
  parentItem = temp;
  item->update();
  if (parentItem) parentItem->update();
}

void SetParentItem::undo() { redo(); }


ChangeMoleculeName::ChangeMoleculeName(Molsketch::Molecule *molecule, const QString &newName, const QString& commandName, QUndoCommand* parent)
  : SymmetricCommand(commandName, parent),
    oldName(newName),
    molecule(molecule)
{}

void ChangeMoleculeName::redo()
{
  QString name = molecule->getName();
  molecule->setName(oldName);
  oldName.swap(name);
}


SymmetricCommand::SymmetricCommand(QUndoCommand *parent)
  : QUndoCommand(parent)
{}

SymmetricCommand::SymmetricCommand(const QString &name, QUndoCommand *parent)
  : QUndoCommand(name, parent)
{}

void SymmetricCommand::undo()
{
  redo();
}

Command::Command(const QString &text)
  : QUndoCommand(text){}

void Command::execute()
{
  QGraphicsItem *item = getItem();
  MolScene *scene = nullptr;
  QUndoStack *stack = nullptr;
  if (item) scene = dynamic_cast<MolScene*>(item->scene());
  if (scene) stack = scene->stack();

  if (!stack) {
    redo();
    delete this;
  } else {
    stack->push(this);
  }
}
