/***************************************************************************
 *   Copyright (C) 2007-2008 by Harm van Eersel                            *
 *   Copyright (C) 2009 Tim Vandermeersch                                  *
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

#include <QLibrary>
#include <QtGui>
#include <QToolBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QStatusBar>
#include <QProgressBar>
#include <QPrintPreviewDialog>
#include <QMenuBar>
#include <QMenu>
#include <QDockWidget>
#include <QToolBox>
#include <QInputDialog>
#if QT_VERSION <= 0x040603
#include <QAssistantClient>
#else
#include <QProcess>
#endif
#include <QGridLayout>

#include <actions/arrowtypeaction.h>
#include <actions/bondtypeaction.h>
#include <actions/coloraction.h>
#include <actions/connectaction.h>
#include <actions/deleteaction.h>
#include <actions/drawaction.h>
#include <actions/flipbondaction.h>
#include <actions/frameaction.h>
#include <actions/frametypeaction.h>
#include <actions/incdecaction.h>
#include <actions/linewidthaction.h>
#include <actions/mechanismarrowaction.h>
#include <actions/ringaction.h>
#include <actions/rotateaction.h>
#include <actions/flipstereobondsaction.h>
#include <actions/textaction.h>

#include "mainwindow.h"

#include "molecule.h"
#include "molview.h"
#include "molscene.h"
#include "element.h"
#include "fileio.h"
#include "mollibitem.h"

#include "obabeliface.h"

// widgets
#include "helpforemptytoolbox.h"
#include "releasenotesdialog.h"
#include "settings.h"
#include "wikiquerywidget.h"


#define PROGRAM_NAME "Molsketch"

#define MSK_DEFAULT_FORMAT "MolsKetch default (*.msk)"
#define GRAPHIC_FILE_FORMATS "Scalable Vector Graphics (*.svg);;Portable Network Graphics (*.png);;Windows Bitmap (*.bmp);;Joint Photo Expert Group (*.jpeg)"
#define GRAPHIC_DEFAULT_FORMAT "Portable Network Graphics (*.png)"
#define OSRA_GRAPHIC_FILE_FORMATS "All supported types (*.*);;Images (*.png *.bmp *.jpg *.jpeg *.gif *.tif *.tiff);;Documents (*.pdf *.ps)"

#ifdef _WIN32
#define OBABELOSSUFFIX ".dll"
#else
#define OBABELOSSUFFIX
#endif

#define PREPARELOADFILE \
  QLibrary obabeliface("obabeliface" QTVERSIONSUFFIX OBABELOSSUFFIX); \
  obabeliface.load() ; \
  loadFileFunctionPointer loadFilePtr = 0 ; \
  formatsFunctionPointer inputFormats = 0; \
  if (obabeliface.isLoaded()) {\
    loadFilePtr = (loadFileFunctionPointer) (obabeliface.resolve("loadFile")) ;\
    inputFormats = (formatsFunctionPointer) (obabeliface.resolve("inputFormats")) ;\
  }

#define PREPARESAVEFILE \
  QLibrary obabeliface("obabeliface" QTVERSIONSUFFIX OBABELOSSUFFIX); \
  obabeliface.load() ; \
  saveFileFunctionPointer saveFilePtr = 0 ; \
  formatsFunctionPointer outputFormats = 0; \
  if (obabeliface.isLoaded()) {\
    saveFilePtr = (saveFileFunctionPointer) (obabeliface.resolve("saveFile")) ; \
    outputFormats = (formatsFunctionPointer) (obabeliface.resolve("outputFormats")); \
  }

// Constructor

using namespace Molsketch;

MainWindow::MainWindow()
{
  // Creating the menus and actions
  createView();
  createToolBox();
  createWikiDock();
  createActions();
  createMenus();
  createToolBars();
  createStatusBar();
  createToolBarContextMenuOptions();
  initializeAssistant();

  setWindowIcon(QIcon(":/images/molsketch.svg"));

  // Loading settings
  readSettings();

  QStringList args = qApp->arguments();
  args.removeFirst();
  QRegExp rx("^[^\\-]");
  QStringList loadedFiles;

  PREPARELOADFILE
  bool openBabelNeeded = false ;
  foreach(QString fileName, args.filter(rx))
  {
    if (fileName.endsWith(".msk"))
    {
      readMskFile(fileName, m_scene);
      loadedFiles << fileName;
    }
    else
    {
      openBabelNeeded = true ;
      if (!loadFilePtr) continue ;
      Molecule *mol = loadFilePtr(fileName);
      if (mol)
      {
        m_scene->addMolecule(mol);
        loadedFiles << fileName;
      }
      else
      {
        // Display error message if load fails
        QMessageBox::critical(this,tr(PROGRAM_NAME),tr("Error while loading file"),QMessageBox::Ok,QMessageBox::Ok);
      }
    }
  }
  if (openBabelNeeded)
    QMessageBox::critical(this, tr(PROGRAM_NAME), tr("Some files could not be loaded because support for OpenBabel is missing.")) ;

  setCurrentFile("");
  if (loadedFiles.count() == 1) setCurrentFile(loadedFiles.first());

  // Connecting signals and slots
  connect(m_scene->stack(),SIGNAL(cleanChanged(bool)), this, SLOT(documentWasModified( )));
//   connect(m_scene,SIGNAL(newMolecule(QPointF,QString)),this, SLOT(newMolecule(QPointF,QString)));
  connect(m_scene,SIGNAL(editModeChange(int)),this,SLOT(updateEditMode(int)));

  m_molView->setAcceptDrops(true);
}

QMenu *MainWindow::createPopupMenu()
{
  QMenu* popupMenu = QMainWindow::createPopupMenu();
  if (!popupMenu) return popupMenu;
  popupMenu->addSeparator();
  QMenu *buttonStyleMenu = new QMenu(tr("Toolbar style"), popupMenu);
  buttonStyleMenu->addActions(toolBarTextsAndIcons->actions());
  popupMenu->addMenu(buttonStyleMenu);
  return popupMenu;
}

// Event handlers

void MainWindow::closeEvent(QCloseEvent *event)
{
  if (maybeSave())
    {
        writeSettings();
      if (assistantClient)
        {
#if QT_VERSION <= 0x040603
          assistantClient->closeAssistant();
#else
          assistantClient->terminate();
#endif
        }
      event->accept();
    }
  else
    {
      event->ignore();
    }
}

void MainWindow::newFile()
{
  if (maybeSave())
    {
      m_scene->clear();
      setCurrentFile("");
      m_molView->resetMatrix();
    }
}

void MainWindow::open()
{
  if (maybeSave())
  {
    QStringList readableFormats;
    readableFormats << MSK_DEFAULT_FORMAT;
    PREPARELOADFILE
    if (inputFormats) readableFormats << inputFormats();
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open - Molsketch"), m_lastAccessedPath,
      readableFormats.join(";;"));
    if (fileName.isEmpty()) return;

    // Save accessed path
    m_lastAccessedPath = QFileInfo(fileName).path();

    // Start a new document
    m_scene->clear();

    Molecule* mol = 0;
    if (fileName.endsWith(".msk"))
    {
      readMskFile(fileName, m_scene);
      return;
    }
    else if (loadFilePtr)
    {
      mol = loadFilePtr(fileName);
    }
    else
    {
      QMessageBox::critical(this, tr(PROGRAM_NAME), tr("Could not open file. OpenBabel support is missing")) ;
    }

    if (mol)
    {
      // Add molecule to scene
      if (mol->canSplit())
      {
        QList<Molecule*> molList = mol->split();
        foreach(Molecule* mol,molList)
          m_scene->addItem(mol);
      }
      else
      {
        m_scene->addItem(mol);
      }

      // Updating view
      setCurrentFile(fileName);
//               setWindowModified(false);
    }
    else
    {
      // Display error message if load fails
      QMessageBox::critical(this,tr(PROGRAM_NAME),tr("Error while loading file"),QMessageBox::Ok,QMessageBox::Ok);
    }
  }
}

bool MainWindow::save()
{
  if (m_curFile.isEmpty())
      return saveAs();
  if (m_curFile.endsWith(".msk"))
  {
      writeMskFile(m_curFile, m_scene);
      m_scene->stack()->setClean() ;
      return true ;
  }
  PREPARESAVEFILE
  if (!saveFilePtr)
  {
    QMessageBox::critical(this, tr(PROGRAM_NAME),
                          tr("Saving failed. OpenBabel support unavailable.")) ;
    return false ;
  }
  bool threeD = QMessageBox::question(this, tr("Save as 3D?"), tr("Save as three dimensional coordinates?")) == QMessageBox::Yes;
  if (!saveFilePtr(m_curFile, m_scene, threeD ? 3 : 2))
    return false ;
  m_scene->stack()->setClean();
  return true ;
}

bool MainWindow::autoSave()
{
  QFileInfo fileName(m_curFile);

  // Do nothing if there is nothing to save
  if(m_scene->stack()->isClean()) return true;

  // Else construct the filename
  if (!fileName.exists())
    fileName = QDir::homePath() + tr("/untitled.backup.msk");
  else
    fileName = QFileInfo(m_curFile).path() + QFileInfo(m_curFile).baseName() +  ".backup." + QFileInfo(m_curFile).completeSuffix();
  // And save the file
  if (fileName.suffix() == "msk")
    writeMskFile(fileName.absoluteFilePath(), m_scene) ;
  else
  {
    PREPARESAVEFILE
    if (!saveFilePtr)
    {
      statusBar()->showMessage(tr("Autosave failed! OpenBabel unavailable."), 10000);
      return false ;
    }
    bool threeD = QMessageBox::question(this, tr("Save as 3D?"), tr("Save as three dimensional coordinates?")) == QMessageBox::Yes; // TODO not in autosave!
    if (!saveFilePtr(fileName.absoluteFilePath(), m_scene, threeD ? 3 : 2))
    {
      statusBar()->showMessage(tr("Autosave failed!"), 10000);
      return false;
    }
  }
  statusBar()->showMessage(tr("Document autosaved"), 10000);
  return true;
}

bool MainWindow::saveAs()
{
  // Get the filename to save under
  QString filter = MSK_DEFAULT_FORMAT;
  PREPARESAVEFILE;
  QStringList supportedFormats;
  supportedFormats << MSK_DEFAULT_FORMAT;
  if (outputFormats) supportedFormats << outputFormats();
  QString fileName = QFileDialog::getSaveFileName(this,
                                                  tr("Save as - Molsketch"),
                                                  m_lastAccessedPath, supportedFormats.join(";;"),
                                                  &filter);
  if (fileName.isEmpty()) return false;

  // Save accessed path
  m_lastAccessedPath = QFileInfo(fileName).path();

  // Finding the right extension
  if (QFileInfo(fileName).suffix().isEmpty())
  {
    int index = filter.indexOf(QRegExp("\\*."));
    filter = filter.remove(0, index + 1);
    index = filter.indexOf(QRegExp("( \\*.)|(\\))"));
    if (index > 0) filter.truncate(index);
    fileName = fileName + filter;
  }
  qDebug() << "Trying to save as " << fileName << "\n";

  if (fileName.endsWith(".msk"))
  {
    writeMskFile(fileName, m_scene);
    setCurrentFile(fileName);
    m_scene->stack()->setClean();
    return true ;
  }
  else
  {
    if (!saveFilePtr)
    {
      QMessageBox::critical(this, tr(PROGRAM_NAME), tr("OpenBabel support not available. Cannot save in this format.")) ;
      return false ;
    }
    bool threeD = QMessageBox::question(this, tr("Save as 3D?"), tr("Save as three dimensional coordinates?")) == QMessageBox::Yes;
    if(saveFilePtr(fileName, m_scene, threeD ? 3: 2))
    {
      setCurrentFile(fileName);
      m_scene->stack()->setClean();
      return true;
    }
  }
  QMessageBox::critical(this,tr(PROGRAM_NAME),tr("Invalid name or unknown file type"),QMessageBox::Ok,QMessageBox::Ok);
  return false;
}



  bool MainWindow::importDoc()
  {
    QLibrary obabeliface("obabeliface" QTVERSIONSUFFIX);
    obabeliface.load() ;
    callOsraFunctionPointer callOsraPtr = (callOsraFunctionPointer) obabeliface.resolve("call_osra") ;
    if (!callOsraPtr)
    {
      QMessageBox::critical(this, tr("Import error"), tr("Could not import: OpenBabel support missing.")) ;
      return false ;
    }
    QString fileName = QFileDialog::getOpenFileName(this, tr("Import - molsKetch"), m_lastAccessedPath, tr(OSRA_GRAPHIC_FILE_FORMATS));

    if (!fileName.isEmpty()) {
      // Save accessed path
      m_lastAccessedPath = QFileInfo(fileName).path();

      m_scene->clear();
      QProgressBar *pb = new QProgressBar(this);
      pb->setMinimum(0);
      pb->setMaximum(0);
      Molecule* mol=callOsraPtr(fileName);
      if (mol) {
        if (mol->canSplit()) {
          QList<Molecule*> molList = mol->split();
          foreach(Molecule* mol,molList)
            m_scene->addItem(mol);
        } else {
          m_scene->addItem(mol);
        }

        setCurrentFile(fileName);
        return true;
      } else {
        QMessageBox::critical(this, tr(PROGRAM_NAME), tr("Error importing file"), QMessageBox::Ok, QMessageBox::Ok);
        return false;
      }

    }

    return false;
  }

bool MainWindow::exportDoc()
{
  // Getting the filename
  QString filter = GRAPHIC_DEFAULT_FORMAT;
  QString fileName = QFileDialog::getSaveFileName(this,tr("Export - Molsketch"), m_lastAccessedPath,tr(GRAPHIC_FILE_FORMATS), &filter);

  // Abort if filename is empty
  if (fileName.isEmpty()) return false;

  // Save accessed path
  m_lastAccessedPath = QFileInfo(fileName).path();

  // Finding the right extension
  if (QFileInfo(fileName).suffix().isEmpty())
  {
    int index = filter.indexOf(QRegExp("\\*."));
    filter = filter.remove(0, index + 1);
    index = filter.indexOf(QRegExp("( \\*.)|(\\))"));
    if (index > 0) filter.truncate(index);
    fileName = fileName + filter;
  }
  qDebug() << "Trying to export as " << fileName << "\n";

  m_lastAccessedPath = QFileInfo(fileName).path();

  // Try to export the file
  if (fileName.endsWith(".svg")) return Molsketch::saveToSVG(fileName, m_scene);

  if (Molsketch::exportFile(fileName,m_scene))
    {
      return true;
    }
  else
    {
      QMessageBox::critical(this,tr(PROGRAM_NAME),tr("Error while exporting file"),QMessageBox::Ok,QMessageBox::Ok);
      return false;
    }
}

void MainWindow::paintSceneOn (QPrinter *printer) {
  Molsketch::printFile(*printer,m_scene);
}

bool MainWindow::print()
{
  QPrintPreviewDialog printPreview;
  connect(&printPreview, SIGNAL(paintRequested(QPrinter * )), this, SLOT(paintSceneOn(QPrinter *)));

  printPreview.exec ();
  return true;
}

void MainWindow::setToolButtonStyle(QAction *styleAction)
{
  if (!styleAction) return;
  QMainWindow::setToolButtonStyle((Qt::ToolButtonStyle) styleAction->data().toInt());
}


void MainWindow::zoomIn()
{
  m_molView->scale(2,2);
}

void MainWindow::zoomOut()
{
  m_molView->scale(0.5,0.5);
}

void MainWindow::zoomReset()
{
  m_molView->resetMatrix();
}

void MainWindow::zoomFit()
{
  m_molView->fitInView(m_scene->itemsBoundingRect(),Qt::KeepAspectRatio);
}

void MainWindow::assistant()
{
  QFileInfo file(MSK_INSTALL_DOCS + QString("/index.html"));
  if (!file.exists()) file.setFile(QApplication::applicationDirPath() + "/doc/en/index.html");
  if (!file.exists()) file.setFile(QApplication::applicationDirPath() + "/../share/doc/molsketch/doc/en/index.html");
#if QT_VERSION <= 0x040603
  assistantClient->showPage(file.absoluteFilePath());
#else
  qDebug() << "Opening help:" << file.absoluteFilePath() ;
  QTextStream stream(assistantClient) ;
  stream << QLatin1String("setSource ")
         << file.absoluteFilePath()
         << QLatin1Char('\0')
         << endl;
#endif
}

QString readFileContent(const QString& absolutePath) {
  QFile file(absolutePath);
  file.open(QFile::ReadOnly);
  return file.readAll();
}

void MainWindow::about()
{
  QString version(readFileContent(":/version")), versionNick(readFileContent(":/versionnick"));
  QMessageBox::about(this, tr("About"),
                     tr("<H3>About Molsketch</H3>"
                        "<H4>Version: ") + version + " -- " + versionNick + tr("</H4>"
                        "<P> Molsketch is a program for drawing molecular structures developed by Harm van Eersel at the "
                        "<A href=\"http://www.tue.nl\">Eindhoven University of Technology</A>."
                        "<P> For more info check <A href=\"http://molsketch.sourceforge.net\">http://molsketch.sourceforge.net</A>"
                        "<P> It is <A href=\"http://www.gnu.org/philosophy/free-sw.html\">free software</A> and available under the "
                        "<A>GPL</A>."
                        "<P> Special thanks to: <UL>"
                        "<LI>Prof. Dr. H. Zantema (coach of the initial version)</LI>"
                        "<LI>Davy van der Vaart (tester)</LI>"
                        "<LI>Frans Visscher (tester)</LI>"
                        "<LI>Carsten Niehaus (reviewer)</LI>"
                        "</UL>Copyright 2007 - 2008, Harm van Eersel"
                        "<P>Copyright 2009 Tim Vandermeersch"
                        "<P>Maintenance since 12/2014: Hendrik Vennekate"));
}

void MainWindow::showReleaseNotes() {
  ReleaseNotesDialog().exec();
}

void MainWindow::documentWasModified()
{
  setWindowModified(!m_scene->stack()->isClean());
}

void MainWindow::updateEditMode(int mode)
{
  Q_UNUSED(mode);
//  m_molView->setDragMode(QGraphicsView::NoDrag);

}

// Widget creators

void MainWindow::createActions()
{
  newAct = new QAction(QIcon::fromTheme("document-new", QIcon(":icons/document-new.svg")), tr("&New"),this);
  newAct->setShortcut(tr("Ctrl+N"));
  newAct->setStatusTip(tr("Create a new file"));
  connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

  openAct = new QAction(QIcon::fromTheme("document-open", QIcon(":icons/document-open.svg")),tr("&Open..."), this);
  openAct->setShortcut(tr("Ctrl+O"));
  openAct->setStatusTip(tr("Open an existing file"));
  connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

  saveAct = new QAction(QIcon::fromTheme("document-save", QIcon(":icons/document-save.svg")), tr("&Save"), this);
  saveAct->setShortcut(tr("Ctrl+S"));
  saveAct->setStatusTip(tr("Save the document to disk"));
  connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

  saveAsAct = new QAction(QIcon::fromTheme("document-save-as", QIcon(":icons/document-save-as.svg")),tr("Save &As..."), this);
  saveAsAct->setStatusTip(tr("Save the document under a new name"));
  connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

  autoSaveAct = new QAction(tr("Autosave document"), this);
  m_autoSaveTimer = new QTimer(this);
//   m_autoSaveTimer->setInterval(m_autoSaveTime);
  connect(autoSaveAct, SIGNAL(triggered()), this, SLOT(autoSave()));
  connect(m_autoSaveTimer, SIGNAL(timeout()), autoSaveAct, SIGNAL(triggered()));
//   m_autoSaveTimer->start();

  importAct = new QAction(QIcon::fromTheme("document-import", QIcon(":icons/document-import.svg")),tr("&Import..."), this);
  importAct->setShortcut(tr("Ctrl+I"));
  importAct->setStatusTip(tr("Insert an existing molecule into the document"));
  connect(importAct, SIGNAL(triggered()), this, SLOT(importDoc()));

  exportAct = new QAction(QIcon::fromTheme("document-export", QIcon(":icons/document-export.svg")),tr("&Export..."), this);
  exportAct->setShortcut(tr("Ctrl+E"));
  exportAct->setStatusTip(tr("Export the current document as a picture"));
  connect(exportAct, SIGNAL(triggered()), this, SLOT(exportDoc()));

  printAct = new QAction(QIcon::fromTheme("document-print", QIcon(":icons/document-print.svg")),tr("&Print..."), this);
  printAct->setShortcut(tr("Ctrl+P"));
  printAct->setStatusTip(tr("Print the current document"));
  connect(printAct, SIGNAL(triggered()), this, SLOT(print()));

  exitAct = new QAction(QIcon::fromTheme("application-exit", QIcon(":icons/application-exit.svg")),tr("E&xit"), this);
  exitAct->setShortcut(tr("Ctrl+Q"));
  exitAct->setStatusTip(tr("Exit the application"));
  connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

  // Edit actions
  undoAct = m_scene->stack()->createUndoAction(this);
  undoAct->setIcon(QIcon::fromTheme("edit-undo", QIcon(":icons/edit-undo.svg")));
  undoAct->setShortcut(tr("Ctrl+Z"));
  undoAct->setStatusTip(tr("Undo the last action"));

  redoAct = m_scene->stack()->createRedoAction(this);
  redoAct->setIcon(QIcon::fromTheme("edit-redo", QIcon(":icons/edit-redo.svg")));
  redoAct->setShortcut(tr("Ctrl+Shift+Z"));
  redoAct->setStatusTip(tr("Redo the last action"));

  cutAct = new QAction(QIcon::fromTheme("edit-cut", QIcon(":icons/edit-cut.svg")), tr("Cu&t"), this);
  cutAct->setShortcut(tr("Ctrl+X"));
  cutAct->setStatusTip(tr("Cut the current selection's contents to the "
                          "clipboard"));
  connect(cutAct, SIGNAL(triggered()), m_scene, SLOT(cut()));

  copyAct = new QAction(QIcon::fromTheme("edit-copy", QIcon(":icons/edit-copy.svg")), tr("&Copy"), this);
  copyAct->setShortcut(tr("Ctrl+C"));
  copyAct->setStatusTip(tr("Copy the current selection's contents to the "
                           "clipboard"));
  connect(copyAct, SIGNAL(triggered()), m_scene, SLOT(copy()));

  pasteAct = new QAction(QIcon::fromTheme("edit-paste", QIcon(":icons/edit-paste.svg")), tr("&Paste"), this);
  pasteAct->setShortcut(tr("Ctrl+V"));
  pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                            "selection"));
  connect(pasteAct, SIGNAL(triggered()), m_scene, SLOT(paste()));

  convertImageAct = new QAction(QIcon(""), tr("C&onvert Image to Mol"),this);
  convertImageAct->setShortcut(tr("Ctrl+M"));
  convertImageAct->setStatusTip(tr("Convert Image to Mol using OSRA"));
  connect(convertImageAct, SIGNAL(triggered()), m_scene, SLOT(convertImage()));

  selectAllAct = new QAction(QIcon::fromTheme("edit-select-all", QIcon(":icons/edit-select-all.svg")), tr("&Select all"),this);
  selectAllAct->setShortcut(tr("Ctrl+A"));
  selectAllAct->setStatusTip(tr("Selects all elements on the scene"));
  connect(selectAllAct, SIGNAL(triggered()), m_scene, SLOT(selectAll()));

  alignAct = new QAction(QIcon(""), tr("Show grid"), this);
  alignAct->setStatusTip(tr("Shows grid and snaps to it while drawing"));
  alignAct->setCheckable(true);
  connect(alignAct, SIGNAL(toggled(bool)), m_scene, SLOT(setGrid(bool)));

  prefAct = new QAction(QIcon::fromTheme("preferences-system", QIcon(":icons/preferences-system.svg")),tr("Edit Pre&ferences..."),this);
  prefAct->setShortcut(tr("Ctrl+F"));
  prefAct->setStatusTip(tr("Edit your preferences"));
  connect(prefAct, SIGNAL(triggered()), this, SLOT(editPreferences()));

  // Zoom actions
  zoomInAct = new QAction(QIcon::fromTheme("zoom-in", QIcon(":icons/zoom-in.svg")),tr("Zoom &In"), this);
  zoomInAct->setShortcut(tr("Ctrl++"));
  zoomInAct->setStatusTip(tr("Zoom in on the canvas"));
  connect(zoomInAct, SIGNAL(triggered()), this, SLOT(zoomIn()));

  zoomOutAct = new QAction(QIcon::fromTheme("zoom-out", QIcon(":icons/zoom-out.svg")),tr("Zoom &Out"), this);
  zoomOutAct->setShortcut(tr("Ctrl+-"));
  zoomOutAct->setStatusTip(tr("Zoom out on the canvas"));
  connect(zoomOutAct, SIGNAL(triggered()), this, SLOT(zoomOut()));

  zoomResetAct = new QAction(QIcon::fromTheme("zoom-original", QIcon(":icons/zoom-original.svg")),tr("Zoom &Reset"), this);
  zoomResetAct->setShortcut(tr("Ctrl+="));
  zoomResetAct->setStatusTip(tr("Reset the zoom level"));
  connect(zoomResetAct, SIGNAL(triggered()), this, SLOT(zoomReset()));

  zoomFitAct = new QAction(QIcon::fromTheme("zoom-fit-best", QIcon(":icons/zoom-fit-best.svg")),tr("Zoom &Fit"), this);
  zoomFitAct->setShortcut(tr("Ctrl+*"));
  zoomFitAct->setStatusTip(tr("Fit to screen"));
  connect(zoomFitAct, SIGNAL(triggered()), this, SLOT(zoomFit()));

  // Help actions
  helpContentsAct = new QAction(QIcon::fromTheme("help-contents", QIcon(":icons/help-contents.svg")),tr("&Help Contents..."), this);
  helpContentsAct->setShortcut(tr("F1"));
  helpContentsAct->setStatusTip(tr("Show the application's help contents"));
  connect(helpContentsAct, SIGNAL(triggered()), this, SLOT(assistant()));

  submitBugAct = new QAction(QIcon(""),tr("Submit &Bug..."), this);
  submitBugAct->setStatusTip(tr("Open the browser with the bug tracker"));
  connect(submitBugAct, SIGNAL(triggered()), this, SLOT(submitBug()));

  youtubeChannelAction = new QAction(tr("YouTube channel..."), this);
  youtubeChannelAction->setStatusTip(tr("Open the browser with the YouTube channel page"));
  connect(youtubeChannelAction, SIGNAL(triggered()), this, SLOT(goToYouTube()));

  aboutAct = new QAction(QIcon::fromTheme("help-about", QIcon(":icons/help-about.svg")),tr("&About..."), this);
  aboutAct->setStatusTip(tr("Show the application's About box"));
  connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

  releaseNotesAct = new QAction(tr("Show release notes..."), this);
  releaseNotesAct->setStatusTip(tr("Show the entire release notes list for all versions of Molsketch"));
  connect(releaseNotesAct, SIGNAL(triggered()), this, SLOT(showReleaseNotes()));

  aboutQtAct = new QAction(tr("About &Qt..."), this);
  aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
  connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

  // Setting actions in their initial states
  cutAct->setEnabled(false);
  copyAct->setEnabled(false);
  pasteAct->setEnabled(false);
  connect(m_scene, SIGNAL(copyAvailable(bool)), cutAct, SLOT(setEnabled(bool)));
  connect(m_scene, SIGNAL(copyAvailable(bool)), copyAct, SLOT(setEnabled(bool)));
  connect(m_scene, SIGNAL(pasteAvailable(bool)), pasteAct, SLOT(setEnabled(bool)));
}


void MainWindow::createMenus()
{
  fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(newAct);
  fileMenu->addAction(openAct);
  fileMenu->addAction(saveAct);
  fileMenu->addAction(saveAsAct);
  fileMenu->addSeparator();
  fileMenu->addAction(importAct);
  fileMenu->addAction(exportAct);
  fileMenu->addAction(printAct);
  fileMenu->addSeparator();
  fileMenu->addAction(exitAct);

  editMenu = menuBar()->addMenu(tr("&Edit"));
  editMenu->addAction(undoAct);
  editMenu->addAction(redoAct);
  editMenu->addSeparator();
  editMenu->addAction(cutAct);
  editMenu->addAction(copyAct);
  editMenu->addAction(pasteAct);
  editMenu->addAction(convertImageAct);
  editMenu->addSeparator();
  editMenu->addAction(selectAllAct);
  editMenu->addAction(alignAct);
  editMenu->addSeparator();
  editMenu->addSeparator();
  editMenu->addAction(prefAct);

  viewMenu = menuBar()->addMenu(tr("&View"));
  viewMenu->addAction(zoomInAct);
  viewMenu->addAction(zoomOutAct);
  viewMenu->addAction(zoomResetAct);
  viewMenu->addAction(zoomFitAct);

  menuBar()->addSeparator();

  helpMenu = menuBar()->addMenu(tr("&Help"));
  helpMenu->addAction(helpContentsAct);
  helpMenu->addSeparator();
  helpMenu->addAction(submitBugAct);
  helpMenu->addAction(youtubeChannelAction);
  helpMenu->addSeparator();
  helpMenu->addAction(aboutAct);
  helpMenu->addAction(releaseNotesAct);
  helpMenu->addAction(aboutQtAct);
}

void setAllToolBarChildren(QObject* parent,
                           QSize size)
{
  foreach (QToolBar* bar, parent->findChildren<QToolBar*>())
    bar->setIconSize(size);
}

void MainWindow::createToolBars()
{
  fileToolBar = addToolBar(tr("File"));
  fileToolBar->setObjectName("file-toolbar");
  fileToolBar->addAction(newAct);
  fileToolBar->addAction(openAct);
  fileToolBar->addAction(saveAct);
  fileToolBar->addAction(importAct);
  fileToolBar->addAction(exportAct);
  fileToolBar->addAction(printAct);

  editToolBar = addToolBar(tr("Edit"));
  editToolBar->setObjectName("edit-toolbar");
  editToolBar->addAction(undoAct);
  editToolBar->addAction(redoAct);
  editToolBar->addSeparator();
  editToolBar->addAction(cutAct);
  editToolBar->addAction(copyAct);
  editToolBar->addAction(pasteAct);


  zoomToolBar = addToolBar(tr("Zoom"));
  zoomToolBar->setObjectName("zoom-toolbar");
  zoomToolBar->addAction(zoomInAct);
  zoomToolBar->addAction(zoomOutAct);
  zoomToolBar->addAction(zoomResetAct);
  zoomToolBar->addAction(zoomFitAct);

#ifdef QT_DEBUG
  QAction *debugAction = new QAction("debug scene", this);
  connect(debugAction, SIGNAL(triggered()), m_scene, SLOT(debugScene()));
  zoomToolBar->addAction(debugAction);
#endif

  drawToolBar = addToolBar(tr("Drawing"));
  drawToolBar->setObjectName("drawing-toolbar");
  drawToolBar->addAction(new drawAction(m_scene));
  drawToolBar->addAction(new ringAction(m_scene));
  drawToolBar->addAction(new reactionArrowAction(m_scene));
  drawToolBar->addAction(new mechanismArrowAction(m_scene));
  drawToolBar->addAction(new FrameAction(m_scene));
  drawToolBar->addAction(new TextAction(m_scene));

  modifyToolBar = addToolBar(tr("Modify"));
  modifyToolBar->setObjectName("modify-toolbar");
  modifyToolBar->addAction(new rotateAction(m_scene));
  modifyToolBar->addAction(new colorAction(m_scene));
  modifyToolBar->addAction(new lineWidthAction(m_scene));
  modifyToolBar->addAction(new chargeAction(m_scene));
  modifyToolBar->addAction(new hydrogenAction(m_scene));
  modifyToolBar->addAction(new connectAction(m_scene));
  modifyToolBar->addAction(new deleteAction(m_scene));
  modifyToolBar->addAction(new flipBondAction(m_scene));
  modifyToolBar->addAction(new flipStereoBondsAction(m_scene));

  new arrowTypeAction(m_scene);
  new bondTypeAction(m_scene);
  new flipBondAction(m_scene);
  new FrameTypeAction(m_scene);

  setAllToolBarChildren(this,
#ifdef __ANDROID__
                        QSize(48,48)
#else
                        QSize(22,22)
#endif
                        );
}

void MainWindow::createStatusBar()
{
  statusBar()->showMessage(tr("Ready"));
#ifdef __ANDROID__
  QFont statusFont(statusBar()->font());
  statusFont.setPixelSize(12);
  statusBar()->setFont(statusFont);
#endif
}

void MainWindow::buildLibraries()
{
  foreach(LibraryListWidget* library, toolBox->findChildren<LibraryListWidget*>())
    delete library;

  foreach(const QString& folder, QSettings().value("libraries").toStringList())
  {
    LibraryListWidget* library = new LibraryListWidget(folder, toolBox);
    toolBox->addItem(library, library->title());
    connect(refreshLibraries, SIGNAL(clicked()), library, SLOT(refreshItems()));
  }
}

void MainWindow::createToolBox()
{
  // Creating the dockwidgets
  toolBoxDock = new QDockWidget(tr("Molecule libraries"), this);
  toolBoxDock->setObjectName("toolbox-dockwidget");
  toolBoxDock->setMinimumWidth(270);
  // Create a library toolbox and add the libraries
  QWidget* toolBoxDockContent = new QWidget;
  QLayout* layout = new QVBoxLayout(toolBoxDockContent);
  toolBoxDockContent->setLayout(layout);
  toolBox = new QToolBox;
  layout->addWidget(new HelpForEmptyToolBox(tr("Define libraries using <b>Edit > Edit Preferences... > Libraries</b>."), toolBox, toolBoxDockContent));
  layout->addWidget(refreshLibraries = new QPushButton(QIcon::fromTheme("view-refresh", QIcon(":icons/view-refresh.svg")),""));
  layout->addWidget(toolBox);
  layout->setMargin(0);
  toolBoxDock->setWidget(toolBoxDockContent);
  // Placing the dockwidgets in their default position
  setCorner(Qt::TopLeftCorner, Qt::LeftDockWidgetArea);
  setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
  addDockWidget(Qt::LeftDockWidgetArea,toolBoxDock);
#ifdef __ANDROID__
  toolBoxDock->hide();
#endif
  buildLibraries();
}

void MainWindow::createWikiDock() {
  wikidataDock = new WikiQueryWidget(this);
  wikidataDock->setObjectName("wikidata-query-widget");
  addDockWidget(Qt::LeftDockWidgetArea, wikidataDock);
}

void MainWindow::createToolBarContextMenuOptions()
{
  toolBarTextsAndIcons = new QActionGroup(this);
  toolBarTextsAndIcons->addAction(tr("Icons"))->setData(Qt::ToolButtonIconOnly);
  toolBarTextsAndIcons->addAction(tr("Texts"))->setData(Qt::ToolButtonTextOnly);
  toolBarTextsAndIcons->addAction(tr("Texts under icons"))->setData(Qt::ToolButtonTextUnderIcon);
  toolBarTextsAndIcons->addAction(tr("Texts besides icons"))->setData(Qt::ToolButtonTextBesideIcon);
  toolBarTextsAndIcons->addAction(tr("System default"))->setData(Qt::ToolButtonFollowStyle);
  QMainWindow::setToolButtonStyle((Qt::ToolButtonStyle) QSettings().value("toolBarIconStyle").toInt());
  for(QAction* action : toolBarTextsAndIcons->actions())
  {
    action->setCheckable(true);
    if (toolButtonStyle() == action->data())
      action->setChecked(true);
  }
  connect(toolBarTextsAndIcons, SIGNAL(triggered(QAction*)), this, SLOT(setToolButtonStyle(QAction*)));
}

void MainWindow::createView()
{
  m_scene = new MolScene(this);
  m_molView = new MolView(m_scene);
  setCentralWidget(m_molView);
}

void MainWindow::initializeAssistant()
{
#if QT_VERSION <= 0x040603
  assistantClient = new QAssistantClient("", this);
  QString docfile("molsketch.adp") ;
  QStringList arguments;
#else
  assistantClient = new QProcess(this) ;
  QString app = QLibraryInfo::location(QLibraryInfo::BinariesPath)
               + QLatin1String("/assistant");
#if QT_VERSION >= 0x050000
  app += QLatin1String("-qt5") ;
#endif
  QString docfile("molsketch.qhp") ;
#endif

  QFileInfo file(MSK_INSTALL_DOCS + QString("/molsketch.adp"));
  if (!file.exists()) file.setFile(QApplication::applicationDirPath() + "/doc/en/" + docfile );
  if (!file.exists()) file.setFile(QApplication::applicationDirPath() + "/../share/doc/molsketch/doc/en/" + docfile);

#if QT_VERSION <= 0x040603
  arguments << "-profile" << file.absoluteFilePath();
  assistantClient->setArguments(arguments);
#else
  qDebug() << "Starting assistant with arguments:" << file.absoluteFilePath() << app ;
//  assistantClient->start(app, QStringList() << QLatin1String("-enableRemoteControl")) ;
  QTextStream stream(assistantClient) ;
  stream << QLatin1String("register ")
         << file.absoluteFilePath()
         << QLatin1Char('\0')
         << endl;
#endif
}

// Auxillary methods

void MainWindow::readSettings()
{
  // Reading the settings
  QSettings settings;

  // Setting the window position
  QPoint pos = settings.value("pos",QPoint(100,100)).toPoint();
  QSize size = settings.value("size",QSize(800,600)).toSize();
  resize(size);
  move(pos);

  QByteArray state = settings.value("window-state", QByteArray("@ByteArray(\0\0\0\xff\0\0\0\0\xfd\0\0\0\x1\0\0\0\0\0\0\x1\xe\0\0\x3N\xfc\x2\0\0\0\x2\xfb\0\0\0$\0t\0o\0o\0l\0\x62\0o\0x\0-\0\x64\0o\0\x63\0k\0w\0i\0\x64\0g\0\x65\0t\x1\0\0\0x\0\0\x2\x65\0\0\0\xe8\0\xff\xff\xff\xfb\0\0\0$\0i\0n\0\x66\0o\0\x62\0o\0x\0-\0\x64\0o\0\x63\0k\0w\0i\0\x64\0g\0\x65\0t\x1\0\0\x2\xe3\0\0\0\xe3\0\0\0l\0\xff\xff\xff\0\0\x6l\0\0\x3N\0\0\0\x1\0\0\0\x4\0\0\0\x1\0\0\0\b\xfc\0\0\0\x2\0\0\0\x2\0\0\0\x3\0\0\0\x18\0\x66\0i\0l\0\x65\0-\0t\0o\0o\0l\0\x62\0\x61\0r\x1\0\0\0\0\xff\xff\xff\xff\0\0\0\0\0\0\0\0\0\0\0\x18\0\x65\0\x64\0i\0t\0-\0t\0o\0o\0l\0\x62\0\x61\0r\x1\0\0\x1\x33\xff\xff\xff\xff\0\0\0\0\0\0\0\0\0\0\0\x18\0z\0o\0o\0m\0-\0t\0o\0o\0l\0\x62\0\x61\0r\x1\0\0\x2s\xff\xff\xff\xff\0\0\0\0\0\0\0\0\0\0\0\x2\0\0\0\x4\0\0\0\b\0\x44\0r\0\x61\0w\x1\0\0\0\0\xff\xff\xff\xff\0\0\0\0\0\0\0\0\0\0\0\n\0R\0i\0n\0g\0s\x1\0\0\x2G\xff\xff\xff\xff\0\0\0\0\0\0\0\0\0\0\0\n\0T\0o\0o\0l\0s\x1\0\0\x3t\xff\xff\xff\xff\0\0\0\0\0\0\0\0\0\0\0\x10\0R\0\x65\0\x61\0\x63\0t\0i\0o\0n\x1\0\0\x4\x80\xff\xff\xff\xff\0\0\0\0\0\0\0\0)")).toByteArray();


  restoreState(state);

  // Load preferences
  readPreferences();
}

void MainWindow::readPreferences()
{
  QSettings settings;
  // Loading auto-save time
  m_autoSaveTime = settings.value("auto-save-time", 300000).toInt();
  m_autoSaveTimer->setInterval(m_autoSaveTime);
  m_autoSaveTimer->start();

  // Loading paths
  m_lastAccessedPath = settings.value("last-save-path", QDir::homePath()).toString();

  // TODO fix this (protected in Qt4)
#if QT_VERSION >= 0x050000
  foreach(QAction *action, m_scene->sceneActions())
    emit action->changed();
#endif

  // TODO add scene default properties

  // Update the scene contents
  m_scene->update();
  buildLibraries();
}

void MainWindow::writeSettings()
{
  // Saving the settings
  QSettings settings;

  // Saving the window position
  settings.setValue("pos",pos());
  settings.setValue("size",size());
  settings.setValue("toolBarIconStyle", toolButtonStyle());

  // Saving the state of the toolbars and dockwidgets
  settings.setValue("window-state", saveState());

  // TODO add scene default properties

  // Saving paths
  settings.setValue("last-save-path", m_lastAccessedPath);

}


bool MainWindow::maybeSave()
{
  /* TODO */
  if (isWindowModified())
    {
      QMessageBox::StandardButton ret;
      ret = QMessageBox::warning(this,tr(PROGRAM_NAME),
                                 tr("This document has been modified.\n"
                                    "Do you want to save your changes?"),
                                 QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
      if (ret == QMessageBox::Save)
        return save();
      else if (ret == QMessageBox::Cancel)
        return false;
    }
  return true;
}

void MainWindow::setCurrentFile(const QString &fileName)
{
  // Synthesizing the correct name
  m_curFile = fileName;
  QString shownName;
  if (m_curFile.isEmpty())
    shownName = tr("untitled.mol");
  else
    shownName = QFileInfo(m_curFile).fileName();

  // Setting the windowtitle
  setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(tr(PROGRAM_NAME)));
}

void MainWindow::editPreferences( )
{
  // Opens the settings dialog
  SettingsDialog dialog;
  connect(&dialog, SIGNAL(settingsChanged()), this, SLOT(readPreferences()));
  dialog.exec();
}

void MainWindow::submitBug()
{
  // Opens a browser with the bug tracker
  QDesktopServices::openUrl(QUrl("http://sourceforge.net/tracker/?func=add&group_id=191562&atid=937880"));
}

void MainWindow::goToYouTube()
{
  QDesktopServices::openUrl(QUrl("https://www.youtube.com/channel/UCIYnNzSnI9AHpB_c48BR7fQ"));
}

