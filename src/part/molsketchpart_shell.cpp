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

#include "molsketchpart_shell.h"
#include "molsketchpart.h"

#include <kiconloader.h>
#include <kstandarddirs.h>
#include <kapplication.h>
#include <kaction.h>
#include <kactioncollection.h>
#include <klocale.h>
#include <kfiledialog.h>
#include <kmessagebox.h>
#include <kcmdlineargs.h>
#include <klibloader.h>
#include <kicon.h>

#include <QtGui/QWidget>
#include <QtGui/QSplitter>
#include <QtGui/QPrintDialog>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtGui/QPrinter>


#include <kparts/componentfactory.h>
#include <klibloader.h>

int main(int argc, char **argv)
{
  KCmdLineOptions options;
  options.add("+file(s)", ki18n("Files to load"));

  const char version[] = "Helium 2008 (c) Harm van Eersel";
  KLocalizedString description = ki18n("This is a test shell for molsKetch part.");

  KCmdLineArgs::init( argc, argv, "molsketchpartshell", 0, ki18n("molsKetchPart Shell"), version, description );
  KCmdLineArgs::addCmdLineOptions( options );
  KApplication app;
  KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
  Shell * shell = new Shell;
  if ( args->count() ==  1 )
  {
    KUrl url( QDir::currentPath() + "/", args->arg(0) );
    shell->openUrl( url );
  }
  shell->show();
  return app.exec();
}


