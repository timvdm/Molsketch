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

#ifndef MOLSKETCHPART_H
#define MOLSKETCHPART_H

#include <kparts/genericfactory.h>
#include <kparts/part.h>

class MolView;
class KAboutData;
class MolScene;
class QPrinter;

class MolsKetchPart : public KParts::ReadWritePart
{
  Q_OBJECT

public:
  MolsKetchPart( QWidget * parentWidget, QObject * parent, const QStringList & args = QStringList() );
  virtual ~MolsKetchPart();

  virtual void setReadWrite( bool rw );

  static KAboutData * createAboutData();

protected:
  virtual bool openFile();
  virtual bool saveFile();

public Q_SLOTS:

  // File action slots
  void slotClear();
  void slotPrint(QPrinter & printer);

  // Edit action slots
  void slotUndo();
  void slotRedo();

  void slotCut();
  void slotCopy();
  void slotPaste();

  void slotSelectAll();

  void slotAddMode();
  void slotDelMode();
  void slotMoveMode();
  void slotRotMode();

  // View action slots
  void slotZoomIn();
  void slotZoomOut();
  void slotZoomFit();
  void slotZoomReset();

protected:
  MolView * m_view;
  MolScene * m_scene;
};

typedef KParts::GenericFactory<MolsKetchPart> MolsKetchFactory;

#endif
