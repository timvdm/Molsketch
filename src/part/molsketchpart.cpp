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

#include "molsketchpart.h"

#include "molscene.h"
#include "molview.h"
#include "fileio.h"

#include <kparts/partmanager.h>
#include <kparts/mainwindow.h>

#include <QtGui/QSplitter>
#include <QtCore/QFile>
#include <QtGui/QPrinter>
#include <QtCore/QTextStream>
#include <QtGui/QTextEdit>

#include <kaboutdata.h>
#include <kapplication.h>
#include <kdebug.h>
#include <kaction.h>
#include <kactioncollection.h>
#include <klocale.h>
#include <kstatusbar.h>
#include <kstandarddirs.h>

MolsKetchPart::MolsKetchPart( QWidget * parentWidget, QObject * parent, const QStringList & ) : KParts::ReadWritePart ( parent )
{

  setComponentData( MolsKetchFactory::componentData() );
  KIcon("logo", iconLoader());
  emit setWindowCaption("molsKetchPart");
  emit setStatusBarText("Ready");

  m_scene = new MolScene( parentWidget );
  m_view = new MolView( m_scene );
  m_view->setFocus();
  setWidget( m_view );

  // Add a temporary data location
  componentData().dirs()->addResourceDir( "data", MSKSRCDIR );
  setXMLFile( "molsketchpart.rc" );

  // File action

  // Edit actions
//   KAction * undo = new KAction( KIcon( "edit-undo"), i18n("Undo"), this );
//   actionCollection()->addAction( "edit_undo", undo );
//   connect( undo, SIGNAL( triggered() ), this,  SLOT( slotUndo() ));
// 
//   KAction * redo = new KAction( KIcon( "edit-redo"), i18n("Redo"), this );
//   actionCollection()->addAction( "edit_redo", redo );
//   connect( redo, SIGNAL( triggered() ), this,  SLOT( slotRedo() ));
// 
//   KAction * cut = new KAction( KIcon( "edit-cut"), i18n("Cut"), this );
//   actionCollection()->addAction( "edit_cut", cut );
//   connect( cut, SIGNAL( triggered() ), this,  SLOT( slotCut() ));
// 
//   KAction * copy = new KAction( KIcon( "edit-copy"), i18n("Copy"), this );
//   actionCollection()->addAction( "edit_copy", copy );
//   connect( copy, SIGNAL( triggered() ), this,  SLOT( slotCopy() ));
// 
//   KAction * paste = new KAction( KIcon( "edit-paste"), i18n("Paste"), this );
//   actionCollection()->addAction( "edit_paste", paste );
//   connect( paste, SIGNAL( triggered() ), this,  SLOT( slotPaste() ));
// 
//   KAction * selectAll = new KAction( KIcon( "edit-select-all"), i18n("Select All"), this );
//   actionCollection()->addAction( "edit_selectall", selectAll );
//   connect( selectAll, SIGNAL( triggered() ), this,  SLOT( slotSelectAll() ));

  KAction * addMode = new KAction( KIcon( "draw-freehand"), i18n("Add mode"), this );
  actionCollection()->addAction( "edit_addmode", addMode );
  connect( addMode, SIGNAL( triggered() ), this,  SLOT( slotAddMode() ));

  KAction * delMode = new KAction( KIcon( "draw-eraser"), i18n("Remove mode"), this );
  actionCollection()->addAction( "edit_delmode", delMode );
  connect( delMode, SIGNAL( triggered() ), this,  SLOT( slotDelMode() ));

  KAction * moveMode = new KAction( KIcon( "transform-move"), i18n("Move mode"), this );
  actionCollection()->addAction( "edit_movemode", moveMode );
  connect( moveMode, SIGNAL( triggered() ), this,  SLOT( slotMoveMode() ));

  KAction * rotMode = new KAction( KIcon( "transform-rotate"), i18n("Rotate mode"), this );
  actionCollection()->addAction( "edit_rotmode", rotMode );
  connect( rotMode, SIGNAL( triggered() ), this,  SLOT( slotRotMode() ));

  // View actions
//   KAction * zoomIn = new KAction( KIcon( "zoom-in"), i18n("Zoom in"), this );
//   actionCollection()->addAction( "zoomin", zoomIn );
//   connect( zoomIn, SIGNAL( triggered() ), this,  SLOT( slotZoomIn() ));
// 
//   KAction * zoomOut = new KAction( KIcon( "zoom-out"), i18n("Zoom out"), this );
//   actionCollection()->addAction( "zoomout", zoomOut );
//   connect( zoomOut, SIGNAL( triggered() ), this,  SLOT( slotZoomOut() ));

//   KAction * zoomFit = new KAction( KIcon( "zoom-fit-best"), i18n("Zoom fit"), this );
//   actionCollection()->addAction( "zoomfit", zoomFit );
//   connect( zoomFit, SIGNAL( triggered() ), this,  SLOT( slotZoomFit() ));
// 
//   KAction * zoomReset = new KAction( KIcon( "zoom-original"), i18n("Zoom reset"), this );
//   actionCollection()->addAction( "zoomreset", zoomReset );
//   connect( zoomReset, SIGNAL( triggered() ), this,  SLOT( slotZoomReset() ));

  setReadWrite( true );
}

