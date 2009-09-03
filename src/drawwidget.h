/**********************************************************************
  DrawWidget - Widget containing draw buttons

  Copyright (C) 2009 Tim Vandermeersch

  This file is part of the Avogadro molecular editor project.
  For more information, see <http://github.com/timvdm/Molsketch>

  Avogadro is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  Avogadro is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
  02110-1301, USA.
 **********************************************************************/

#ifndef MSK_DRAWWIDGET_H
#define MSK_DRAWWIDGET_H

#include "ui_drawwidget.h"

#include <QWidget>

namespace Molsketch {

  class MolScene;
  class DrawWidget : public QWidget
  {
    Q_OBJECT

    public:
      DrawWidget(MolScene *scene);

    private Q_SLOTS:
      void uncheckAllButtons();
      void uncheckAtomButtons();
      void uncheckBondButtons();
      void uncheckRingButtons();
      void uncheckToolButtons();
      QPushButton* checkedAtomButton();
      QPushButton* checkedBondButton();

      void textButtonClicked();
      void anyClicked();
      void singleBondClicked();
      void doubleBondClicked();
      void tripleBondClicked();
      void wedgeBondClicked();
      void hashBondClicked();
      void wedgeOrHashBondClicked();
      void cisOrTransBondClicked();
      void ring3Clicked();
      void ring4Clicked();
      void ring5Clicked();
      void ring6Clicked();
      void ring7Clicked();
      void ring8Clicked();
      void aromaticRing5Clicked();
      void aromaticRing6Clicked();

      void moveClicked();
      void lassoClicked();
      void rotateClicked();
      void minimizeClicked();
      void connectClicked();

    private:
      Ui::DrawWidget ui;
      MolScene *m_scene;
  };

} // end namespace Molsketch

#endif
