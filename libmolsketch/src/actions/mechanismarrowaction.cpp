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
#include "mechanismarrowaction.h"

namespace Molsketch {

  mechanismArrowAction::mechanismArrowAction(MolScene* scene)
    : reactionArrowAction(scene)
  {
    // TODO make arrow action with all arrow tips available
    // TODO arrow action manipulates selected arrow items
    setText(tr("Curved arrow"));
    foreach(QAction* action, findChildren<QAction*>())
    {
      if (action->text() == tr("Single arrow"))
      {
        action->setIcon(QIcon(":images/mechanismarrow.png"));
        action->trigger();
      }
      if (action->text() == tr("Double arrow"))
        action->setIcon(QIcon(":images/curveddoublearrow.png"));
      if (action->text() == tr("Half arrow"))
        action->setIcon(QIcon(":images/curvedhalfarrow.png"));
    }
    setChecked(false);
  }

  mechanismArrowAction::~mechanismArrowAction()
  {
  }

  QPolygonF mechanismArrowAction::makePolygon(const QLineF &line)
  {
    return QPolygonF() << line.p1() << line.normalVector().p2()
                       << line.normalVector().translated(line.p2()-line.p1()).p2() << line.p2();
  }

} // namespace Molsketch

