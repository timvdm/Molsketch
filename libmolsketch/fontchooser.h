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
#ifndef MOLSKETCH_FONTCHOOSER_H
#define MOLSKETCH_FONTCHOOSER_H

#include <QWidget>

namespace Molsketch {

  namespace Ui {
    class FontChooser;
  }

  class FontChooser : public QWidget
  {
    Q_OBJECT
  public:
    explicit FontChooser(QWidget *parent = 0);
    ~FontChooser();
    QFont getSelectedFont() const;
  signals:
    void fontChanged(const QFont&) const;
  public slots:
    void setFont(const QFont&);
  private slots:
    void fontSelectionChanged() const;
  private:
    Ui::FontChooser *ui;
  };


} // namespace Molsketch
#endif // MOLSKETCH_FONTCHOOSER_H
