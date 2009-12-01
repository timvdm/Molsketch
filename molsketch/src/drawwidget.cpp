/***************************************************************************
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
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "drawwidget.h"

#include <molsketch/molscene.h>

namespace Molsketch {

  DrawWidget::DrawWidget(MolScene *scene) : QWidget()
  {
    Q_CHECK_PTR(scene);
    
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

    connect(ui.buttonAny, SIGNAL(clicked()), this, SLOT(anyClicked()));

    // bonds
    connect(ui.buttonSingleBond, SIGNAL(clicked()), this, SLOT(singleBondClicked()));
    connect(ui.buttonDoubleBond, SIGNAL(clicked()), this, SLOT(doubleBondClicked()));
    connect(ui.buttonTripleBond, SIGNAL(clicked()), this, SLOT(tripleBondClicked()));
    connect(ui.buttonWedge, SIGNAL(clicked()), this, SLOT(wedgeBondClicked()));
    connect(ui.buttonHash, SIGNAL(clicked()), this, SLOT(hashBondClicked()));
    connect(ui.buttonWedgeOrHash, SIGNAL(clicked()), this, SLOT(wedgeOrHashBondClicked()));
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

    // reaction
    connect(ui.buttonReactionArrow, SIGNAL(clicked()), this, SLOT(reactionArrowClicked()));
    connect(ui.buttonMechanismArrow, SIGNAL(clicked()), this, SLOT(mechanismArrowClicked()));


    // tools
    connect(ui.buttonMove, SIGNAL(clicked()), this, SLOT(moveClicked()));
    connect(ui.buttonLasso, SIGNAL(clicked()), this, SLOT(lassoClicked()));
    connect(ui.buttonRotate, SIGNAL(clicked()), this, SLOT(rotateClicked()));
    connect(ui.buttonMinimize, SIGNAL(clicked()), this, SLOT(minimizeClicked()));
    connect(ui.buttonConnect, SIGNAL(clicked()), this, SLOT(connectClicked()));
    connect(ui.buttonCharges, SIGNAL(clicked()), this, SLOT(chargesClicked()));
    connect(ui.buttonHydrogens, SIGNAL(clicked()), this, SLOT(hydrogensClicked()));
  }


  void DrawWidget::uncheckAllButtons()
  {
    uncheckAtomButtons();
    uncheckBondButtons();
    uncheckRingButtons();
    uncheckToolButtons();
  }

  void DrawWidget::uncheckAtomButtons()
  {
    ui.buttonH->setChecked(false);
    ui.buttonC->setChecked(false);
    ui.buttonN->setChecked(false);
    ui.buttonO->setChecked(false);
    ui.buttonP->setChecked(false);
    ui.buttonS->setChecked(false);
    ui.buttonF->setChecked(false);
    ui.buttonCl->setChecked(false);
    ui.buttonBr->setChecked(false);
    ui.buttonI->setChecked(false);
    ui.buttonX->setChecked(false);
    ui.buttonR->setChecked(false);
    ui.buttonAny->setChecked(false);
  }
   
  void DrawWidget::uncheckBondButtons()
  {
    ui.buttonSingleBond->setChecked(false);
    ui.buttonDoubleBond->setChecked(false);
    ui.buttonTripleBond->setChecked(false);
    ui.buttonWedge->setChecked(false);
    ui.buttonHash->setChecked(false);
    ui.buttonWedgeOrHash->setChecked(false);
    ui.buttonCisOrTrans->setChecked(false);
  }

  void DrawWidget::uncheckRingButtons()
  {  
    ui.buttonRing3->setChecked(false);
    ui.buttonRing4->setChecked(false);
    ui.buttonRing5->setChecked(false);
    ui.buttonRing6->setChecked(false);
    ui.buttonRing7->setChecked(false);
    ui.buttonRing8->setChecked(false);
    ui.buttonRing5Arom->setChecked(false);
    ui.buttonRing6Arom->setChecked(false);
  }
  
  void DrawWidget::uncheckToolButtons()
  {
    ui.buttonReactionArrow->setChecked(false);
    ui.buttonMechanismArrow->setChecked(false);
    ui.buttonMove->setChecked(false);
    ui.buttonLasso->setChecked(false);
    ui.buttonRotate->setChecked(false);
    ui.buttonMinimize->setChecked(false);
    ui.buttonConnect->setChecked(false);
    ui.buttonCharges->setChecked(false);
    ui.buttonHydrogens->setChecked(false);
  }

  QPushButton* DrawWidget::checkedBondButton()
  {
    if (ui.buttonSingleBond->isChecked())
      return ui.buttonSingleBond;
    if (ui.buttonDoubleBond->isChecked())
      return ui.buttonDoubleBond;
    if (ui.buttonTripleBond->isChecked())
      return ui.buttonTripleBond;
    if (ui.buttonWedge->isChecked())
      return ui.buttonWedge;
    if (ui.buttonHash->isChecked())
      return ui.buttonHash;
    if (ui.buttonWedgeOrHash->isChecked())
      return ui.buttonWedgeOrHash;
    if (ui.buttonCisOrTrans->isChecked())
      return ui.buttonCisOrTrans;  
    return 0;
  }

  QPushButton* DrawWidget::checkedAtomButton()
  {
    if (ui.buttonH->isChecked())
      return ui.buttonH;
    if (ui.buttonC->isChecked())
      return ui.buttonC;
    if (ui.buttonN->isChecked())
      return ui.buttonN;
    if (ui.buttonO->isChecked())
      return ui.buttonO;
    if (ui.buttonP->isChecked())
      return ui.buttonP;
    if (ui.buttonS->isChecked())
      return ui.buttonS;
    if (ui.buttonF->isChecked())
      return ui.buttonF;
    if (ui.buttonCl->isChecked())
      return ui.buttonCl;
    if (ui.buttonBr->isChecked())
      return ui.buttonBr;
    if (ui.buttonI->isChecked())
      return ui.buttonI;
    if (ui.buttonX->isChecked())
      return ui.buttonX;
    if (ui.buttonR->isChecked())
      return ui.buttonR;
    if (ui.buttonAny->isChecked())
      return ui.buttonAny;
    return 0;
  }

  //
  // Atoms
  //

  void DrawWidget::textButtonClicked()
  {
    QObject *sender = QObject::sender();
    QPushButton *button = qobject_cast<QPushButton*>(sender);
    QString text = button->text();
    //m_scene->setElement(text);
    m_scene->setEditMode(MolScene::DrawMode);

    uncheckAtomButtons();
    if (text == "H")
      ui.buttonH->setChecked(true);
    else if (text == "C")
      ui.buttonC->setChecked(true);
    else if (text == "N")
      ui.buttonN->setChecked(true);
    else if (text == "O")
      ui.buttonO->setChecked(true);
    else if (text == "P")
      ui.buttonP->setChecked(true);
    else if (text == "S")
      ui.buttonS->setChecked(true);
    else if (text == "F")
      ui.buttonF->setChecked(true);
    else if (text == "Cl")
      ui.buttonCl->setChecked(true);
    else if (text == "Br")
      ui.buttonBr->setChecked(true);
    else if (text == "I")
      ui.buttonI->setChecked(true);
    else if (text == "X")
      ui.buttonX->setChecked(true);
    else if (text == "R")
      ui.buttonR->setChecked(true);
   
    if (!checkedBondButton()) {
      ui.buttonSingleBond->setChecked(true);
      //m_scene->setBondOrder(1);
      //m_scene->setBondType(Bond::InPlane);
    }
    uncheckRingButtons();
    uncheckToolButtons();
  }

  void DrawWidget::anyClicked()
  {
    uncheckAllButtons();
    ui.buttonAny->setChecked(true);
    m_scene->setEditMode(MolScene::TextMode);
  }
 
  //
  // Bonds
  //

  void DrawWidget::singleBondClicked()
  {
    //m_scene->setBondOrder(1);
    //m_scene->setBondType(Bond::InPlane);
    m_scene->setEditMode(MolScene::DrawMode);

    if (!checkedAtomButton()) {
      ui.buttonC->setChecked(true);
      //m_scene->setElement("C");
    }
    uncheckBondButtons();
    ui.buttonSingleBond->setChecked(true);
    uncheckRingButtons();
    uncheckToolButtons();
  }

  void DrawWidget::doubleBondClicked()
  {
    //m_scene->setBondOrder(2);
    //m_scene->setBondType(Bond::InPlane);
    m_scene->setEditMode(MolScene::DrawMode);

    if (!checkedAtomButton()) {
      ui.buttonC->setChecked(true);
      //m_scene->setElement("C");
    }
    uncheckBondButtons();
    ui.buttonDoubleBond->setChecked(true);
    uncheckRingButtons();
    uncheckToolButtons();
   }

  void DrawWidget::tripleBondClicked()
  {
    //m_scene->setBondOrder(3);
    //m_scene->setBondType(Bond::InPlane);
    m_scene->setEditMode(MolScene::DrawMode);
    
    if (!checkedAtomButton()) {
      ui.buttonC->setChecked(true);
      //m_scene->setElement("C");
    }
    uncheckBondButtons();
    ui.buttonTripleBond->setChecked(true);
    uncheckRingButtons();
    uncheckToolButtons();
  }

  void DrawWidget::wedgeBondClicked()
  {
    // also sets order
    //m_scene->setBondType(Bond::Wedge);
    m_scene->setEditMode(MolScene::DrawMode);
 
    if (!checkedAtomButton()) {
      ui.buttonC->setChecked(true);
      //m_scene->setElement("C");
    }
    uncheckBondButtons();
    ui.buttonWedge->setChecked(true);    
    uncheckRingButtons();
    uncheckToolButtons();
  }
  
  void DrawWidget::hashBondClicked()
  {
    // also sets order
    //m_scene->setBondType(Bond::Hash);
    m_scene->setEditMode(MolScene::DrawMode);

    if (!checkedAtomButton()) {
      ui.buttonC->setChecked(true);
      //m_scene->setElement("C");
    }
    uncheckBondButtons();
    ui.buttonHash->setChecked(true);
    uncheckRingButtons();
    uncheckToolButtons();
  }
  
  void DrawWidget::wedgeOrHashBondClicked()
  {
    // also sets order
    //m_scene->setBondType(Bond::WedgeOrHash);
    m_scene->setEditMode(MolScene::DrawMode);
    
    if (!checkedAtomButton()) {
      ui.buttonC->setChecked(true);
      //m_scene->setElement("C");
    }
    uncheckBondButtons();
    ui.buttonWedgeOrHash->setChecked(true); 
    uncheckRingButtons();
    uncheckToolButtons();
  }
  
  void DrawWidget::cisOrTransBondClicked()
  {
    // also sets order
    //m_scene->setBondType(Bond::CisOrTrans);
    m_scene->setEditMode(MolScene::DrawMode);

    if (!checkedAtomButton()) {
      ui.buttonC->setChecked(true);
      //m_scene->setElement("C");
    }
    uncheckBondButtons();
    ui.buttonCisOrTrans->setChecked(true);
    uncheckRingButtons();
    uncheckToolButtons();
  }
 
  //
  // Rings
  //

  void DrawWidget::ring3Clicked()
  {
    uncheckAllButtons();
    ui.buttonRing3->setChecked(true);
    //m_scene->setHintRing(3);
    m_scene->setEditMode(MolScene::DrawMode);
  }

  void DrawWidget::ring4Clicked()
  {
    uncheckAllButtons();
    ui.buttonRing4->setChecked(true);
    //m_scene->setHintRing(4);
    m_scene->setEditMode(MolScene::DrawMode);
  }
  
  void DrawWidget::ring5Clicked()
  {
    uncheckAllButtons();
    ui.buttonRing5->setChecked(true);
    //m_scene->setHintRing(5);
    m_scene->setEditMode(MolScene::DrawMode);
  }
  
  void DrawWidget::ring6Clicked()
  {
    uncheckAllButtons();
    ui.buttonRing6->setChecked(true);
    //m_scene->setHintRing(6);
    m_scene->setEditMode(MolScene::DrawMode);
  }
  
  void DrawWidget::ring7Clicked()
  {
    uncheckAllButtons();
    ui.buttonRing7->setChecked(true);
    //m_scene->setHintRing(7);
    m_scene->setEditMode(MolScene::DrawMode);
  }
  
  void DrawWidget::ring8Clicked()
  {
    uncheckAllButtons();
    ui.buttonRing8->setChecked(true);
    //m_scene->setHintRing(8);
    m_scene->setEditMode(MolScene::DrawMode);
  }
  
  void DrawWidget::aromaticRing5Clicked()
  {
    uncheckAllButtons();
    ui.buttonRing5Arom->setChecked(true);
    //m_scene->setHintRing(5, true);
    m_scene->setEditMode(MolScene::DrawMode);
  }

  void DrawWidget::aromaticRing6Clicked()
  {
    uncheckAllButtons();
    ui.buttonRing6Arom->setChecked(true);
    //m_scene->setHintRing(6, true);
    m_scene->setEditMode(MolScene::DrawMode);
  }
  
  void DrawWidget::reactionArrowClicked()
  {
    uncheckAllButtons();
    ui.buttonReactionArrow->setChecked(true);
    m_scene->setEditMode(MolScene::ReactionMode);
  }

  void DrawWidget::mechanismArrowClicked()
  {
    uncheckAllButtons();
    ui.buttonMechanismArrow->setChecked(true);
    m_scene->setEditMode(MolScene::MechanismMode);
  }

  void DrawWidget::moveClicked()
  {
    uncheckAllButtons();
    ui.buttonMove->setChecked(true);
    m_scene->setEditMode(MolScene::MoveMode);
  }

  void DrawWidget::lassoClicked()
  {
    uncheckAllButtons();
    ui.buttonLasso->setChecked(true);
    m_scene->setEditMode(MolScene::LassoMode);
  }

  void DrawWidget::rotateClicked()
  {
    uncheckAllButtons();
    ui.buttonRotate->setChecked(true);
    m_scene->setEditMode(MolScene::RotateMode);
  }
  
  void DrawWidget::minimizeClicked()
  {
    uncheckAllButtons();
    ui.buttonMinimize->setChecked(true);
    m_scene->setEditMode(MolScene::MinimiseMode);
  }
 
  void DrawWidget::connectClicked()
  {  
    uncheckAllButtons();
    ui.buttonConnect->setChecked(true);
    m_scene->setEditMode(MolScene::ConnectMode);
  }

  void DrawWidget::chargesClicked()
  {
    uncheckAllButtons();
    ui.buttonCharges->setChecked(true);
    m_scene->setEditMode(MolScene::ChargeMode);
  }

  void DrawWidget::hydrogensClicked()
  {
    uncheckAllButtons();
    ui.buttonHydrogens->setChecked(true);
    m_scene->setEditMode(MolScene::HydrogenMode);
  }

} // end namespace Molsketch

//#include "drawwidget.moc"
