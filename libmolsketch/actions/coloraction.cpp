/***************************************************************************
 *   Copyright (C) 2007-2008 by Harm van Eersel                            *
 *   Copyright (C) 2009 Tim Vandermeersch                                  *
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

#include <QColorDialog>
#include "coloraction.h"
#include "commands.h"

namespace Molsketch {

  struct colorAction::privateData
  {
    QColor color;
  };

  colorAction::colorAction(MolScene *parent) :
    abstractRecursiveItemAction(parent),
    d(new privateData)
  {
    setText(tr("Color...")) ;
    setToolTip(tr("Set color")) ;
    setWhatsThis(tr("Displays the color chooser dialog")) ;
    QPixmap newicon(22,22);
    newicon.fill(Qt::black);
    d->color = QColor(Qt::black);
    setIcon(newicon);
    setCheckable(false);
  }

  colorAction::~colorAction()
  {
    delete d;
  }

  void colorAction::execute()
  {
    QColor newColor = QColorDialog::getColor(d->color) ;
    if (!newColor.isValid()) return ;

    d->color = newColor;
    QPixmap newicon(22,22);
    newicon.fill(d->color);
    setIcon(newicon);

    ITERATEOVERITEMSMACRO("Change color", changeColor, newColor)
  }

}// namespace
