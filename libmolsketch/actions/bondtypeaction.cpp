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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/
#include "bondtypeaction.h"
#include "bondtypewidget.h"
#include "commands.h"

#include <QMenu>
#include <QVBoxLayout>
#include <QDebug>

namespace Molsketch {

  bondTypeAction::bondTypeAction(MolScene *scene)
    : ItemTypeAction(scene)
  {
    setItemTypeWidget(new bondTypeWidget(false));
    setText(tr("Bond type"));
  }

  // TODO templated function (cf. e.g. BondTypeAction
  Bond *getBond(graphicsItem* item)
  {
    if (item->type() != Bond::Type) return 0;
    Bond *bond = dynamic_cast<Bond*>(item);
    return bond;
  }

  void bondTypeAction::applyTypeToItem(graphicsItem *item, int type) const
  {
    Bond *bond = getBond(item);
    if (!bond) return ;
    attemptUndoPush(new Commands::SetBondType(bond, (Bond::BondType) type));
  }

  bool bondTypeAction::getTypeFromItem(graphicsItem *item, int &type) const
  {
    Bond *bond = getBond(item);
    if (!bond) return false;
    type = bond->bondType();
    return true;
  }

  int bondTypeAction::defaultType() const
  {
    return Bond::Single;
  }

} // namespace Molsketch

