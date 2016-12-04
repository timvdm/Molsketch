/***************************************************************************
 *   Copyright (C) 2015 Hendrik Vennekate                                  *
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
#ifndef ATOMPOPUP_H
#define ATOMPOPUP_H

#include "propertieswidget.h"

namespace Ui {
  class AtomPopup;
}

namespace Molsketch {
  class Atom;

  class AtomPopup : public PropertiesWidget
  {
    Q_OBJECT

  public:
    explicit AtomPopup(QWidget *parent = 0);
    ~AtomPopup();

    void connectAtom(Atom* a);

  private slots:
    void on_element_textChanged(const QString &arg1);
    void on_charge_valueChanged(int arg1);
    void on_hydrogens_valueChanged(int arg1);
    void onCoordinatesDatachanged();
  private:
    Ui::AtomPopup *ui;
    class PrivateData;
    PrivateData *d;
    void propertiesChanged();
    // TODO write macro instead for property popup
  };

} // namespace

#endif // ATOMPOPUP_H