MolsKetchPart::~MolsKetchPart()
{

}

void MolsKetchPart::setReadWrite( bool rw )
{
  m_view->setInteractive( rw );
  if ( rw )
    connect( m_scene, SIGNAL( documentChanged() ), this, SLOT( setModified() ) );
  else
    disconnect( m_scene, SIGNAL( documentChanged() ), this, SLOT( setModified() ) );

  ReadWritePart::setReadWrite( rw );
}

bool MolsKetchPart::openFile()
{
  kDebug() << "MolsKetchPart: opening " << localFilePath() << endl;
  QFile f(localFilePath());
  QString s;
  if ( f.open( QIODevice::ReadOnly ) )
  {
    m_scene->addMolecule(molsKetch::loadFile(localFilePath()));
    f.close();
  }

  return true;
}

bool MolsKetchPart::saveFile()
{
  if ( !isReadWrite() )
    return false;
  QFile f(localFilePath());
  if ( f.open(QIODevice::WriteOnly) )
  {
    molsKetch::saveFile(localFilePath(), m_scene);
    f.close();
    return true;
  }
  else
    return false;

}

// Edit action slots

void MolsKetchPart::slotUndo()
{
  m_scene->stack()->undo();
}

void MolsKetchPart::slotRedo()
{
  m_scene->stack()->redo();
}

void MolsKetchPart::slotCut()
{
  m_scene->cut();
}

void MolsKetchPart::slotCopy()
{
  m_scene->copy();
}

void MolsKetchPart::slotPaste()
{
  m_scene->paste();
}

void MolsKetchPart::slotSelectAll()
{
  m_scene->selectAll();
}

void MolsKetchPart::slotAddMode()
{
  m_scene->setEditMode( MolScene::AddMode );
}

void MolsKetchPart::slotDelMode()
{
  m_scene->setEditMode( MolScene::RemoveMode );
}

void MolsKetchPart::slotMoveMode()
{
  m_scene->setEditMode( MolScene::MoveMode );
}

void MolsKetchPart::slotRotMode()
{
  m_scene->setEditMode( MolScene::RotateMode );
}


// View action slots

void MolsKetchPart::slotZoomIn()
{
  m_view->scale( 2, 2 );
}

void MolsKetchPart::slotZoomOut()
{
  m_view->scale( 0.5, 0.5 );
}

void MolsKetchPart::slotZoomFit()
{
  m_view->fitInView( m_scene->itemsBoundingRect(), Qt::KeepAspectRatio );
}

void MolsKetchPart::slotZoomReset()
{
  m_view->resetMatrix();
}

KAboutData * MolsKetchPart::createAboutData()
{
  return new KAboutData( "molsketchpart", 0, ki18n( "molsKetch" ), "Helium", ki18n("A KPart to draw 2D molecular structures"), KAboutData::License_GPL, ki18n("Copyright 2008 (c) Harm van Eersel"), ki18n(""), "http://molsketch.sourceforge.net", "devsciurus@xs4all.nl" );
}

void MolsKetchPart::slotClear()
{
  m_scene->clear();
}

void MolsKetchPart::slotPrint(QPrinter & printer)
{
  molsKetch::printFile(printer, m_scene);
}
