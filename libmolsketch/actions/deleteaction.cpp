/***************************************************************************
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
#include "deleteaction.h"
#include "molscene.h"
#include "commands.h"
#include <QDebug>
#include <molecule.h>

namespace Molsketch {

  deleteAction::deleteAction(MolScene *scene)
    : genericAction(scene)
  {
    setCheckable(false);
    connect(this, SIGNAL(triggered()), this, SLOT(deleteSelection()));
    setText(tr("Delete"));
    setIcon(QIcon(":images/delete.svg"));
    setShortcut(QKeySequence::Delete);
  }

  deleteAction::~deleteAction()
  {
  }

  void deleteAction::deleteSelection()
  {
    if (!scene()) return;
    QList<QGraphicsItem*> selectedItems(scene()->selectedItems());
    if (selectedItems.isEmpty()) return;
    attemptBeginMacro(tr("Delete items"));
    QSet<Molecule*> molecules;
    Atom *atom = 0;
    Bond *bond = 0;
    foreach(QGraphicsItem* item, selectedItems)
    {
      if (!item->scene()) continue;
      switch(item->type())
      {
        case Atom::Type:
          atom = dynamic_cast<Atom*>(item);
          if (atom)
            molecules << atom->molecule();
          attemptUndoPush(new Commands::DelAtom(atom));
          break;
        case Bond::Type:
          bond = dynamic_cast<Bond*>(item);
          if (bond)
            molecules << bond->molecule();
          attemptUndoPush(new Commands::DelBond(bond));
          break;
        default:
          Commands::ItemAction::removeItemFromScene(item);
      }
    }
    molecules.remove(0);

    foreach(Molecule* molecule, molecules)
    {
      if (!molecule->scene()) continue;
      if (!molecule->canSplit()) continue;
      foreach(Molecule* subMolecule, molecule->split())
        Commands::ItemAction::addItemToScene(subMolecule, scene());
      Commands::ItemAction::removeItemFromScene(molecule);
    }
    attemptEndMacro();
  }

} // namespace Molsketch

