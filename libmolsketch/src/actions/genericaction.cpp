#include "genericaction.h"
#include <QUndoStack>
#include "molscene.h"
#include "toolgroup.h"

namespace Molsketch{

  genericAction::genericAction(MolScene *scene)
    : QAction(scene),
      Tool(scene)
  {
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

  void genericAction::activationSlot(const bool &b)
  {
    if (b)
    {
      scene()->toolGroup()->setActiveTool(this) ;
      activated();
    }
    else deactivated();
  }


} // namespace
