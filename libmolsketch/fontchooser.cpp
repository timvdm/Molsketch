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
#include "fontchooser.h"
#include "ui_fontchooser.h"

namespace Molsketch {

  FontChooser::FontChooser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FontChooser)
  {
    ui->setupUi(this);
  }

  FontChooser::~FontChooser()	{
    delete ui;
  }

  QFont FontChooser::getSelectedFont() const {
    QFont font = ui->fontName->currentFont();
    font.setPointSize(ui->size->value());
    font.setBold(ui->bold->isChecked());
    font.setItalic(ui->italic->isChecked());
    return font;
  }

  void FontChooser::setFont(const QFont &font) {
    blockSignals(true);
    ui->fontName->setCurrentFont(font);
    ui->size->setValue(font.pointSize());
    ui->bold->setChecked(font.bold());
    ui->italic->setChecked(font.italic());
    blockSignals(false);
    fontSelectionChanged();
  }

  void FontChooser::fontSelectionChanged() const {
    emit fontChanged(getSelectedFont());
  }

} // namespace Molsketch
