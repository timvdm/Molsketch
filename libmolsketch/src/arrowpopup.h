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
#ifndef ARROWPOPUP_H
#define ARROWPOPUP_H

#include <QWidget>

namespace Ui {
  class arrowPopup;
}

namespace Molsketch {

  class Arrow;

  class ArrowPopup : public QWidget
  {
    Q_OBJECT

  public:
    explicit ArrowPopup(QWidget *parent = 0);

    void connectArrow(Arrow* a);
    ~ArrowPopup();

  protected:
    void focusOutEvent(QFocusEvent *e);
  private:
    Ui::arrowPopup *ui;
    class privateData;
    privateData *d;
    void showEvent(QShowEvent *e);
  private slots:
    void applyPropertiesToArrow();
    void checkSplineEligibility();
  };

} // namespace

#endif // ARROWPOPUP_H
