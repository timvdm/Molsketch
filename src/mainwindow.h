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

/** @file
 * This file is part of molsKetch and defines the mainwindow of molsKetch.
 *
 * @author Harm van Eersel <devsciurus@xs4all.nl>
 * @since Hydrogen
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QAction;
class QMenu;
class QToolBox;
class QDockWidget;
class QTextEdit;
class QUndoStack;
class QListWidget;
class QListWidgetItem;
class QTableWidget;
class QTableWidgetItem;
class QAssistantClient;
class QSettings;
class QTimer;

namespace Molsketch {
  class Molecule;
  class MolScene;
  class MolView;
}




/**
 * The main application window of molsKetch.
 *
 * @author Harm van Eersel
 */
class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  /** Constructor */
  MainWindow();

protected:
  /** Reimplements the close event to asked for a save on exit. */
  void closeEvent(QCloseEvent *event);

private slots:
  /** Opens a new empty file. */
  void newFile();
  /** Opens a Open file dialog. */
  void open();
  /** Saves the current document. */
  bool save();
  /** Saves the current document under a new name. */
  bool saveAs();
  /** Saves a backup of the current document. */
  bool autoSave();
  /** Import a file in the current document. */
  bool importDoc();
  /** Export the current document as a picture. */
  bool exportDoc();
  /** Prints the current document. */
  bool print();




  /** Open the preferences editor. */
  void editPreferences();

  /** Open the help window. */
  void assistant();
  /** Open a browser with the bugtracker. */
  void submitBug();
  /** Open the molsKetch about dialog. */
  void about();

  /** Add the selected molecule to the custom library. */
  void addCustomMol();
  /** Removes the selected item from the custom library. */
  void delCustomMol();
  
  /** Mark the current document as modified. */
  void documentWasModified( );


  /** Zoom in on the current view. */
  void zoomIn();
  /** Zoom out on the current view. */
  void zoomOut();
  /** Restore the original position and zoom level. */
  void zoomReset();
  /** Adjust the zoom leven to fit the whole document in the view. */
  void zoomFit();

  /** Update the window to match the edit mode @p mode. */
  void updateEditMode(int mode);
  /** Update the list of recent elements with @p element. */
  void updateRecentList(QListWidgetItem* element);
  /** Update the list of recent elements with @p element. */
  void updateRecentList(QTableWidgetItem* element);
  /** Updated the info box with info on the selected item. */
  void updateInfoBox( );

//  /** 
//   * Adds a new molecule with one atom to the scene.
//   *
//   * @param position position of the new molecule
//   * @param element the element symbol of the new atom
//   */
//   Molecule* newMolecule(const QPointF& position = QPointF(0,0), const QString& element = "C");

private:
  /** Creates the QActions of the MainWindow. */
  void createActions();
  /** Creates the menus of the MainWindow. */
  void createMenus();
  /** Creates the toolbars of the MainWindow. */
  void createToolBars();
  /** Creates the status bar of the MainWindow. */
  void createStatusBar();
  /** Creates the view of the MainWindow. */
  void createView();
  /** Creates the toolboxes of the MainWindow. */
  void createToolBoxes();

  /** Stores the settings mol the application */
  QSettings * m_settings;
  /** Loads the previous settings of molsKetch. */
  void readSettings();
  /** Saves the current settings of molsKetch. */
  void writeSettings();
  /** Reloads the preferences */
  void readPreferences(const QSettings & settings);

  /** Ask if the document should be saved. */
  bool maybeSave();
  /** Clears the scene. */
  void clearScene();

  /** Set the current file name to @p fileName. */
  void setCurrentFile(const QString &fileName);
  /** Return the stripped file name of @p fullFileName. */
  QString strippedName(const QString &fullFileName);
  /** Saves the current document as OpenBabel file under the name @p fileName. */
  bool saveToOB(const QString &fileName);

  // Global settings
  /** The path of the molecule library */
  QString m_libPath;
  /** Path where the last document was saved */
  QString m_lastAccessedPath;
  /** The time between auto-saves in miliseconds */
  int m_autoSaveTime;

  // Timers
  /** The timer for the auto-save action */
  QTimer * m_autoSaveTimer;

  // Documentation classes
  /** The help client */
  QAssistantClient* assistantClient;
  /** Initialize the help client. */
  void initializeAssistant();

  // Libraries
  /** The library widget of elements. */
  QListWidget* elementLib;
  /** The library widget with user-added molecules. */
  QListWidget* customLib;
  /** The library widget with common molecules. */
  QListWidget* genericLib;

  // Widgets
  /** The scene that contains the document's molecules. */
  Molsketch::MolScene* m_scene;
  /** The view of the MolScene. */
  Molsketch::MolView* m_molView;
  /** The file name of the current document. */
  QString m_curFile;

  /** The dock widget for the toolbox. */
  QDockWidget* toolBoxDock;
  /** The dock widget for the info box. */
  QDockWidget* infoDock;
  /** The toolbox with the libraries. */
  QToolBox* toolBox;
  /** The info textbox. */
  QTextEdit* infoText;

  // Menus
  /** The file menu. */
  QMenu* fileMenu;
  /** The edit menu. */
  QMenu* editMenu;
  /** The view menu. */
  QMenu* viewMenu;
  /** The window menu. */
  QMenu* windowMenu;
  /** The help menu. */
  QMenu* helpMenu;

  // Toolbars
  /** The file toolbar. */
  QToolBar* fileToolBar;
  /** The edit toolbar. */
  QToolBar* editToolBar;
  /** The zoom toolbar. */
  QToolBar* zoomToolBar;

  // File actions
  /** Open a new empty file action. */
  QAction* newAct;
  /** Open an existing file action. */
  QAction* openAct;
  /** Save the current document action. */
  QAction* saveAct;
  /** Save the current document under a new name action. */
  QAction* saveAsAct;
  /** Save as 3D */
  QAction* saveAs3DAct;
  /** Save a backup of the current document. */
  QAction* autoSaveAct;
  /** Import an existing file action. */
  QAction* importAct;
  /** Export the current document as a picture action. */
  QAction* exportAct;
  /** Print the current document action. */
  QAction* printAct;
  /** Exit molsKetch action. */
  QAction* exitAct;

  // Edit actions
  /** Undo the last command action. */
  QAction* undoAct;
  /** Redo the last command action. */
  QAction* redoAct;
  /** Cut the selected item action. */
  QAction* cutAct;
  /** Copy the selected item action. */
  QAction* copyAct;
  /** Paste the contents of the clipboard action. */
  QAction* pasteAct;
  /** Select all items on the scene action. */
  QAction* selectAllAct;
  /** Align all items to the grid action. */
  QAction* alignAct;
  /** Open the settings dialog action. */
  QAction* prefAct;
	/** Minimiser **/
	QAction *minimiseModeAct;

  // View actions
  /** Zoom in on the view action. */
  QAction* zoomInAct;
  /** Zoom out on the view action. */
  QAction* zoomOutAct;
  /** Reset the position and zoom leven of the view action. */
  QAction* zoomResetAct;
  /** Adjust the zoom level to fit all items in the view action. */
  QAction* zoomFitAct;

  // Help actions
  /** Show the help contents action. */
  QAction* helpContentsAct;
  /** Submit a bug action. */
  QAction* submitBugAct;
  /** Show the molsKetch about dialog action. */
  QAction* aboutAct;
  /** Show the Qt about dialog action. */
  QAction* aboutQtAct;

};

#endif
