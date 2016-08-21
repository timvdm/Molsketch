/***************************************************************************
 *   Copyright (C) 2007-2008 by Harm van Eersel                            *
 *   Copyright (C) 2009 by Tim Vandermeersch                               *
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
 * This file is part of molsKetch and contains the SettingsDialog class.
 *
 * @author Harm van Eersel
 * @since Hydrogen
 */

#ifndef SETTINGS_H
#define SETTINGS_H

#include "ui_settings.h"

#include <QDialog>

class QSettings;

/**
 * The settings dialog. Contains the logic of the ui file.
 *
 * @author Harm van Eersel
 */
class SettingsDialog : public QDialog
  {
  Q_OBJECT

  public:
    /** Creates a new dialog to set preferences.
     *
     * @param scene the scene of which the settings should be edited
     */
    SettingsDialog(QWidget * parent = 0, Qt::WindowFlags f = 0);
    /** Destructor of the dialog. */
    ~SettingsDialog();

  private:
    /** Stores the scene of which the settings should be edited. */
    QSettings* m_settings;
    /** The user interface of the dialog. */
    Ui::SettingsDialog ui;

  private slots:
    /** Reimplementation of the accept() slot to change the scene settings on exit. */
    void accept();
    /** Applies the settings, but does not close the dialog */
    void applyChanges();
    /** (Re)set all controls to their initial values */
    void setInitialValues();
    /** Slot to handle clicks on the buttons of the buttonbox */
    void buttonClicked(QAbstractButton * button);
    /** Slot to show the appropiate help page */
    void showHelp();
    /** Shows dialog to select the font of the atom symbols */
    void selectFont();
    void on_addLibrary_clicked();
    void on_removeLibrary_clicked();
};

#endif
