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
#ifndef MOLSKETCH_ITEMTYPEWIDGET_H
#define MOLSKETCH_ITEMTYPEWIDGET_H

#include <QWidget>
#include <QBoxLayout>

namespace Molsketch {

  class ItemTypeWidget : public QWidget
  {
    Q_OBJECT
  public:
    typedef QPair<int, QPixmap> typeIconPair;
    explicit ItemTypeWidget(QWidget *parent = 0);
    ~ItemTypeWidget();
    int currentType() const ;
    void setCurrentType(const int& type) const;
    QPixmap currentIcon() const ;
    void setDirection(QBoxLayout::Direction);
    void setButtons(QList<typeIconPair > typeButtons);

  signals:
    void currentTypeChanged(const int& newType) ;

  private:
    class privateData ;
    privateData *d;
  };

} // namespace Molsketch

#endif // MOLSKETCH_ITEMTYPEWIDGET_H
