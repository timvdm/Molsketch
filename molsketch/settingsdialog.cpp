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
#include <molscene.h>
#include <scenesettings.h>

#include "settingsdialog.h"

SettingsDialog::SettingsDialog(ApplicationSettings *settings, QWidget * parent, Qt::WindowFlags f )
  : QDialog(parent,f),
    settings(settings)
{
  // Setup the user interface
  ui.setupUi(this);

  foreach(QListWidgetItem * item, ui.listWidget->findItems("*",Qt::MatchWildcard))
    item->setTextAlignment(Qt::AlignHCenter);
  ui.listWidget->setCurrentRow(0);

  // Connect signals and slots
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

  // Setting initial values
  setInitialValues();
}

SettingsDialog::~SettingsDialog( ){}

void SettingsDialog::setInitialValues()
{
  ui.spinBoxAutoSave->setValue(settings->autoSaveInterval()/60000);


  if (settings->isCarbonVisible())
    ui.checkBoxShowCarbon->setCheckState(Qt::Checked);
  if (settings->isChargeVisible())
    ui.checkBoxShowValency->setCheckState(Qt::Checked);
  if (settings->isElectronSystemsVisible())
    ui.checkBoxShowES->setCheckState(Qt::Checked);


  QFont atomFont(settings->getAtomFont());
  ui.doubleSpinBoxFontSize->setValue(atomFont.pointSizeF());
  ui.fontComboBox->setCurrentFont(atomFont);

  ui.lineEditBondLength->setText(QString::number(settings->getBondLength()));
  ui.doubleSpinBoxBondWidth->setValue(settings->getBondWidth());
  ui.spinBoxBondAngle->setValue(settings->getBondAngle());

  ui.libraries->clear();
  ui.libraries->addItems(settings->getLibraries());

  Molsketch::SceneSettings::MouseWheelMode mouseWheelForTools = settings->getMouseWheelMode();
  ui.mouseWheelCycleTools->setChecked(Molsketch::SceneSettings::CycleTools == mouseWheelForTools);
  ui.mouseWheelCycleTools->setChecked(Molsketch::SceneSettings::Zoom == mouseWheelForTools);

  ui.libraryPath->setText(settings->obabelIfacePath());
  ui.obfPath->setText(settings->obabelFormatsPath());
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
  settings->setAutoSaveInterval(ui.spinBoxAutoSave->value()*60000);

  // Atom settings
  settings->setCarbonVisible(ui.checkBoxShowCarbon->isChecked());
  settings->setChargeVisible(ui.checkBoxShowValency->isChecked());
  settings->setElectronSystemsVisible(ui.checkBoxShowES->isChecked());
  QFont font = ui.fontComboBox->currentFont();
  font.setPointSizeF(ui.doubleSpinBoxFontSize->value());
  settings->setAtomFont(font);

  // MsKBond settings
  settings->setBondLength(ui.lineEditBondLength->text().toDouble());
  settings->setBondWidth(ui.doubleSpinBoxBondWidth->value());
  settings->setBondAngle(ui.spinBoxBondAngle->value());

  // Library settings
  QStringList libraries;
  for (int i = 0 ; i < ui.libraries->count() ; ++i)
    libraries << ui.libraries->item(i)->text();
  settings->setLibraries(libraries);

  if (ui.mouseWheelCycleTools->isChecked())
    settings->setMouseWheelMode(ApplicationSettings::MouseWheelMode::CycleTools);
  else if (ui.mouseWheelZoom->isChecked())
    settings->setMouseWheelMode(ApplicationSettings::MouseWheelMode::Zoom);
  else
    settings->setMouseWheelMode(ApplicationSettings::MouseWheelMode::Unset);

  settings->setObabelIfacePath(ui.libraryPath->text());
  settings->setObabelFormatsPath(ui.obfPath->text());
  emit settingsChanged();
}

void SettingsDialog::selectFont()
{
  bool * ok = NULL;
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

void SettingsDialog::on_addLibrary_clicked()
{
  QString newLibrary = QFileDialog::getExistingDirectory(this, tr("Select library folder"));
  if (newLibrary.isEmpty()) return;
  ui.libraries->addItem(newLibrary);
  ui.libraries->sortItems();
}

void SettingsDialog::on_removeLibrary_clicked()
{
  foreach (QListWidgetItem* item, ui.libraries->selectedItems())
    delete item;
}

void SettingsDialog::on_obfPathButton_clicked() {
  QString folder = QFileDialog::getExistingDirectory(0, tr("OBF folder"), ui.obfPath->text());
  if (!folder.isEmpty()) ui.obfPath->setText(folder);
}

void SettingsDialog::on_libraryPathButton_clicked() {
  QString filename = QFileDialog::getOpenFileName(0, tr("Path to obabelIface"), ui.libraryPath->text(), "*.dll"); // TODO replace dll with appropriate extension
  if (!filename.isEmpty()) ui.libraryPath->setText(filename);
}
