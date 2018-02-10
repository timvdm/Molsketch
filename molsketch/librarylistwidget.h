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
#ifndef LIBRARYLISTWIDGET_H
#define LIBRARYLISTWIDGET_H

#include <QDir>
#include "libraryview.h"

class LibraryListWidget : public Molsketch::LibraryView
{
  Q_OBJECT
public:
  explicit LibraryListWidget(QString directory, QWidget *parent = 0);
  QString title() const;
public slots:
  void refreshItems();
private:
  QDir folder;
};

#endif // LIBRARYLISTWIDGET_H
