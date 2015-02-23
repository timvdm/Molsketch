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
#ifndef BONDTYPEWIDGET_H
#define BONDTYPEWIDGET_H

#include <QWidget>
#include "bond.h"

namespace Molsketch {

  class bondTypeWidget : public QWidget
  {
    Q_OBJECT
  public:
    enum BondType
    {
      Single = 10,
      Wedge  = 11,
      Hash   = 12,
      WedgeOrHash = 13,
      Double = 20,
      CisOrTrans = 21,
      Triple = 30
    };

    explicit bondTypeWidget(QWidget *parent = 0);
    ~bondTypeWidget();
    bool backward() const ;
    BondType bondType() const ;
    Bond::BondType legacyType() const;
    QPixmap bondIcon() const ;
    int bondOrder() const ;

  signals:
    void bondTypeChanged(const bondTypeWidget::BondType& newType) ;

  private:
    class privateData ;
    privateData *d;
  private slots:
    void changeType() ;
  };

} // namespace

#endif // BONDTYPEWIDGET_H
