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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
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
  protected:
    QVariant defaultData() const override;
    QSet<graphicsItem*> filterItems(const QList<QGraphicsItem *> &inputItems) const override;
  private:
    void getType(int &type, QVariant &data) const override;
    void applyType(int type, const QVariant &data) const override;
    class privateData;
    privateData *d;
  };

} // namespace Molsketch

#endif // MOLSKETCH_FRAMETYPEACTION_H
