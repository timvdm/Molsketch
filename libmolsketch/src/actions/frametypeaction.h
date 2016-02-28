/***************************************************************************
 *   Copyright (C) 2016 Hendrik Vennekate                                  *
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

#ifndef MOLSKETCH_FRAMETYPEACTION_H
#define MOLSKETCH_FRAMETYPEACTION_H

#include "itemgrouptypeaction.h"


namespace Molsketch {

  class FrameTypeAction : public ItemGroupTypeAction
  {
    Q_OBJECT
  public:
    explicit FrameTypeAction(MolScene *scene = 0);
    ~FrameTypeAction();
  private:
    void getType(int &type, QVariant &data) const;
    void applyType(int type, const QVariant &data) const;
    class privateData;
    privateData *d;
  };

} // namespace Molsketch

#endif // MOLSKETCH_FRAMETYPEACTION_H
