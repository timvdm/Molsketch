/***************************************************************************
 *   Copyright (C) 2007 by Harm van Eersel                                 *
 *   devsciurus@xs4all.nl                                                  *
 *   Copyright (C) 2015 Hendrik Vennekate                                  *
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
 * This file is part of molsKetch and contains the command classes used in
 * the Qt Redo/Undo system. Each class represents an action on the scene and
 * contains the code to redo/undo that action.
 *
 * @author Harm van Eersel <devsciurus@xs4all.nl>
 * @since Deuterium
 */


#ifndef commands_H
#define commands_H

#include <QUndoCommand>
#include <QPointF>

class QGraphicsItem;
class QGraphicsScene;
class QTransform;

#include "bond.h"
#include "arrow.h"
#include "frame.h"

namespace Molsketch {

  class Atom;
  class Molecule;
  class MolScene;
  class Residue;

  namespace Commands { // TODO make null safe and rework

  enum CommandId {
    BondTypeId = 1,
    ArrowTypeId,
    ArrowPropertiesId,
    FrameTypeId,
    ArrowSplineId,
    AtomChargeId,
  };


  class SymmetricCommand : public QUndoCommand {
  public:
    SymmetricCommand(QUndoCommand *parent = 0);
    SymmetricCommand(const QString& name, QUndoCommand* parent = 0);
    virtual ~SymmetricCommand(){}
    void undo();
  };

    /**
 * Command to add an atom
 *
 * @author Harm van Eersel
 */
    class AddAtom : public QUndoCommand
    {
    public:
      /**
     * Creates a new AddAtom command.
     *
     * @param newAtom pointer to the atom that should be added
     * @param molecule pointer to the molecule for the new atom
     * @param text a description of the command
     */
      AddAtom(Atom* newAtom, Molecule* molecule, const QString & text = "");
      /**
     * Destructor
     *
     * Deletes m_atom if command is destructed in an undone state.
     */
      ~AddAtom();
      /** Undo this command. */
      virtual void undo();
      /** Redo this command. */
      virtual void redo();
    private:
      /** Undo state of the command. */
      bool m_undone;
      /** Atom of this command. */
      Atom* m_atom;
      /** Molecule of this command. */
      Molecule* m_molecule;
    };


    class AddResidue : public QUndoCommand
    {
    public:
      /**
                         * Creates a new AddResidue command.
                         *

                         */
      AddResidue(Residue* newResidue, const QString & text = "");
      /**
                         * Destructor
                         *

                         */
      ~AddResidue();
      /** Undo this command. */
      virtual void undo();
      /** Redo this command. */
      virtual void redo();
    private:
      /** Undo state of the command. */
      bool m_undone;
      /** Residue of this command. */
      Residue* m_residue;
      /** Molecule of this command. */
      QGraphicsScene* m_scene;
    };


    /**
 * Command to change the element of an atom
 *
 * @author Harm van Eersel
 */
    class ChangeElement : public QUndoCommand
    {
    public:
      /**
     * Creates a new ChangeElement command.
     *
     * @param changeAtom the atom which element symbol should be changed
     * @param newElementSymbol the new element symbol
     * @param text a description of the command
     */
      ChangeElement(Atom* changeAtom, const QString & newElementSymbol, const QString & text = "");
      /** Undo this command. */
      virtual void undo();
      /** Redo this command. */
      virtual void redo();
    private:
      /** Undo state of the command. */
      bool m_undone;
      /** Old element symbol of the atom. */
      QString m_oldName;
      /** New element symbol of the atom. */
      QString m_newName;
      /** Atom of this command. */
      Atom* m_atom;
    };

    /**
 * Command to decrease the charge of an atom
 *
 * @author Harm van Eersel
 */
    class DecCharge : public QUndoCommand
    {
    public:
      /**
     * Constructor
     *
     * @param atom the atom to decrease the charge of
     * @param text a description of the command
     */
      DecCharge(Atom* atom, const QString & text = "");
      /** Undo this command. */
      virtual void undo();
      /** Redo this command. */
      virtual void redo();
    private:
      /** Undo state of the command. */
      bool m_undone;
      /** Atom of this command. */
      Atom* m_atom;
    };