Shell::Shell()
{
  // Set window icon
  QIcon icon;
  icon.addFile(":/molsketch.svg");
  setWindowIcon(icon);  

  m_mskpart = new MolsKetchPart( this, this );

//   setAboutData(m_mskpart->createAboutData());

  // Set tempory data location
  KGlobal::dirs()->addResourceDir( "data", MSKSRCDIR );
  setXMLFile( "molsketchpart_shell.rc" );

  // File actions
  KAction * paNew = new KAction( KIcon( "document-new" ), i18n( "&New file" ), this );
  actionCollection()->addAction( "file_new", paNew );
  connect( paNew, SIGNAL( triggered() ), m_mskpart, SLOT( slotClear() ));

  KAction * paOpen = new KAction( KIcon( "document-open" ), i18n( "&Open file" ), this );
  actionCollection()->addAction( "file_open", paOpen );
  connect( paOpen, SIGNAL( triggered() ), this, SLOT( slotFileOpen() ));

  KAction * paSave = new KAction( KIcon( "document-save" ), i18n( "&Save file" ), this );
  actionCollection()->addAction( "file_save", paSave );
  connect( paSave, SIGNAL( triggered() ), this, SLOT( slotFileSave() ));

  KAction * paSaveAs = new KAction( KIcon( "document-save-as" ), i18n( "&Save file as" ), this );
  actionCollection()->addAction( "file_save_as", paSaveAs );
  connect( paSaveAs, SIGNAL( triggered() ), this, SLOT( slotFileSaveAs() ));

  KAction * paPrint = new KAction( KIcon( "document-print" ), i18n( "&Print..." ), this );
  actionCollection()->addAction( "file_print", paPrint );
  connect( paPrint, SIGNAL( triggered() ), this, SLOT( slotFilePrint() ));

  KAction * paQuit = new KAction( KIcon( "application-exit" ), i18n( "&Quit" ), this );
  actionCollection()->addAction( "file_quit", paQuit );
  connect( paQuit, SIGNAL( triggered() ), this, SLOT( close() ));

  // Edit actions
  KAction * paUndo = new KAction( KIcon( "edit-undo" ), i18n( "&Undo" ), this );
  actionCollection()->addAction( "edit_undo", paUndo );
  connect( paUndo, SIGNAL( triggered() ), m_mskpart, SLOT( slotUndo() ));

  KAction * paRedo = new KAction( KIcon( "edit-redo" ), i18n( "&Redo" ), this );
  actionCollection()->addAction( "edit_redo", paRedo );
  connect( paRedo, SIGNAL( triggered() ), m_mskpart, SLOT( slotRedo() ));

  KAction * paCut = new KAction( KIcon( "edit-cut" ), i18n( "&Cut" ), this );
  actionCollection()->addAction( "edit_cut", paCut );
  connect( paCut, SIGNAL( triggered() ), m_mskpart, SLOT( slotCut() ));

  KAction * paCopy = new KAction( KIcon( "edit-copy" ), i18n( "&Copy" ), this );
  actionCollection()->addAction( "edit_copy", paCopy );
  connect( paCopy, SIGNAL( triggered() ), m_mskpart, SLOT( slotCopy() ));

  KAction * paPaste = new KAction( KIcon( "edit-paste" ), i18n( "&Paste" ), this );
  actionCollection()->addAction( "edit_paste", paPaste );
  connect( paPaste, SIGNAL( triggered() ), m_mskpart, SLOT( slotPaste() ));

  KAction * paSelectAll = new KAction( KIcon( "edit-select-all" ), i18n( "&Select all" ), this );
  actionCollection()->addAction( "edit_select_all", paSelectAll );
  connect( paSelectAll, SIGNAL( triggered() ), m_mskpart, SLOT( slotSelectAll() ));


  // View actions
  KAction * paZoomIn = new KAction( KIcon( "zoom-in" ), i18n( "&Zoom in" ), this );
  actionCollection()->addAction( "view_zoom_in", paZoomIn );
  connect( paZoomIn, SIGNAL( triggered() ), m_mskpart, SLOT( slotZoomIn() ));

  KAction * paZoomOut = new KAction( KIcon( "zoom-out" ), i18n( "&Zoom out" ), this );
  actionCollection()->addAction( "view_zoom_out", paZoomOut );
  connect( paZoomOut, SIGNAL( triggered() ), m_mskpart, SLOT( slotZoomOut() ));

  KAction * paZoomFit = new KAction( KIcon( "zoom-fit-best" ), i18n( "&Zoom fit best" ), this );
  actionCollection()->addAction( "view_fit_to_page", paZoomFit );
  connect( paZoomFit, SIGNAL( triggered() ), m_mskpart, SLOT( slotZoomFit() ));

  KAction * paZoomOriginal = new KAction( KIcon( "zoom-original" ), i18n( "&Zoom reset" ), this );
  actionCollection()->addAction( "view_actual_size", paZoomOriginal );
  connect( paZoomOriginal, SIGNAL( triggered() ), m_mskpart, SLOT( slotZoomReset() ));

//   // Try to find a molsKetch part
//   m_mskpart = KParts::ComponentFactory::createPartInstanceFromQuery<KParts::ReadWritePart>( "application", QString(), 
// this, this );
// 
//   // Try to find libmolsketch
//   if ( !m_mskpart )
//   {
//     KLibFactory * factory = KLibLoader::self()->factory( "libmolsketchpart" );
//     if (factory)
//     {
//       // Create the part
//       m_mskpart = factory->create<KParts::ReadWritePart>( this );
//     }
//     else
//     {
//       KMessageBox::error( this, "No libmolsketchpart found !" );
//     }
//   }
// 
//   if ( m_mskpart )
//   {
//       // Set the main widget
//       setCentralWidget( m_mskpart->widget() );
//       // Integrate its GUI
//       createGUI( m_mskpart );
//   }

  setCentralWidget( m_mskpart->widget() );
  createGUI( m_mskpart );

  resize( 800, 600 );
}

Shell::~Shell()
{
  delete m_mskpart;
}

void Shell::openUrl( const KUrl & url )
{
  m_mskpart->openUrl( url );
}

void Shell::saveUrl( const KUrl & url )
{
  m_mskpart->saveAs( url );
}

void Shell::slotFileNew()
{
  if (m_mskpart->closeUrl(true))
    m_mskpart->slotClear();
}

void Shell::slotFileOpen()
{
  KUrl url = KFileDialog::getOpenUrl( QString(), "*.mol|Molecule documents (*.mol)", 0L, "file dialog" );

  if ( !url.isEmpty() ) openUrl( url );
}

void Shell::slotFileSave()
{
  m_mskpart->save();
}

void Shell::slotFileSaveAs()
{
  KUrl url = KFileDialog::getSaveUrl( QString(), "*.mol|Molecule documents (*.mol)", 0L, "file dialog" );

  if ( !url.isEmpty() ) saveUrl( url );
}
void Shell::slotFilePrint()
{
  // Creating a new printerobject
  QPrinter printer(QPrinter::HighResolution);

  // Prompt for the printoptions
  QPrintDialog printDialog(&printer, this);

  if (printDialog.exec() == QDialog::Accepted)
    m_mskpart->slotPrint(printer);
}

