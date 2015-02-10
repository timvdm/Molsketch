#include "multiaction.h"
#include <QMenu>

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
            this, SLOT(trigger()));
  }

  multiAction::~multiAction()
  {
    delete d->menu ;
    delete d ;
  }

  void multiAction::addSubAction(QAction *a)
  {
    a->setCheckable(true);
    d->menu->addAction(a) ;
    d->actionGroup->addAction(a);
    if (!d->actionGroup->checkedAction())
    {
      a->setChecked(true);
      setIcon(a->icon());
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

  void multiAction::changeIcon()
  {
    setIcon(d->actionGroup->checkedAction()->icon());
  }

}
