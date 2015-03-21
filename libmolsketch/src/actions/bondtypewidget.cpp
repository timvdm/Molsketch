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
#include "bondtypewidget.h"
#include <QButtonGroup>
#include <QToolButton>
#include <QHBoxLayout>

namespace Molsketch {

  bondTypeWidget::typeIconPair tipGen(int t, const QString& name, bool invert = false)
  {
    QImage icon(":images/" + name + ".svg");
    return bondTypeWidget::typeIconPair(t, QPixmap::fromImage(icon.mirrored(invert, invert)));
  }

  bondTypeWidget::bondTypeWidget(bool withInversion, QWidget *parent)
    : ItemTypeWidget(parent)
  {
    QList<typeIconPair> buttonList;
    buttonList
        << tipGen(Bond::Single, "single")
        << tipGen(Bond::Hash, "hash");
    if (withInversion) buttonList << tipGen(- Bond::Hash, "hash", true);
    buttonList
        << tipGen(Bond::Wedge, "wedge");
    if (withInversion) buttonList << tipGen(- Bond::Wedge, "wedge", true);
    buttonList
               << tipGen(Bond::WedgeOrHash, "hashOrWedge")
               << tipGen(Bond::Double, "double")
               << tipGen(Bond::CisOrTrans, "cistrans")
               << tipGen(Bond::Triple, "triple");
    setButtons(buttonList);
  }

  bool bondTypeWidget::backward() const
  {
    return currentType() < 0 ;
  }

  Bond::BondType bondTypeWidget::bondType() const
  {
    return (Bond::BondType) qAbs(currentType()) ;
  }

  void bondTypeWidget::setBondType(Bond::BondType type) const
  {
    setCurrentType(type);
  }

} // namespace
