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

  QList<QAction *> genericAction::actions()
  {
    return QList<QAction *>() << this ;
  }

  QString genericAction::toolbarName(QAction *action)
  {
    Q_UNUSED(action)
    return text() ;
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
      default: ;
    }
    if (event->isAccepted()) return true;
    return false;
  }

  void genericAction::attemptUndoPush(QUndoCommand *command)
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

  void genericAction::attemptBeginMacro(const QString &text)
  {
    MolScene *molscene = dynamic_cast<MolScene*>(scene());
    if (!molscene || !molscene->stack()) return;
    molscene->stack()->beginMacro(text);
  }

  void genericAction::attemptEndEndMacro()
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
