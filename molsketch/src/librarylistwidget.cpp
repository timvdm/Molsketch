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
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "librarylistwidget.h"
#include "mollibitem.h"
#include "molecule.h"
#include "molscene.h"

#include <QDrag>
#include <QMimeData>
#include <QXmlStreamWriter>

using namespace Molsketch;

LibraryListWidget::LibraryListWidget(QWidget *parent) :
  QListWidget(parent)
{
  setDragEnabled(true);
}

QStringList LibraryListWidget::mimeTypes() const
{
  return QStringList() << MolScene::mimeType();
}

QMimeData *LibraryListWidget::mimeData(const QList<QListWidgetItem *> items) const
{
  QByteArray data;
  QXmlStreamWriter xmlWriter(&data);
  xmlWriter.writeStartDocument();
  foreach(QListWidgetItem* item, items)
  {
    MolLibItem* mollibitem = dynamic_cast<MolLibItem*>(item);
    if (!mollibitem) continue;
    xmlWriter << mollibitem->getMolecule();
  }
  xmlWriter.writeEndDocument();
  QMimeData *result = new QMimeData;
  result->setData(MolScene::mimeType(), data);
  return result;
}

void LibraryListWidget::startDrag(Qt::DropActions supportedActions)
{
  QModelIndexList indexes = selectedIndexes();
  if (indexes.count() == 0) return;
  QMimeData *data = model()->mimeData(indexes);
  if (!data)
    return;
  QRect rect;
  rect.adjust(horizontalOffset(), verticalOffset(), 0, 0);
  QDrag *drag = new QDrag(this);
  drag->setMimeData(data);
//  drag->setHotSpot(d->pressedPosition - rect.topLeft());
  drag->exec(supportedActions, Qt::CopyAction);
}
