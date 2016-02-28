#include <QMenu>

#include "itemtypewidget.h"
#include "itemgrouptypeaction.h"

namespace Molsketch {

  struct ItemGroupTypeAction::privateData
  {
    ItemTypeWidget *typeWidget;
    QMenu *menu;
    bool transferingItemType;
  };

  ItemGroupTypeAction::ItemGroupTypeAction(MolScene *scene)
    : abstractItemAction(scene),
      d(new privateData)
  {
    d->menu = new QMenu;
    d->typeWidget = 0;
    QVBoxLayout *layout = new QVBoxLayout(d->menu);
    d->menu->setLayout(layout);
    connect(this, SIGNAL(itemsChanged()), this, SLOT(checkItemType()));
    setMinimumItemCount(1);
    setMenu(d->menu);
    setCheckable(false);
    d->transferingItemType = false;
  }

  ItemGroupTypeAction::~ItemGroupTypeAction()
  {
    delete d->menu;
    delete d;
  }

  void ItemGroupTypeAction::setItemTypeWidget(ItemTypeWidget *widget)
  {
    delete d->typeWidget;
    d->typeWidget = widget;
    widget->setParent(d->menu);
    d->menu->layout()->addWidget(widget);
    connect(widget, SIGNAL(currentTypeChanged(int)), d->menu, SLOT(close()));
    connect(widget, SIGNAL(currentTypeChanged(int)), this, SLOT(trigger()));
  }

  QString ItemGroupTypeAction::undoName() const
  {
    return tr("Set ") + text().toLower();
  }

  int ItemGroupTypeAction::defaultType() const
  {
    return -1;
  }

  QVariant ItemGroupTypeAction::defaultData() const
  {
    return QVariant();
  }

  void ItemGroupTypeAction::execute()
  {
    if (d->transferingItemType) return;
    applyType(d->typeWidget->currentType(), d->typeWidget->currentData());
  }

  void ItemGroupTypeAction::checkItemType()
  {
    if (!d->typeWidget) return;
    d->transferingItemType = true;
    int type = defaultType();
    QVariant data = defaultData();
    getType(type, data);
    if (type != -1) d->typeWidget->setCurrentType(type);
    else if (data.isValid()) d->typeWidget->setCurrentType(data);
    d->transferingItemType = false;
  }


} // namespace Molsketch