    /**
 * Command to increase the charge of an atom
 *
 * @author Harm van Eersel
 */
    class IncCharge : public QUndoCommand
    {
    public:
      /**
     * Constructor
     *
     * @param atom the atom to increase the charge of
     * @param text a description of the command
     */
      IncCharge(Atom* atom, const QString & text = "");
      /** Undo this command. */
      virtual void undo();
      /** Redo this command. */
      virtual void redo();
    private:
      /** Undo state of the command. */
      bool m_undone;
      /** Atom of this command. */
      Atom* m_atom;
    };

    /**
 * Command to add an implicit hydrogen
 *
 * @author Harm van Eersel
 */
    class AddImplicitHydrogen : public QUndoCommand
    {
    public:
      /**
     * Constructor
     *
     * @param atom the atom to decrease the charge of
     * @param text a description of the command
     */
      AddImplicitHydrogen(Atom* atom, const QString & text = "");
      /** Undo this command. */
      virtual void undo();
      /** Redo this command. */
      virtual void redo();
    private:
      /** Undo state of the command. */
      bool m_undone;
      /** Atom of this command. */
      Atom* m_atom;
    };

    /**
 * Command to remove an implicit hydrogen
 *
 * @author Harm van Eersel
 */
    class RemoveImplicitHydrogen : public QUndoCommand
    {
    public:
      /**
     * Constructor
     *
     * @param atom the atom to increase the charge of
     * @param text a description of the command
     */
      RemoveImplicitHydrogen(Atom* atom, const QString & text = "");
      /** Undo this command. */
      virtual void undo();
      /** Redo this command. */
      virtual void redo();
    private:
      /** Undo state of the command. */
      bool m_undone;
      /** Atom of this command. */
      Atom* m_atom;
    };

    /**
 * Command to delete an atom
 *
 * @author Harm van Eersel
 */
    class DelAtom : public QUndoCommand
    {
    public:
      /**
     * Creates a new DelAtom command.
     *
     * @param delAtom the atom to be removed
     * @param text a description of the command
     */
      DelAtom(Atom* delAtom, const QString & text = "");
      /**
     * Destructor
     *
     * Deletes m_atom if the command is destructed in a done state.
     */
      virtual ~DelAtom();
      /** Undo this command. */
      virtual void undo();
      /** Redo this command. */
      virtual void redo();
    private:
      /** Undo state of the command. */
      bool m_undone;
      /** Atom of this command. */
      Atom* m_atom;
      /** Molecule of this command. */
      Molecule* m_molecule;
      /** The list of bonds that were connected to m_atom. */
      QList<Bond*> m_bondList;
    };

    // Bond commands

    /**
 * Command to add a bond
 *
 * @author Harm van Eersel
 */
    class AddBond : public QUndoCommand
    {
    public:
      /**
     * Constructor
     *
     * @param newBond the new bond to add
     * @param text a description of the command
     */

      AddBond(Bond* newBond, const QString & text = "");
      /**
     * Destructor
     *
     * Deletes m_bond if the command is in an undone state.
     */
      ~AddBond();
      /** Undo this command. */
      virtual void undo();
      /** Redo this command. */
      virtual void redo();
    private:
      /** Undo state of the command. */
      bool m_undone;
      /** The bond of this command. */
      Bond* m_bond;
      /** Molecule of this command. */
      Molecule* m_mol;
    };


    /**
 * Command to remove a bond
 *
 * @author Harm van Eersel
 */
    class DelBond : public  QUndoCommand
    {
    public:
      /**
     * Constructor
     *
     * @param delBond the bond that should be removed
     * @param text a description of the command
     */
      DelBond(Bond* delBond, const QString & text = "");
      /**
     * Destructor
     *
     * Deletes m_bond if the command is destructed in a done state.
     */
      virtual ~DelBond();
      /** Undo this command. */
      virtual void undo();
      /** Redo this command. */
      virtual void redo();
    private:
      /** Undo state of the command. */
      bool m_undone;
      /** The bond of this command. */
      Bond* m_bond;
      /** Molecule of this command. */
      Molecule* m_mol;
    };

template<class ItemClass,
         class ItemPropertyType,
         void (ItemClass::*setFunction)(const ItemPropertyType&),
         ItemPropertyType (ItemClass::*getFunction)()const,
         int CommandId = -1>

class setItemPropertiesCommand : public QUndoCommand
{
private:
  ItemClass *item;
  ItemPropertyType type;
  typedef setItemPropertiesCommand<ItemClass, ItemPropertyType, setFunction, getFunction, CommandId> ownType;
public:
  setItemPropertiesCommand(ItemClass *Item, ItemPropertyType newType, const QString& text = "")
    : QUndoCommand(text),
      item(Item),
      type(newType){}
  void redo()
  {
    ItemPropertyType temp = (item->*getFunction)();
    (item->*setFunction)(type);
    type = temp;
    item->update();
  }
  void undo() { redo(); }
  int id() const { return CommandId; }
  bool mergeWith(const QUndoCommand *other)
  {
    auto otherCommand = dynamic_cast<const ownType*>(other);
    if (!otherCommand) return false;
    if (otherCommand->item!= item) return false;
    return true;
  }

