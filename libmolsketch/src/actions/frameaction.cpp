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

#include <QGraphicsSceneMouseEvent>
#include "frameaction.h"

#include "molscene.h"
#include "commands.h"
#include "frame.h"

namespace Molsketch {

#define FRAMEMACRO(ARROWSTRING, ICON, DESCRIPTION) a = new QAction(QIcon(":images/" ICON ".svg"), tr(DESCRIPTION), this); a->setData(ARROWSTRING); addSubAction(a);

  // TODO this leads to ugly doubling with FrameTypeWidget
  struct FrameAction::privateData
  {
    Frame *currentFrame;
    QPointF mousePressPosition;
    //    QAction *lastCheckedAction;
    privateData()
      : currentFrame(0)
      //      , lastCheckedAction(0)
    {}
  };

  FrameAction::FrameAction(MolScene *scene)
    : multiAction(scene),
      d(new privateData)
  {
    connect(scene, SIGNAL(selectionChanged()), this, SLOT(changeCheckable()));
    connect(this, SIGNAL(triggered()), this, SLOT(gotTriggerd()));
    setText(tr("Decoration"));
    QAction *a = 0;
    FRAMEMACRO("(r .5,r.5)+(-10,0)-+( 10,0)-(r .5,r-.5)-+(-10,0)"
               "(r-.5,r.5)+( 10,0)-+(-10,0)-(r-.5,r-.5)-+( 10,0)",
               "bracket", "brackets")
        FRAMEMACRO("(r.5,r-.5)+(-20,0)-+(20,0)-+(0,20)",
                   "angle", "corner")
        FRAMEMACRO("(r.5,r-.5)+(-10,0).+(10,0).+(0,10)"
                   "$(r.5,r0)-+(0,-10).+(0,10).+(5,0)"
                   ".+(-5,0).+(0,10)$(r.5,r.5)-+(0,-10)"
                   ".+(0,10).+(-10,0)",
                   "curlybracket", "curly brackets")
        FRAMEMACRO("(r.5,r.5)-(r.5,r-.5)-(r-.5,r-.5)-(r-.5,r.5)-(r.5,r.5)",
                   "fullframe", "frame")
  }

  FrameAction::~FrameAction()
  {
    delete d;
  }

  void FrameAction::mousePressEvent(QGraphicsSceneMouseEvent *event)
  {// TODO refactor with other versions (reactionarrowaction etc)
    if (event->button() != Qt::LeftButton) return ;
    if (event->modifiers() != Qt::NoModifier) return ;
    if (!activeSubAction()) return;
    event->accept();
    if (d->currentFrame) delete d->currentFrame;
    d->currentFrame= new Frame() ;
    d->currentFrame->setFrameString(activeSubAction()->data().toString());
    d->mousePressPosition = event->scenePos();
    d->currentFrame->setCoordinates(QVector<QPointF>(2, d->mousePressPosition)) ;
    scene()->addItem(d->currentFrame) ;
    scene()->update(d->currentFrame->boundingRect());
  }

  void FrameAction::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
  {
    if (!d->currentFrame) return ;
    event->accept();
    d->currentFrame->setCoordinates( QPolygonF() << d->mousePressPosition
                                     << event->scenePos());
    scene()->update(d->currentFrame->boundingRect());
  }

  void FrameAction::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
  {
    if (!d->currentFrame) return ;
    if (!scene() || !scene()->stack()) return ; // TODO
    scene()->stack()->push(new Commands::AddItem(d->currentFrame, scene(), tr("Decoration arrow"))) ;
    d->currentFrame = 0 ;
    event->accept();
  }

  void FrameAction::changeCheckable()
  {// TODO move this to multiaction
    if (!scene()) return;
    setCheckable(scene()->selectedItems().isEmpty());
    //    if (!isCheckable()) return;
    //    QAction *actionToCheck = d->lastCheckedAction;
    //    if (!actionToCheck) actionToCheck = firstSubAction();
    //    if (!actionToCheck) return;
    //    actionToCheck->setChecked(true);
  }

  void FrameAction::gotTriggerd()
  {
    if (!scene()) return;
    if (scene()->selectedItems().isEmpty()) return;
    if (!activeSubAction()) return;
    Frame *frame = new Frame();
    frame->setFrameString(activeSubAction()->data().toString());

    auto undoStack = scene()->stack();
    undoStack->beginMacro(tr("Add decoration"));
    scene()->stack()->push(new Commands::AddItem(frame, scene(), tr("Add decoration")));
    for(auto selectedItem : scene()->selectedItems())
      undoStack->push(new Commands::SetParentItem(selectedItem, frame));
    undoStack->endMacro();
  }

} // namespace Molsketch
