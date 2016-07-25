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
#include "moleculepopup.h"
#include "ui_moleculepopup.h"

#include <molecule.h>
#include <molscene.h>
#include "commands.h"

namespace Molsketch {

  struct MoleculePopup::privateData
  {
    Molecule *molecule;
  };

  MoleculePopup::MoleculePopup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MoleculePopup),
    d(new privateData)
  {
    ui->setupUi(this);
    d->molecule = 0;
    connect(ui->nameEdit, SIGNAL(editingFinished()), this, SLOT(applyPropertiesToMolecule()));

    setWindowFlags(Qt::Popup);
  }

  void MoleculePopup::connectMolecule(Molecule *m)
  {
    d->molecule = m;
    if (!m) return;
    ui->nameEdit->setText(m->getName());
  }

  MoleculePopup::~MoleculePopup()
  {
    delete ui;
    delete d;
  }

  void MoleculePopup::focusOutEvent(QFocusEvent *e)
  {
    Q_UNUSED(e)
    deleteLater();
  }

  void MoleculePopup::showEvent(QShowEvent *e)
  {
    connectMolecule(d->molecule);
    QWidget::showEvent(e);
  }

  void MoleculePopup::applyPropertiesToMolecule()
  {
    if (!d->molecule) return;
    MolScene *scene = qobject_cast<MolScene*>(d->molecule->scene());
    if (scene)
      scene->stack()->push(new Commands::ChangeMoleculeName(d->molecule, ui->nameEdit->text(), tr("Change name of molecule")));
    else
      d->molecule->setName(ui->nameEdit->text());
  }

} // namespace