  // TODO make mergeable
};

typedef setItemPropertiesCommand<Bond, Bond::BondType, &Bond::setType, &Bond::bondType, BondTypeId> SetBondType;
typedef setItemPropertiesCommand<Arrow, Arrow::ArrowType, &Arrow::setArrowType, &Arrow::getArrowType, ArrowTypeId> SetArrowType;
typedef setItemPropertiesCommand<Arrow, Arrow::Properties, &Arrow::setProperties, &Arrow::getProperties, ArrowPropertiesId> setArrowProperties;
typedef setItemPropertiesCommand<Frame, QString, &Frame::setFrameString, &Frame::frameString, FrameTypeId> SetFrameTypeString;
typedef setItemPropertiesCommand<Arrow, bool, &Arrow::setSpline, &Arrow::getSpline, ArrowSplineId> setArrowSplineCommand;
typedef setItemPropertiesCommand<Atom, int, &Atom::setCharge, &Atom::charge, AtomChargeId> setAtomChargeCommand;

class SetParentItem : public QUndoCommand
{
private:
  QGraphicsItem* item;
  QGraphicsItem* parentItem;
public:
  SetParentItem(QGraphicsItem *item, QGraphicsItem* parentItem, const QString& text = "");
  void redo();
  void undo();
};

class SwapBondAtoms : public QUndoCommand
{
public:
  SwapBondAtoms(Bond* bond, const QString& text = "");
  void redo();
  void undo();
private:
  Bond* m_bond;
};

// Molecule commands


class ChangeMoleculeName : public SymmetricCommand
{
  QString oldName;
  Molecule *molecule;
public:
  ChangeMoleculeName(Molecule* molecule, const QString& newName ="", const QString &commandName = "", QUndoCommand *parent = 0);
  void redo();
};

/**
 * Command to merge two molecules
 *
 * @author Harm van Eersel
 */
    class MergeMol : public QUndoCommand
    {
    public:
      /**
     * Constructor
     *
     * @param oldMolA pointer to the first of the two molecules that should be merged
     * @param oldMolB pointer to the second of the two molecules that should be merged
     * @param newMol pointer to the new merged molecule, passed as reference
     * @param text a description of the command
     */
      MergeMol(Molecule* oldMolA, Molecule* oldMolB, Molecule*& newMol, const QString & text = "");
      /**
     * Destructor
     *
     * Deletes the two unmerged molecules if the command is destucted in a done state
     * and the merged molecule if destructed in an undone state.
     */
      virtual ~MergeMol();
      /** Undo this command. */
      virtual void undo();
      /** Redo this command. */
      virtual void redo();
    private:
      /** Merges @p molA and @p molB. Used two merge two molecules when connected with a bond. */
      Molecule* merge(const Molecule* molA, const Molecule* molB);

      /** Undo state of the command. */
      bool m_undone;
      /** The first of the two molecules that should be merged. */
      Molecule* m_molA;
      /** The second of the two molecules that should be merged. */
      Molecule* m_molB;
      /** The merged molecule. */
      Molecule* m_molC;
      /** The scene of this command. */
      MolScene* m_scene;
    };


    /**
 * Command to split a molecule
 *
 * @author Harm van Eersel
 */
    class SplitMol : public QUndoCommand
    {
    public:
      /**
     * Constructor
     *
     * @param molecule the molecule to be split
     * @param text a description of the command
     */
      SplitMol(Molecule* molecule, const QString & text = "");
      /**
     * Destructor
     *
     * Deletes the original molecule if destructed in a done state
     * and the submolecules if destructed in an undone state.
     */
      ~SplitMol();
      /** Undo this command. */
      virtual void undo();
      /** Redo this command. */
      virtual void redo();
    private:
      /** Undo state of the command. */
      bool m_undone;
      /** The molecule before the split. */
      Molecule* m_oldMol;
      /** The list of molecules after the split. */
      QList<Molecule*> m_newMolList;
      /** The scene of this command. */
      MolScene* m_scene;
    };


