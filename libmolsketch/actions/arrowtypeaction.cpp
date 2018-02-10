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
#include "arrowtypeaction.h"
#include "arrowtypewidget.h"
#include "arrow.h"
#include "commands.h"

namespace Molsketch {

  arrowTypeAction::arrowTypeAction(MolScene* scene)
    : ItemTypeAction(scene)
  {
    setItemTypeWidget(new arrowTypeWidget);
    setText(tr("Arrow tip"));
  }

  arrowTypeAction::~arrowTypeAction()
  {
  }

  Arrow *getArrow(graphicsItem* item)
  {
    if (item->type() != Arrow::Type) return 0;
    Arrow *a = dynamic_cast<Arrow*>(item);
    return a;
  }

  void arrowTypeAction::applyTypeToItem(graphicsItem *item, int type) const
  {
    Arrow *arrow = getArrow(item);
    if (!arrow) return;
    attemptUndoPush(new Commands::SetArrowType(arrow, (Arrow::ArrowType) type));
  }

  bool arrowTypeAction::getTypeFromItem(graphicsItem *item, int &type) const
  {
    Arrow *arrow = getArrow(item);
    if (!arrow) return false;
    type = arrow->getArrowType();
    return true;
  }

  int arrowTypeAction::defaultType() const
  {
    return Arrow::UpperForward | Arrow::LowerForward;
  }

} // namespace Molsketch

