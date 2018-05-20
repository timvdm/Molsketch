/***************************************************************************
 *   Copyright (C) 2017 Hendrik Vennekate                                  *
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
#include "colorbutton.h"

#include <QColorDialog>

namespace Molsketch {

  ColorButton::ColorButton(QWidget *parent, const QColor &color)
    : QPushButton(parent)
  {
    setFlat(true);
    setAutoFillBackground(true);
    setColor(color);
    connect(this, SIGNAL(clicked(bool)), this, SLOT(changeColor()));
  }

  QColor ColorButton::getColor() const {
    return palette().color(QPalette::Button);
  }

  void ColorButton::setColor(const QColor &color) {
    QPalette p = palette();
    p.setColor(QPalette::Button, color);
    setPalette(QPalette(color));
    emit colorChanged(color);
  }

  void ColorButton::changeColor()
  {
    QColor newColor = QColorDialog::getColor(getColor(), nullptr, tr("Choose color"));
    if (newColor.isValid()) setColor(newColor);
  }

} // namespace Molsketch
