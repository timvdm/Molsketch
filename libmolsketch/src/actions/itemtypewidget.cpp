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

#include <QButtonGroup>
#include <QToolButton>

#include "itemtypewidget.h"

namespace Molsketch {

  struct ItemTypeWidget::privateData
  {
    QButtonGroup *buttonGroup ;
    QBoxLayout *layout ;
    ItemTypeWidget *parent;

    void createButton(const QPixmap& Icon,
                      const int& type)
    {
      QToolButton *button = new QToolButton(parent) ;
      buttonGroup->addButton(button, type) ;
      button->setIcon(Icon) ;
      button->setAutoRaise(true);
      button->setCheckable(true);
      layout->addWidget(button);
      layout->setMargin(0);
    }

    privateData(ItemTypeWidget* p)
      : buttonGroup(new QButtonGroup(p)),
        layout(new QHBoxLayout(p)),
        parent(p)
    {}
  };

  ItemTypeWidget::ItemTypeWidget(QWidget *parent)
    : QWidget(parent),
      d(new privateData(this))
  {
    connect(d->buttonGroup, SIGNAL(buttonClicked(int)), this, SIGNAL(currentTypeChanged(int)));
    d->buttonGroup->setExclusive(true);
  }

  ItemTypeWidget::~ItemTypeWidget()
  {
    delete d;
  }

  int ItemTypeWidget::currentType() const
  {
    return qAbs(d->buttonGroup->checkedId());
  }

  void ItemTypeWidget::setCurrentType(const int &type) const
  {
    QAbstractButton *button = d->buttonGroup->button(type);
    if (button) button->setChecked(true);
  }

  QPixmap ItemTypeWidget::currentIcon() const
  {
    return d->buttonGroup->checkedButton()->icon().pixmap(d->buttonGroup->checkedButton()->size());
  }

  void ItemTypeWidget::setDirection(QBoxLayout::Direction dir)
  {
    d->layout->setDirection(dir);
  }

  void ItemTypeWidget::setButtons(QList<typeIconPair> typeButtons)
  {
    foreach(QAbstractButton* button, d->buttonGroup->buttons())
      delete button;
    foreach(const typeIconPair& tip, typeButtons)
      d->createButton(tip.second, tip.first);
    if (!d->buttonGroup->buttons().isEmpty())
      d->buttonGroup->buttons().first()->setChecked(true);
  }


} // namespace Molsketch

