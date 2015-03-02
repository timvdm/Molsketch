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
#include "bondtypewidget.h"
#include <QButtonGroup>
#include <QToolButton>
#include <QHBoxLayout>

namespace Molsketch {

  class bondTypeWidget::privateData
  {
    void createButton(const QString& IconName,
                      const bondTypeWidget::BondType& type,
                      bool forwardAndBackward = false) ;
  public:
    QButtonGroup *buttonGroup ;
    QHBoxLayout *layout ;
    bondTypeWidget *parent;
    privateData(bondTypeWidget* p)
      : buttonGroup(new QButtonGroup(p)),
        layout(new QHBoxLayout(p)),
        parent(p)
    {
      buttonGroup->setExclusive(true) ;
      createButton("single", Bond::Single) ;
      createButton("hash", Bond::Hash, true) ;
      createButton("wedge", Bond::Wedge, true) ;
      createButton("hashOrWedge", Bond::WedgeOrHash) ;
      createButton("double", Bond::Double) ;
      createButton("cistrans", Bond::CisOrTrans) ;
      createButton("triple", Bond::Triple) ;
      buttonGroup->button(Bond::Single)->setChecked(true) ;
    }
  };

  void bondTypeWidget::privateData::createButton(const QString &IconName, const BondType &type, bool forwardAndBackward)
  {
    QToolButton *button = new QToolButton(parent) ;
    buttonGroup->addButton(button, type) ;
    QImage icon(":images/" + IconName + ".png") ;
    button->setIcon(QPixmap::fromImage(icon)) ;
    button->setAutoRaise(true);
    button->setCheckable(true);
    layout->addWidget(button);

    if (!forwardAndBackward) return;
    button = new QToolButton(parent);
    buttonGroup->addButton(button, -(int) type);
    button->setIcon(QPixmap::fromImage(icon.mirrored(true, true)));
    button->setAutoRaise(true);
    button->setCheckable(true);
    layout->addWidget(button);
    layout->setMargin(0);
  }

  bondTypeWidget::bondTypeWidget(QWidget *parent)
    : QWidget(parent),
      d(new privateData(this))
  {
    connect(d->buttonGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(changeType())) ;
  }

  bondTypeWidget::~bondTypeWidget()
  {
    delete d ;
  }

  bool bondTypeWidget::backward() const
  {
    return d->buttonGroup->checkedId() < 0 ;
  }

  bondTypeWidget::BondType bondTypeWidget::bondType() const
  {
    return (BondType) qAbs(d->buttonGroup->checkedId()) ;
  }

  void bondTypeWidget::setBondType(bondTypeWidget::BondType type) const
  {
    QAbstractButton *button = d->buttonGroup->button(type);
    if (button) button->setChecked(true);
  }

  QPixmap bondTypeWidget::bondIcon() const
  {
    return d->buttonGroup->checkedButton()->icon().pixmap(d->buttonGroup->checkedButton()->size()) ;
  }

  int bondTypeWidget::bondOrder() const
  {
    return Bond::orderFromType((BondType) d->buttonGroup->checkedId());
  }

  void bondTypeWidget::changeType()
  {
    emit bondTypeChanged(bondType());
  }

} // namespace
