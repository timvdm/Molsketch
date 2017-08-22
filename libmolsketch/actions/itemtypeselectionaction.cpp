/***************************************************************************
 *   Copyright (C) 2017 by Hendrik Vennekate                               *
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
#include "itemtypeselectionaction.h"
#include "ui_typelistdialog.h"
#include <QDebug>

namespace Molsketch {

ItemTypeSelectionAction::ItemTypeSelectionAction(MolScene *parent)
  : AbstractItemAction(parent){
  setMinimumItemCount(1);
  setIcon(QIcon(":images/select-by-type.svg"));
  setWhatsThis(tr("Select items by type within the current selection or the entire document if nowthing is selected"));
  setToolTip(tr("Select by type"));
  setText(tr("Select by type..."));
  setObjectName("item-type-selection-action");
}

void ItemTypeSelectionAction::execute() {
  qInfo() << "Executing type selection action" << this;
  QDialog dialog;
  Ui::TypeListDialog ui;
  ui.setupUi(&dialog);
  dialog.exec();
}

} // namespace Molsketch
