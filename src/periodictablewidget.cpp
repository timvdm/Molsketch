/***************************************************************************
 *   Copyright (C) 2007-2008 by Harm van Eersel                            *
 *   devsciurus@xs4all.nl                                                  *
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

#include "periodictablewidget.h"
#include "element.h"

#include <QHeaderView>
#include <QSize>

PeriodicTableWidget::PeriodicTableWidget()
{
  // Setting properties
  setSelectionMode(QTableWidget::SingleSelection);
  setDragEnabled(false);
  setEditTriggers(QTableWidget::NoEditTriggers);

  // Setting appereance
//   setMaximumSize(460,260);
  setRowCount(10);
  setColumnCount(18);
  horizontalHeader()->hide();
  verticalHeader()->hide();
  int cellSize = 25;
  for (int i = 0; i < rowCount(); i++) setRowHeight(i, cellSize);
  for (int i = 0; i < columnCount(); i++) setColumnWidth(i, cellSize);

  // Inserting the elements
  for ( int i = 0; i < rowCount(); i++ )
    for ( int j = 0; j < columnCount(); j++ ) {
      QTableWidgetItem * item = new QTableWidgetItem(molsKetch::position2symbol(i + 1, j + 1));
      item->setTextAlignment(Qt::AlignCenter);
      setItem( i, j, item);
    }

  // Set background color
  for ( int i = 0; i < rowCount(); i++ )
    for ( int j = 0; j < 2; j++ ) {
      QTableWidgetItem * tItem = item(i,j);
      if (!tItem->text().isEmpty()) tItem->setBackground(QBrush(QColor(190, 114, 238)));
    }
  for ( int i = 0; i < rowCount(); i++ )
    for ( int j = 2; j < 12; j++ ) {
      QTableWidgetItem * tItem = item(i,j);
      if (!tItem->text().isEmpty()) tItem->setBackground(QBrush(QColor(244, 253, 64)));
    }
  for ( int i = 0; i < rowCount(); i++ )
    for ( int j = 12; j < 18; j++ ) {
      QTableWidgetItem * tItem = item(i,j);
      if (!tItem->text().isEmpty()) tItem->setBackground(QBrush(QColor(98, 238, 37)));
    }
  for ( int i = 8; i < 10; i++ )
    for ( int j = 0; j < columnCount(); j++ ) {
      QTableWidgetItem * tItem = item(i,j);
      if (!tItem->text().isEmpty()) tItem->setBackground(QBrush(QColor(238, 90, 184)));
    }
  QTableWidgetItem * tItem1 = item(5,2);
  tItem1->setBackground(QBrush(QColor(238, 90, 184)));
  QTableWidgetItem * tItem2 = item(6,2);
  tItem2->setBackground(QBrush(QColor(238, 90, 184)));

  // Make sure that only non-empty items can be selected
  m_previousItem = item(1,13);
  connect(this, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(checkSelection(QTableWidgetItem*)));
}

QSize PeriodicTableWidget::sizeHint() const
{
  return QSize(460,260);
}

void PeriodicTableWidget::checkSelection(QTableWidgetItem * tItem)
{
  if (tItem->text().isEmpty()) setCurrentItem(m_previousItem);
  else m_previousItem = tItem;
}
