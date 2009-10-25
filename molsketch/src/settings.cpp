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

#include <QDialog>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QFileDialog>
#include <QFontDialog>
#include <QSettings>

#include "settings.h"

#include <molsketch/molscene.h>

SettingsDialog::SettingsDialog(QWidget * parent, Qt::WindowFlags f ) : QDialog(parent,f)
{
  // Setup the user interface
  ui.setupUi(this);

  foreach(QListWidgetItem * item, ui.listWidget->findItems("*",Qt::MatchWildcard))
    item->setTextAlignment(Qt::AlignHCenter);
  ui.listWidget->setCurrentRow(0);

  // Connect signals and slots
  connect(ui.toolButtonBrowseLibraryPath, SIGNAL(clicked()), this, SLOT(browseLibraryPath()));
  connect(ui.toolButtonBrowseCustomLibraryPath, SIGNAL(clicked()), this, SLOT(browseCustomLibraryPath()));
  connect(ui.pushButtonFont, SIGNAL(clicked()), this, SLOT(selectFont()));
  connect(ui.buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(buttonClicked(QAbstractButton*)));
  connect(ui.buttonBox, SIGNAL(helpRequested()), this, SLOT(showHelp()));

  // Setting translations
  ui.groupBoxGeneral->setTitle(tr("Save options"));
  ui.labelAutoSave->setText(tr("Autosave every"));
  ui.labelDefaultFileType->setText(tr("Default file type"));
  ui.labelDefaultImageType->setText(tr("Default image type"));

  ui.groupBoxAtom->setTitle(tr("Atom settings"));
  ui.checkBoxShowCarbon->setText(tr("Show neutral carbon atoms"));
  ui.checkBoxShowValency->setText(tr("Show atom charge"));
  ui.labelAtomSymbolFont->setText(tr("Atom symbol font"));

  ui.groupBoxBond->setTitle(tr("Bond settings"));
//   ui.labelMsKAtomSize->setText(tr("Atom size: "));
  ui.labelBondLength->setText(tr("Bond length: "));
  ui.labelBondWidth->setText(tr("Bond width: "));
  ui.labelBondAngle->setText(tr("Bond angle: "));

  ui.groupBoxLibraries->setTitle(tr("Libraries"));
  ui.labelLibraryPath->setText(tr("Library path"));
  ui.labelCustomLibraryPath->setText(tr("Custom library path"));

  // Setting initial values
  setInitialValues();
}

SettingsDialog::~ SettingsDialog( )
{

}

void SettingsDialog::setInitialValues()
{
  QSettings settings;
  ui.spinBoxAutoSave->setValue(settings.value("auto-save-time", 300000).toInt()/60000);


  if (settings.value("carbon-visible", false).toBool())
    ui.checkBoxShowCarbon->setCheckState(Qt::Checked);
  if (settings.value("charge-visible", true).toBool())
    ui.checkBoxShowValency->setCheckState(Qt::Checked);
  if (settings.value("electronSystems-visible", true).toBool())
    ui.checkBoxShowES->setCheckState(Qt::Checked);


  ui.doubleSpinBoxFontSize->setValue((settings.value("atom-symbol-font").value<QFont>()).pointSizeF());
  ui.fontComboBox->setCurrentFont(settings.value("atom-symbol-font").value<QFont>());

  ui.lineEditBondLength->setText(QString::number(settings.value("bond-length", 40).toDouble()));
  ui.doubleSpinBoxBondWidth->setValue(settings.value("bond-width", 1).toDouble());
  ui.spinBoxBondAngle->setValue(settings.value("bond-angle", 30).toInt());

  ui.lineEditLibraryPath->setText(settings.value("library-path", "/usr/share/molsketch/library/").toString());
  ui.lineEditCustomLibraryPath->setText(settings.value("custom-library-path", QDir::homePath()).toString());
}

void SettingsDialog::accept()
{
  // Applying changes
  applyChanges();

  // Close dialog
  QDialog::accept();
}

void SettingsDialog::applyChanges()
{
  QSettings settings;
  // General settings
  settings.setValue("auto-save-time", ui.spinBoxAutoSave->value()*60000);

  // Atom settings
  settings.setValue("carbon-visible", ui.checkBoxShowCarbon->isChecked());
  settings.setValue("charge-visible", ui.checkBoxShowValency->isChecked());
  settings.setValue("electronSystems-visible", ui.checkBoxShowES->isChecked());
  QFont font = ui.fontComboBox->currentFont();
  font.setPointSizeF(ui.doubleSpinBoxFontSize->value());
  settings.setValue("atom-symbol-font", font);

  // MsKBond settings
  settings.setValue("bond-length", ui.lineEditBondLength->text().toDouble());
  settings.setValue("bond-width", ui.doubleSpinBoxBondWidth->value());
  settings.setValue("bond-angle", ui.spinBoxBondAngle->value());

  // Library settings
  /* TODO check if the paths are valid */
  settings.setValue("library-path", ui.lineEditLibraryPath->text());
  settings.setValue("custom-library-path", ui.lineEditCustomLibraryPath->text());
}

void SettingsDialog::browseLibraryPath()
{
  QString path = QFileDialog::getExistingDirectory(this, "molsKetch - Select library path", ui.lineEditLibraryPath->text());
  if (!path.isEmpty())
    ui.lineEditLibraryPath->setText(path);
}

void SettingsDialog::browseCustomLibraryPath()
{
  QString path = QFileDialog::getExistingDirectory(this, "molsKetch - Select custom library path", ui.lineEditCustomLibraryPath->text());
  if (!path.isEmpty())
    ui.lineEditCustomLibraryPath->setText(path);
}

void SettingsDialog::selectFont()
{
  bool * ok = false;
  QFont previousFont = ui.fontComboBox->currentFont();
  previousFont.setPointSizeF(ui.doubleSpinBoxFontSize->value());
  QFont font = QFontDialog::getFont(ok, previousFont, this, "molsKetch - Select atomsymbol font");
  if (ok) {
    ui.fontComboBox->setCurrentFont(font);
    ui.doubleSpinBoxFontSize->setValue(font.pointSizeF());
  }
}

void SettingsDialog::buttonClicked(QAbstractButton * button)
{
  switch (ui.buttonBox->standardButton(button)) {
    case QDialogButtonBox::RestoreDefaults:
      /* TODO make this really the default values instead of the initial values */
      setInitialValues();
      break;
    case QDialogButtonBox::Apply:
      applyChanges();
      break;
    default:
      break;
  }
}

void SettingsDialog::showHelp()
{
  /* TODO this should show the right help page */
}
