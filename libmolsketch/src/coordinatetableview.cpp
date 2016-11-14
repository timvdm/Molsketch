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
#include "coordinatedelegate.h"
#include "coordinatetableview.h"

namespace Molsketch {

  CoordinateTableView::CoordinateTableView(QWidget *parent) :
    QTableView(parent)
  {
    QAbstractItemDelegate* oldDelegate = itemDelegate();
    setItemDelegate(new CoordinateDelegate(this));
    delete oldDelegate;
  }

  CoordinateModel *CoordinateTableView::model() const
  {
    return qobject_cast<CoordinateModel*>(QTableView::model());
  }

  void CoordinateTableView::setModel(CoordinateModel *model) // TODO dangerous: co-variant argument
  {
    QTableView::setModel(model);
  }

} // namespace Molsketch
