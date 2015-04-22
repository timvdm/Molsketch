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
#include "flipbondaction.h"

#include "bond.h"
#include "commands.h"

namespace Molsketch {

  flipBondAction::flipBondAction(MolScene *scene)
    : abstractItemAction(scene)
  {
    setMinimumItemCount(1);
    setIcon(QIcon(":images/flipbond.png"));
    setText(tr("Flip bond"));
    setCheckable(false);
  }

  void flipBondAction::execute()
  {
    attemptBeginMacro(tr("flip bond"));
    foreach(graphicsItem* item, items())
    {
      Bond *bond = dynamic_cast<Bond*>(item);
      if (item->type() != Bond::Type || !bond) continue;
      attemptUndoPush(new Commands::SwapBondAtoms(bond));
    }

    attemptEndMacro();
  }

} // namespace Molsketch

