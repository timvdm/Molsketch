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
#include "multiaction.h"
#include <QGraphicsSceneMouseEvent>
#include <QMenu>
#include "generics.h"

namespace Molsketch {

  struct multiAction::privateData
  {
    QMenu *menu ;
    QActionGroup *actionGroup ;
  };

  multiAction::multiAction(MolScene *scene)
    : genericAction(scene),
      d(new privateData)
  {
    d->menu = new QMenu() ;
    setMenu(d->menu) ;
    setIconVisibleInMenu(false) ;
    d->actionGroup = new QActionGroup(this);
    d->actionGroup->setExclusive(true);
    connect(d->actionGroup, SIGNAL(triggered(QAction*)),
            this, SLOT(changeIcon()));
    connect(d->actionGroup, SIGNAL(triggered(QAction*)),
            this, SLOT(checkActivation(QAction*)));
  }

  multiAction::~multiAction()
  {
    delete d->menu ;
    delete d ;
  }

  void multiAction::wheelEvent(QGraphicsSceneWheelEvent *event)
  {
    if (!event->delta()) return;
    cycleSubActions(event->delta() > 0); // TODO consider size of wheel movement
    event->accept();
  }

  void multiAction::cycleSubActions(bool inverse)
  {
    cycleActions(d->actionGroup, inverse);
  }

  void multiAction::addSubAction(QAction *a)
  {
    a->setCheckable(true);
    d->menu->addAction(a) ;
    d->actionGroup->addAction(a);
    connect(a, SIGNAL(changed()), this, SLOT(changeIcon()));
    if (!d->actionGroup->checkedAction())
    {
      a->setChecked(true);
      changeIcon();
    }
  }

  void multiAction::addSeparator()
  {
    d->menu->addSeparator() ;
  }

  QAction *multiAction::activeSubAction() const
  {
    return d->actionGroup->checkedAction() ;
  }

  QAction *multiAction::firstSubAction() const
  {
    if (d->actionGroup->actions().isEmpty()) return 0;
    return d->actionGroup->actions().first();
  }

  void multiAction::changeIcon()
  {
    QAction *activeAction = activeSubAction();
    QIcon activeIcon(activeAction ? activeAction->icon() : QIcon());
    setIcon(activeIcon);
  }

  void multiAction::checkActivation(QAction *subaction)
  {
    setChecked(subaction && subaction->isChecked());
  }

}
