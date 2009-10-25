/***************************************************************************
 *   Copyright (C) 2009 Tim Vandermeersch                                  *
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
#include "mechanismarrowdialog.h"

#include "mechanismarrow.h"

#include "ui_mechanismarrowdialog.h"

namespace Molsketch {

  MechanismArrowDialog::MechanismArrowDialog(MechanismArrow *arrow, QWidget *parent) :
    QDialog(parent), m_ui(new Ui::MechanismArrowDialog), m_arrow(arrow)
  {
    m_ui->setupUi(this);

    connect(m_ui->typeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(typeChanged(int)));
  }

  void MechanismArrowDialog::typeChanged(int t)
  {
    m_arrow->setArrowType(static_cast<MechanismArrow::ArrowType>(t));  
    m_arrow->update();
  }

  MechanismArrowDialog::~MechanismArrowDialog()
  {
    delete m_ui;
  }

}
