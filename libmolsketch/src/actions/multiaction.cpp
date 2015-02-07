#include "multiaction.h"
#include <QMenu>

namespace Molsketch {

  struct multiAction::privateData
  {
    QMenu *menu ;
    QAction *activeAction ;
  };

  multiAction::multiAction(MolScene *scene)
    : genericAction(scene),
      d(new privateData)
  {
    d->menu = new QMenu() ;
    setMenu(d->menu) ;
    d->activeAction = 0 ;
    setIconVisibleInMenu(false) ;
  }

  multiAction::~multiAction()
  {
    delete d->menu ;
    delete d ;
  }

  void multiAction::addSubAction(QAction *a)
  {
    if (d->menu->actions().isEmpty())
    {
      setIcon(a->icon());
      d->activeAction = a ;
    }
    d->menu->addAction(a) ;
    connect(a, SIGNAL(triggered()), this, SLOT(subActionSlot())) ;
    connect(a, SIGNAL(toggled(bool)), this, SLOT(subActionSlot(bool))) ;
  }

  void multiAction::addSeparator()
  {
    d->menu->addSeparator() ;
  }

  QAction *multiAction::activeSubAction() const
  {
    return d->activeAction ;
  }

  void multiAction::subActionSlot(bool active)
  {
    Q_UNUSED(active)
    QAction* subAction = qobject_cast<QAction*>(sender()) ;
    if (!subAction) return ;
    setIcon(subAction->icon());
    d->activeAction = subAction ;
    setChecked(true);
  }


}
