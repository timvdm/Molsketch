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
#ifndef COLORBUTTON_H
#define COLORBUTTON_H

#include <QPushButton>

namespace Molsketch {

  class ColorButton : public QPushButton
  {
    Q_OBJECT
  public:
    explicit ColorButton(QWidget *parent = 0, const QColor& color = Qt::black);
    QColor getColor() const;
  public slots:
    void setColor(const QColor&);
  signals:
    void colorChanged(const QColor&);
  private slots:
    void changeColor();
  };

} // namespace Molsketch

#endif // COLORBUTTON_H
