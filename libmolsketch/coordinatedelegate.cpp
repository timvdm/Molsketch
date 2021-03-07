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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/
#include "coordinatedelegate.h"

#include <QLineEdit>

namespace Molsketch {

  CoordinateDelegate::CoordinateDelegate(QObject *parent) :
    QItemDelegate(parent)
  {
  }

  CoordinateDelegate::~CoordinateDelegate()
  {
  }

  QWidget *CoordinateDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
  {
    Q_UNUSED(option)
    Q_UNUSED(index)
    auto edit = new QLineEdit(parent);
    edit->setValidator(new QDoubleValidator(edit));
    edit->setFrame(false);
    return edit;
  }

#define CASTEDITORMACRO QLineEdit *edit = qobject_cast<QLineEdit*>(editor); if (!edit) return;
  void CoordinateDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
  {
    CASTEDITORMACRO
    edit->setText(index.data(Qt::EditRole).toString());
  }

  void CoordinateDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
  {
    CASTEDITORMACRO
    model->setData(index, edit->text());
  }

} // namespace Molsketch
