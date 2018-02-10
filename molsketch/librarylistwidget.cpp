/***************************************************************************
 *   Copyright (C) 2016 Hendrik Vennekate                                  *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/
#include <librarymodel.h>
#include <moleculemodelitem.h>
#include "fileio.h"

#include "librarylistwidget.h"
using namespace Molsketch;

// TODO acceptance test
// TODO check out batch size

void LibraryListWidget::refreshItems() {
  LibraryModel *moleculeModel = dynamic_cast<LibraryModel*>(model());
  if (!moleculeModel) return;
  folder.refresh();
  QList<MoleculeModelItem*> newItems;
  foreach(const QString& entry, folder.entryList())
    foreach(Molecule* molecule, moleculesFromFile(folder.filePath(entry)))
      if (molecule) newItems << MoleculeModelItem::fromMolecule(molecule);
  moleculeModel->setMolecules(newItems);
}

LibraryListWidget::LibraryListWidget(QString directory, QWidget *parent)
  : Molsketch::LibraryView(parent),
    folder(directory)
{
  setToolTip(directory);
  setModel(new Molsketch::LibraryModel(parent));
  refreshItems();
}

QString LibraryListWidget::title() const {
  return folder.dirName();
}
