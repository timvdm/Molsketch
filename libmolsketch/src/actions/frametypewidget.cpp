/***************************************************************************
 *   Copyright (C) 2016 Hendrik Vennekate                                  *
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
#include "frametypewidget.h"
#include "frameaction.h"

namespace Molsketch {

#define ADDFRAMEBUTTON(FRAME, ICON) addButton(-1, QPixmap::fromImage(QImage(":/images/" + QString(ICON) + ".svg")), QVariant(FRAME));

  FrameTypeWidget::FrameTypeWidget(QWidget *parent)
    : ItemTypeWidget(parent)
  {
    addButton(-1, QPixmap(), QVariant());
    ADDFRAMEBUTTON(FrameAction::bracketsFrame(), "bracket")
    ADDFRAMEBUTTON(FrameAction::angleFrame(), "angle")
    ADDFRAMEBUTTON(FrameAction::curlyBracketsFrame(), "curlybracket")
    ADDFRAMEBUTTON(FrameAction::rectangleFrame(), "fullframe")
  }

} // namespace Molsketch
