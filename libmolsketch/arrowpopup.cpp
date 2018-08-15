/***************************************************************************
 *   Copyright (C) 2015 Hendrik Vennekate                                  *
 *      --- in part based on code originally written by: ---               *
 *   Copyright (C) 2007-2008 by Harm van Eersel                            *
 *   Copyright (C) 2009 Tim Vandermeersch                                  *
 *   Copyright (C) 2009 by Nicola Zonta                                    *
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
#include <QItemDelegate>
#include <QLineEdit>
#include <map>
#include <molscene.h>
#include "arrowpopup.h"
#include "ui_arrowpopup.h"
#include "arrow.h"
#include "commands.h"
#include <QDebug>
#include "coordinatemodel.h"

namespace Molsketch {

  struct ArrowPopup::privateData
  {
    Arrow *arrow;
    // TODO make static
    std::map<QCheckBox*, Arrow::ArrowTypeParts> uiToPropertyAssignment;
    void transferPropertiesToUi(Ui::arrowPopup* ui,
                                const Arrow::Properties& originalProperties)
    {
      for (auto arrowPart : uiToPropertyAssignment)
        arrowPart.first->setChecked(originalProperties.arrowType & arrowPart.second);
      ui->Coordinates->model()->setCoordinates(originalProperties.points);
      ui->curved->setChecked(arrow->getSpline());
      ui->curved->setEnabled(Arrow::splinePossible(
              ui->Coordinates->model()->rowCount(QModelIndex())));
    }
    Arrow::Properties getPropertiesFromUi(Ui::arrowPopup* ui)
    {
      Arrow::Properties result;
      result.arrowType = Arrow::NoArrow;
      for (auto arrowPart : uiToPropertyAssignment)
        if (arrowPart.first->isChecked())
          result.arrowType |= arrowPart.second;
      result.spline = ui->curved->isChecked();
      result.points = ui->Coordinates->model()->getCoordinates();
      return result;
    }
  };

  ArrowPopup::ArrowPopup(QWidget *parent) :
    PropertiesWidget(parent),
    ui(new Ui::arrowPopup),
    d(new privateData)
  {
    ui->setupUi(this);
    d->arrow = 0;
    d->uiToPropertyAssignment = std::map<QCheckBox*, Arrow::ArrowTypeParts>
    {
        {ui->beginningBottomTip, Arrow::LowerForward },
        {ui->endBottomTip,       Arrow::LowerBackward},
        {ui->beginningTopTip,    Arrow::UpperForward },
        {ui->endTopTip,          Arrow::UpperBackward},
    };

    for(auto child : findChildren<QCheckBox*>())
      connect(child, SIGNAL(toggled(bool)), this, SLOT(applyPropertiesToArrow()));

    connect(ui->Coordinates->model(), SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(applyPropertiesToArrow()));

    setWindowFlags(Qt::Popup);
    setObjectName("arrow properties");
  }

  void ArrowPopup::connectArrow(Arrow *a)
  {
    d->arrow = a;
    setScene(a ? dynamic_cast<MolScene*>(a->scene()) : 0);
  }

  ArrowPopup::~ArrowPopup()
  {
    delete ui;
    delete d;
  }

  void ArrowPopup::propertiesChanged()
  {
    if (!scene()) return; // TODO reset GUI
    if (itemValid(d->arrow)) d->transferPropertiesToUi(ui, d->arrow->getProperties());
  }

  void ArrowPopup::applyPropertiesToArrow()
  {
    attemptToPushUndoCommand(new Commands::setArrowProperties(d->arrow, d->getPropertiesFromUi(ui), tr("Modify arrow")));
  }

} // namespace
