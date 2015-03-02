#include "bondtypeaction.h"
#include "bondtypewidget.h"
#include "commands.h"

#include <QMenu>
#include <QVBoxLayout>
#include <QDebug>

namespace Molsketch {

  struct bondTypeAction::privateData
  {
    bondTypeWidget *typeWidget;
    QMenu *menu;
    bool transferingBondType;
  };

  bondTypeAction::bondTypeAction(MolScene *scene)
    : abstractItemAction(scene),
      d(new privateData)
  {
    d->menu = new QMenu;
    d->typeWidget = new bondTypeWidget(d->menu);
    QVBoxLayout *layout = new QVBoxLayout(d->menu);
    d->menu->setLayout(layout);
    layout->addWidget(d->typeWidget);
    connect(d->typeWidget, SIGNAL(bondTypeChanged(bondTypeWidget::BondType)), d->menu, SLOT(close()));
    connect(d->typeWidget, SIGNAL(bondTypeChanged(bondTypeWidget::BondType)), this, SLOT(trigger()));
    connect(this, SIGNAL(itemsChanged()), this, SLOT(checkBondType()));
    setMinimumItemCount(1);
    setText(tr("Bond type"));
    setMenu(d->menu);
    setCheckable(false);
    d->transferingBondType = false;
  }

  bondTypeAction::~bondTypeAction()
  {
    delete d->menu;
    delete d;
  }

  void bondTypeAction::execute()
  {
    if (d->transferingBondType) return;
    qDebug() << "Beginning change bond type";
    attemptBeginMacro(tr("change bond type"));
    foreach(graphicsItem *item, items())
    {
      if (item->type() != Bond::Type) continue;
      Bond *bond = dynamic_cast<Bond*>(item);
      if (!bond) continue; // TODO shouldn't happen
      attemptUndoPush(new Commands::SetBondType(bond, d->typeWidget->bondType()));
      if (d->typeWidget->backward())
        attemptUndoPush(new Commands::SwapBondAtoms(bond));
    }
    attemptEndEndMacro();
  }

  void bondTypeAction::checkBondType()
  {
    d->transferingBondType = true;
    d->typeWidget->setBondType(Bond::Single);
    foreach(graphicsItem* item, items())
    {
      Bond *bond = dynamic_cast<Bond*>(item);
      if (bond)
      {
        d->typeWidget->setBondType(bond->bondType());
        break;
      }
    }
    d->transferingBondType = false;
  }

} // namespace Molsketch

