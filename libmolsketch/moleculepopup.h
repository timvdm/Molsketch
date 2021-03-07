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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/
#ifndef MOLECULEPOPUP_H
#define MOLECULEPOPUP_H

#include <QWidget>

namespace Ui {
  class MoleculePopup;
}


namespace Molsketch {

  class Molecule;

  class MoleculePopup : public QWidget
  {
    Q_OBJECT

  public:
    explicit MoleculePopup(QWidget *parent = 0);

    void connectMolecule(Molecule* m);
    ~MoleculePopup();

  protected:
    void focusOutEvent(QFocusEvent* e) override;
  private:
    ::Ui::MoleculePopup *ui;
    class privateData;
    privateData *d;
    void showEvent(QShowEvent* e) override;
  private slots:
    void applyPropertiesToMolecule();
    void on_saveButton_clicked();
  };

} // namespace
#endif // MOLECULEPOPUP_H
