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
#include "arrowtypewidget.h"
#include "arrow.h"

namespace Molsketch {

#define TIPGEN(TYPE, IMAGESTRING) addButton(TYPE, QPixmap::fromImage(QImage(":/images/" + QString(IMAGESTRING) + ".png")));

  arrowTypeWidget::arrowTypeWidget(QWidget *parent)
    : ItemTypeWidget(parent)
  {
    TIPGEN(Arrow::NoArrow, "line") // --
    TIPGEN(Arrow::UpperBackward | Arrow::LowerBackward, "arrow") // ->
    TIPGEN(Arrow::UpperForward | Arrow::LowerForward | Arrow::UpperBackward | Arrow::LowerBackward, "resonanceArrow") // <->
    TIPGEN(Arrow::UpperBackward, "hookUp") // -P
    TIPGEN(Arrow::LowerBackward, "hookDown") // -b
    TIPGEN(Arrow::UpperForward | Arrow::UpperBackward, "eqHook") //
    TIPGEN(Arrow::UpperForward | Arrow::LowerBackward, "eqMirrorHook")
    TIPGEN(Arrow::LowerForward | Arrow::UpperBackward, "eqMirrorHookb")
  }

} // namespace Molsketch

