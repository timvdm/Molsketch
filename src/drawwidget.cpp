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

#include "drawwidget.h"
#include "molscene.h"

namespace Molsketch {

  DrawWidget::DrawWidget(MolScene *scene) : QWidget()
  {
    Q_CHECK_PTR(m_scene);
    
    m_scene = scene;
    ui.setupUi(this);

    // H C N ... Br I X R Any
    connect(ui.buttonH, SIGNAL(clicked()), this, SLOT(textButtonClicked()));
    connect(ui.buttonC, SIGNAL(clicked()), this, SLOT(textButtonClicked()));
    connect(ui.buttonN, SIGNAL(clicked()), this, SLOT(textButtonClicked()));
    connect(ui.buttonO, SIGNAL(clicked()), this, SLOT(textButtonClicked()));
    connect(ui.buttonP, SIGNAL(clicked()), this, SLOT(textButtonClicked()));
    connect(ui.buttonS, SIGNAL(clicked()), this, SLOT(textButtonClicked()));
    connect(ui.buttonF, SIGNAL(clicked()), this, SLOT(textButtonClicked()));
    connect(ui.buttonCl, SIGNAL(clicked()), this, SLOT(textButtonClicked()));
    connect(ui.buttonBr, SIGNAL(clicked()), this, SLOT(textButtonClicked()));
    connect(ui.buttonI, SIGNAL(clicked()), this, SLOT(textButtonClicked()));
    connect(ui.buttonX, SIGNAL(clicked()), this, SLOT(textButtonClicked()));
    connect(ui.buttonR, SIGNAL(clicked()), this, SLOT(textButtonClicked()));
    connect(ui.buttonAny, SIGNAL(clicked()), this, SLOT(textButtonClicked()));

    // bonds
    connect(ui.buttonSingleBond, SIGNAL(clicked()), this, SLOT(singleBondClicked()));
    connect(ui.buttonDoubleBond, SIGNAL(clicked()), this, SLOT(doubleBondClicked()));
    connect(ui.buttonTripleBond, SIGNAL(clicked()), this, SLOT(tripleBondClicked()));
    connect(ui.buttonWedge, SIGNAL(clicked()), this, SLOT(wedgeBondClicked()));
    connect(ui.buttonHash, SIGNAL(clicked()), this, SLOT(hashBondClicked()));
    connect(ui.buttonWedgeOrHash, SIGNAL(clicked()), this, SLOT(hashOrWedgeBondClicked()));
    connect(ui.buttonCisOrTrans, SIGNAL(clicked()), this, SLOT(cisOrTransBondClicked()));

    // rings
    connect(ui.buttonRing3, SIGNAL(clicked()), this, SLOT(ring3Clicked()));
    connect(ui.buttonRing4, SIGNAL(clicked()), this, SLOT(ring4Clicked()));
    connect(ui.buttonRing5, SIGNAL(clicked()), this, SLOT(ring5Clicked()));
    connect(ui.buttonRing6, SIGNAL(clicked()), this, SLOT(ring6Clicked()));
    connect(ui.buttonRing7, SIGNAL(clicked()), this, SLOT(ring7Clicked()));
    connect(ui.buttonRing8, SIGNAL(clicked()), this, SLOT(ring8Clicked()));
    connect(ui.buttonRing5Arom, SIGNAL(clicked()), this, SLOT(aromaticRing5Clicked()));
    connect(ui.buttonRing6Arom, SIGNAL(clicked()), this, SLOT(aromaticRing6Clicked()));
  }

  void DrawWidget::textButtonClicked()
  {
    QObject *sender = QObject::sender();
    QPushButton *button = qobject_cast<QPushButton*>(sender);
    button->setDown(true);
    m_scene->setElement(button->text());  
  }

  void DrawWidget::singleBondClicked()
  {
    m_scene->setBondOrder(1);
    m_scene->setBondType(Bond::InPlane);  
  }

  void DrawWidget::doubleBondClicked()
  {
    m_scene->setBondOrder(2);
    m_scene->setBondType(Bond::InPlane);  
  }

  void DrawWidget::tripleBondClicked()
  {
    m_scene->setBondOrder(3);
    m_scene->setBondType(Bond::InPlane);  
  }

  void DrawWidget::wedgeBondClicked()
  {
    m_scene->setBondType(Bond::Wedge);  
  }
  
  void DrawWidget::hashBondClicked()
  {
    m_scene->setBondType(Bond::Hash);  
  }
  
  void DrawWidget::wedgeOrHashBondClicked()
  {
    m_scene->setBondType(Bond::WedgeOrHash);  
  }
  
  void DrawWidget::cisOrTransBondClicked()
  {
    m_scene->setBondType(Bond::CisOrTrans);  
  }
  
  void DrawWidget::ring3Clicked()
  {
    m_scene->setHintRing(3);
  }
  void DrawWidget::ring4Clicked()
  {
    m_scene->setHintRing(4);
  }
  void DrawWidget::ring5Clicked()
  {
    m_scene->setHintRing(5);
  }
  void DrawWidget::ring6Clicked()
  {
    m_scene->setHintRing(6);
  }
  void DrawWidget::ring7Clicked()
  {
    m_scene->setHintRing(7);
  }
  void DrawWidget::ring8Clicked()
  {
    m_scene->setHintRing(8);
  }
  void DrawWidget::aromaticRing5Clicked()
  {
    m_scene->setHintRing(5);
  }
  void DrawWidget::aromaticRing6Clicked()
  {
    m_scene->setHintRing(6);
  }


} // end namespace Molsketch

//#include "drawwidget.moc"
