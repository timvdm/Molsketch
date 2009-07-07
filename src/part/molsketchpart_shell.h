/***************************************************************************
 *   Copyright (C) 2007 by Harm van Eersel                                 *
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

#ifndef MOLSKETCHPART_SHELL_H
#define MOLSKETCHPART_SHELL_H

#include <kparts/partmanager.h>
#include <kparts/mainwindow.h>

class QWidget;
class MolsKetchPart;

class Shell : public KParts::MainWindow
{
  Q_OBJECT
public:
  Shell();
  virtual ~Shell();

  void openUrl( const KUrl & url );
  void saveUrl( const KUrl & url );

protected Q_SLOTS:
  void slotFileNew();
  void slotFileOpen();
  void slotFileSave();
  void slotFileSaveAs();
  void slotFilePrint();

private:
  MolsKetchPart * m_mskpart;
};

#endif
