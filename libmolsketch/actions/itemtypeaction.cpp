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
#include "itemtypeaction.h"

namespace Molsketch {

  ItemTypeAction::ItemTypeAction(MolScene *scene)
    :ItemGroupTypeAction(scene)
  {}

  void ItemTypeAction::applyType(int type, const QVariant &data) const
  {
    Q_UNUSED(data);
    attemptBeginMacro(undoName());
    foreach(graphicsItem *item, items())
      applyTypeToItem(item,type);
    attemptEndMacro();
  }

  void ItemTypeAction::getType(int &type, QVariant &data) const
  {
    Q_UNUSED(data)
    foreach(graphicsItem* item, items())
      if (getTypeFromItem(item, type)) break;
  }




} // namespace Molsketch

