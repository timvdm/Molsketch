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
#include "molecule.h"
#include "molscene.h"
#include "graphicsitem.h"

using Molsketch::Atom;
using Molsketch::Bond;
using Molsketch::Molecule;
using namespace Molsketch::Commands;

AddAtom::AddAtom(Atom * newAtom, Molecule * newMol, const QString & text)
  : Command(newMol, text), atom(newAtom), molecule(newMol)
{}

AddAtom::~AddAtom()
{
  if (atom && !atom->scene() && !atom->parentItem())
    delete atom;
}

void AddAtom::undo()
{
  if (!atom || !getItem()) return;
  getItem()->delAtom(atom);
}

void AddAtom::redo()
{
  if (!atom || !getItem()) return;
  getItem()->addAtom(atom);
}

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

AddItem::AddItem(QGraphicsItem* newItem, MolScene* addScene, const QString & text)
  : QUndoCommand(text), m_item(newItem), m_scene(addScene)
{}

AddItem::~AddItem()
{
  if (m_item && !m_item->scene())
    delete m_item;
}

void AddItem::undo()
{
  m_scene->removeItem(m_item);
  m_scene->update();
}
void AddItem::redo()
{
  if (m_item->scene() != m_scene) m_scene->addItem(m_item);
  m_scene->update();
}

DelItem::DelItem(QGraphicsItem* delItem, const QString & text)
  : QUndoCommand(text), m_item(delItem)
{
  m_scene = dynamic_cast<MolScene*>(delItem->scene());
}

DelItem::~DelItem()
{
  if (m_item && !m_item->scene()) delete m_item;
}

void DelItem::undo()
{
  m_scene->addItem(m_item);
  m_scene->update();
}
void DelItem::redo()
{
  m_scene->removeItem(m_item);
  m_scene->update();
}

MoveItem::MoveItem(QGraphicsItem* item, const QPointF & pos, const QString & text, QUndoCommand* parent)
  : setItemPropertiesCommand(item, pos, text, parent) {}

void MoveItem::redo() {
  setItemPropertiesCommand::redo();
  if (getItem()->type()==Atom::Type && dynamic_cast<Atom*>(getItem())->molecule())
    dynamic_cast<Atom*>(getItem())->molecule()->rebuild();
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
