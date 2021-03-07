/***************************************************************************
 *   Copyright (C) 2018 Hendrik Vennekate                                  *
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
#ifndef LIBRARYTOOLBOX_H
#define LIBRARYTOOLBOX_H

#include <QDockWidget>

class LibraryToolBoxPrivate;
class ApplicationSettings;

class LibraryToolBox : public QDockWidget {
	Q_OBJECT
  Q_DECLARE_PRIVATE(LibraryToolBox)
  QScopedPointer<LibraryToolBoxPrivate> d_ptr;
public:
  explicit LibraryToolBox(const QStringList &folders, QWidget *parent = 0);
	~LibraryToolBox();
public slots:
  void rebuildLibraries(const QStringList &folders);
};
#endif // LIBRARYTOOLBOX_H
