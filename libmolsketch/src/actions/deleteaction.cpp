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
    foreach(QGraphicsItem* item, selectedItems)
    {
      switch(item->type())
      {
        case Atom::Type:
          attemptUndoPush(new Commands::DelAtom(dynamic_cast<Atom*>(item)));
          break;
        case Bond::Type:
          attemptUndoPush(new Commands::DelBond(dynamic_cast<Bond*>(item)));
          break;
        default:
          attemptUndoPush(new Commands::DelItem(item));
      }
      // TODO check if molecule is split
    }
    attemptEndEndMacro();
  }

} // namespace Molsketch