    // Generic item commands

    /**
 * Command to add an item to the scene
 *
 * @author Harm van Eersel
 */
    class AddItem : public QUndoCommand
    {
    public:
      /**
     * Constructor
     *
     * @param newItem the item that should be added to the scene
     * @param addScene the scene for the new item
     * @param text a description of the command
     */
      AddItem(QGraphicsItem* newItem, MolScene* addScene, const QString & text = "");
      /**
     * Destructor
     *
     * Deletes m_item if the command is destructed in an undone state.
     */
      ~AddItem();
      /** Undo this command. */
      virtual void undo();
      /** Redo this command. */
      virtual void redo();
    private:
      /** Undo state of the command. */
      bool m_undone;
      /** The item of this command. */
      QGraphicsItem* m_item;
      /** The scene of this command. */
      MolScene* m_scene;
    };


    /**
 * Command to remove an item from the scene
 *
 * @author Harm van Eersel
 */
    class DelItem : public QUndoCommand
    {
    public:
      /**
     * Constructor
     *
     * @param delItem item to be deleted
     * @param text a description of the command
     */
      DelItem(QGraphicsItem* delItem, const QString & text = "");
      /**
     * Destructor
     *
     * Deletes m_item if the command is destructed in a done state.
     */
      ~DelItem();
      /** Undo this command. */
      virtual void undo();
      /** Redo this command. */
      virtual void redo();
    private:
      /** Undo state of the command. */
      bool m_undone;
      /** The item of this command. */
      QGraphicsItem* m_item;
      /** The scene of this command. */
      MolScene* m_scene;
    };


    /**
 * Command to move an item on the scene
 *
 * @author Harm van Eersel
 */
    class MoveItem : public QUndoCommand
    {
    public:
      /**
     * Constructor
     *
     * @param moveItem the item to be moved
     * @param moveVector the vector representation of the move
     * @param text a description of the command
     */
      MoveItem(QGraphicsItem* moveItem, const QPointF & moveVector, const QString & text = "");
      /** Undo this command. */
      virtual void undo();
      /** Redo this command. */
      virtual void redo();
    private:
      /** Undo state of the command. */
      bool m_undone;
      /** The position of the item before the move. */
      QPointF m_oldPos;
      /** The position of the item after the move. */
      QPointF m_newPos;
      /** The item of this command. */
      QGraphicsItem* m_item;
    };

    /**
 * Command to rotate an item on the scene
 *
 * @author Harm van Eersel
 */
    class RotateItem : public QUndoCommand
    {
    public:
      /**
     * Constructor
     *
     * @param rotateItem the item to be rotated
     * @param transform the matrix representation of the rotation
     * @param text a description of the command
     */
      RotateItem(QGraphicsItem* rotateItem, const QTransform & transform, const QString & text = "");
      /** Undo this command. */
      virtual void undo();
      /** Redo this command. */
      virtual void redo();
    private:
      /** Undo state of the command. */
      bool m_undone;
      /** The item of this command. */
      QGraphicsItem* m_item;
      /** The position of the item before the move. */
      QTransform m_transform;

    };

    /**
 * Command to change color of an item
 *
 * @author Hendrik Vennekate
 */
    class changeColor : public QUndoCommand
    {
    public:
      /**
   * Constructor
   *
   * @param color the new color
   * @param item the item whose color will be changed
   */
      changeColor(QColor color, graphicsItem* item) ;
      void undo() ;
      void redo() ;
    private:
      QColor m_color ;
      graphicsItem* m_item ;
    };

    /**
 * Command to change the relative line width
 *
 * @author Hendrik Vennekate
 */
    class changeRelativeWidth : public QUndoCommand // TODO macro for property changes (linewidth, color, bond order...)
    {
    public:
      /**
   * Constructor
   *
   * @param relativeWidth the factor by which the global width (defined for the scene)
   * will be changed during drawing
   * @param item the affected item
   */
      changeRelativeWidth(qreal relativeWidth, graphicsItem* item) ;
      void undo() ;
      void redo() ;
    private:
      qreal lw ;
      graphicsItem* m_item ;
    };

  } // namespace Commands

} // namespace Molsketch

#endif
