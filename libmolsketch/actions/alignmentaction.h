/***************************************************************************
 *   Copyright (C) 2017 by Hendrik Vennekate                               *
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
#ifndef ALIGNMENTACTION_H
#define ALIGNMENTACTION_H

#include <functional>
#include <molecule.h>
#include "abstractitemaction.h"

namespace Molsketch {

  class AlignmentAction : public TopLevelItemAction {
	public:
    static AlignmentAction* flushLeft(MolScene *scene = 0);
    static AlignmentAction* flushRight(MolScene *scene = 0);
    static AlignmentAction* atTop(MolScene *scene = 0);
    static AlignmentAction* atBottom(MolScene *scene = 0);
    static AlignmentAction* atVerticalCenter(MolScene *scene = 0);
    static AlignmentAction* atHorizontalCenter(MolScene *scene = 0);
  private:
    explicit AlignmentAction(const QString& description, MolScene *scene = 0);
    void execute() override;
    typedef std::function<qreal (const qreal&, const graphicsItem*)> Accumulator;
    virtual Accumulator getAccumulator(int count) const = 0;
    virtual QPointF getShift(const graphicsItem* item, const qreal& targetValue) const = 0;
    virtual qreal initialValue() const = 0;
  };

} // namespace Molsketch

#endif // ALIGNMENTACTION_H
