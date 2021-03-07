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
#ifndef MOLSKETCH_BONDTYPEACTION_H
#define MOLSKETCH_BONDTYPEACTION_H

#include "itemtypeaction.h"

namespace Molsketch {

  class bondTypeAction : public ItemTypeAction
  {
    Q_OBJECT
  public:
    explicit bondTypeAction(MolScene* scene = 0);
  private:
    void applyTypeToItem(graphicsItem *item, int type) const override;
    bool getTypeFromItem(graphicsItem *item, int &type) const override;
    int defaultType() const override;
  };

} // namespace Molsketch

#endif // MOLSKETCH_BONDTYPEACTION_H
