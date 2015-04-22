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
#include "genericaction.h"
#include <QGraphicsSceneMouseEvent>
#include <QUndoStack>
#include <QtGui>
#include "molscene.h"

namespace Molsketch{

  genericAction::genericAction(MolScene *scene)
    : QAction(scene)
  {
    setProperty("exclusiveAction",1);
    setCheckable(true) ;
    connect(this, SIGNAL(toggled(bool)), this, SLOT(activationSlot(bool))) ;
  }

  MolScene *genericAction::scene() const
  {
    return parentWidget()->findChild<MolScene*>() ;
  }

  QUndoStack *genericAction::undoStack() const
  {
    MolScene *s = scene() ;
    if (!s) return 0 ;
    return s->stack() ;
  }

  bool genericAction::eventFilter(QObject *object, QEvent *event)
  {
    if (scene() != object) return false;
    event->ignore();
    switch(event->type())
    {
      case QEvent::GraphicsSceneMousePress:
        mousePressEvent(static_cast<QGraphicsSceneMouseEvent*>(event));
        break;
      case QEvent::GraphicsSceneMouseMove:
        mouseMoveEvent(static_cast<QGraphicsSceneMouseEvent*>(event));
        break;
      case QEvent::GraphicsSceneMouseRelease:
        mouseReleaseEvent(static_cast<QGraphicsSceneMouseEvent*>(event));
        break;
      case QEvent::GraphicsSceneMouseDoubleClick:
        mouseDoubleClickEvent(static_cast<QGraphicsSceneMouseEvent*>(event));
        break;
      case QEvent::Leave:
        leaveSceneEvent(event);
        break;
      case QEvent::Enter:
        enterSceneEvent(event);
        break;
      default: ;
    }
    if (event->isAccepted()) return true;
    return false;
  }

  void genericAction::attemptUndoPush(QUndoCommand *command) const
  {// TODO merge with graphisitem
    MolScene *molscene = dynamic_cast<MolScene*>(scene());
    if (!molscene || !molscene->stack())
    {
      command->redo();
      delete command;
    }
    else
      molscene->stack()->push(command) ;
  }

  void genericAction::attemptBeginMacro(const QString &text) const
  {
    MolScene *molscene = dynamic_cast<MolScene*>(scene());
    if (!molscene || !molscene->stack()) return;
    molscene->stack()->beginMacro(text);
  }

  void genericAction::attemptEndMacro() const
  {
    MolScene *molscene = dynamic_cast<MolScene*>(scene());
    if (!molscene || !molscene->stack()) return;
    molscene->stack()->endMacro();
  }

  void genericAction::activationSlot(const bool &b)
  {
    if (!scene()) return;
    if (b)
    {
      int exclusivity = property("exclusiveAction").toInt();
      if (exclusivity)
        foreach(QAction* other, scene()->findChildren<QAction*>())
          if (other != this
              && other->isCheckable()
              && other->isChecked()
              && other->property("exclusiveAction").toInt())
            other->setChecked(false);
      scene()->installEventFilter(this);
    }
    else   scene()->removeEventFilter(this);
  }


} // namespace
