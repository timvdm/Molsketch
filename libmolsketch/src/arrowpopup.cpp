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
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
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
      ui->curved->setChecked(originalProperties.spline);
      qDebug() << originalProperties.points;
      ui->Coordinates->model()->setCoordinates(originalProperties.points);
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
    QWidget(parent),
    ui(new Ui::arrowPopup),
    d(new privateData)
  {
    ui->setupUi(this);
    ui->Coordinates->setModel(new CoordinateModel(ui->Coordinates));
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

    setWindowFlags(Qt::Popup);
  }

  void ArrowPopup::connectArrow(Arrow *a)
  {
    d->arrow = a;
    if (!a) return;
    d->transferPropertiesToUi(ui,d->arrow->getProperties());
    checkSplineEligibility();
  }

  // immediate TODO: Enable quit by pressing Esc (revert settings first) or loosing focus
  // connect this to arrow somehow

  // TODO precaution against illegal number of coordinates in spline
  //      - disable "spline" checkbox if illegal number
  //      - Tooltip for "spline"
  ArrowPopup::~ArrowPopup()
  {
    delete ui;
    delete d;
  }

  void ArrowPopup::focusOutEvent(QFocusEvent *e)
  {
    Q_UNUSED(e)
    deleteLater();
  }

  void ArrowPopup::showEvent(QShowEvent *e)
  {
    connectArrow(d->arrow);
    QWidget::showEvent(e);
  }

  void ArrowPopup::applyPropertiesToArrow()
  {
    if (!d->arrow) return;
    MolScene *scene = qobject_cast<MolScene*>(d->arrow->scene());
    if (scene)
      scene->stack()->push(new Commands::setArrowProperties(d->arrow, d->getPropertiesFromUi(ui), tr("Modify arrow")));
    else
      d->arrow->setProperties(d->getPropertiesFromUi(ui));
  }

  void ArrowPopup::checkSplineEligibility()
  {
    ui->curved->setEnabled(
          Arrow::splinePossible(
            ui->Coordinates->model()->rowCount(QModelIndex())
            ));
  }

} // namespace
