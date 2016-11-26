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
#include "atompopup.h"
#include "ui_atompopup.h"
#include "coordinatemodel.h"
#include "atom.h"

namespace Molsketch {

  struct AtomPopup::PrivateData
  {
    Atom* atom;
    Ui::AtomPopup* ui;

    void getFromAtom()
    {
      ui->element->setText(atom->element());
      ui->charge->setValue(atom->charge());
      ui->hydrogens->setValue(atom->numImplicitHydrogens());
      ui->coordinates->model()->setCoordinates(atom->coordinates());
    }

    void setToAtom()
    {
      atom->setElement(ui->element->text());
      atom->setCharge(ui->charge->value());
      atom->setNumImplicitHydrogens(ui->hydrogens->value());
      atom->setCoordinates(ui->coordinates->model()->getCoordinates());
    }
  };

  AtomPopup::AtomPopup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AtomPopup),
    d(new PrivateData)
  {
    ui->setupUi(this);
    d->ui = ui;
  }

  AtomPopup::~AtomPopup()
  {
    delete ui;
    delete d;
  }

  void AtomPopup::connectAtom(Atom *a)
  {
    // tell old atom to release
    d->atom = a;
    if (!a) return;
    d->getFromAtom();
  }

  void AtomPopup::applyPropertiesToAtom()
  {
    if (!d->atom) return;
    d->setToAtom();
  }
} // namespace
