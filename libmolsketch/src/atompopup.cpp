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
#include "commands.h"
#include "atom.h"
#include "molscene.h"
#include <QDebug>

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
  };

  AtomPopup::AtomPopup(QWidget *parent) :
    PropertiesWidget(parent),
    ui(new Ui::AtomPopup),
    d(new PrivateData)
  {
    ui->setupUi(this);
    d->ui = ui;
    d->atom = 0;
    connect(ui->coordinates->model(), SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)), this, SLOT(onCoordinatesDatachanged()));
  }

  AtomPopup::~AtomPopup()
  {
    delete ui;
    delete d;
  }

  void AtomPopup::connectAtom(Atom *a)
  {
    d->atom = a;
    setScene(a ? dynamic_cast<MolScene*>(a->scene()) : 0);
  }

  void AtomPopup::on_element_textChanged(const QString &arg1)
  {
    Q_UNUSED(arg1)
    attemptToPushUndoCommand(new Commands::ChangeElement(d->atom, ui->element->text()));
  }

  void AtomPopup::on_charge_valueChanged(int arg1)
  {
    Q_UNUSED(arg1)
    attemptToPushUndoCommand(new Commands::setAtomChargeCommand(d->atom, ui->charge->value()));
  }

  void AtomPopup::on_hydrogens_valueChanged(int arg1)
  {
    Q_UNUSED(arg1)
    attemptToPushUndoCommand(new Commands::setImplicitHydrogensCommand(d->atom, ui->hydrogens->value()));
  }

  void AtomPopup::onCoordinatesDatachanged()
  {
    if (!d->atom) return;
    attemptToPushUndoCommand(Commands::MoveItem::absolute(d->atom, ui->coordinates->model()->getCoordinates().first()));
  }

  void AtomPopup::propertiesChanged()
  {
    if (!scene()) return; // TODO reset GUI
    d->getFromAtom();
  }
} // namespace
