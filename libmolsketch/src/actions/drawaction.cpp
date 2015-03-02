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
#include "drawaction.h"
#include <QPainter>
#include <QVBoxLayout>
#include <QDockWidget>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItemGroup>
#include <cmath>
#include <QMainWindow>
#include "periodictablewidget.h"
#include "bondtypewidget.h"
#include "molecule.h"
#include "molscene.h"
#include "commands.h"
#include "math2d.h"
#include <QDebug>

// TODO snap to help points
namespace Molsketch {

  class drawAction::privateData
  {
  public:
    QDockWidget *dock ;
    periodicTableWidget *periodicTable;
    bondTypeWidget *bondType;
    QGraphicsLineItem hintLine;
    bool autoAddHydrogen;
    QGraphicsItemGroup hintPointsGroup;
    qreal bondAngle;
    qreal bondLength;
    drawAction *parent;

    privateData(drawAction* p) :
      hintLine(QLineF(0,0,0,0)),
      autoAddHydrogen(false),
      bondAngle(Bond::defaultAngle),
      bondLength(Bond::defaultLength),
      parent(p)
    {
      hintLine.setAcceptedMouseButtons(Qt::NoButton);
      hintLine.setZValue(-1);

      // Initialize hint point circle
      for (int i = 0; i < 360./bondAngle ; ++i)
      {
        QGraphicsEllipseItem* dot = new QGraphicsEllipseItem(-2.5,-2.5,5,5);
        dot->setBrush(Qt::lightGray);
        dot->setPen(Qt::NoPen);
        dot->setPos(QLineF::fromPolar(bondLength, bondAngle * i).p2());
        hintPointsGroup.addToGroup(dot);
      }
      hintPointsGroup.setAcceptedMouseButtons(Qt::NoButton);
    }


    QPointF nearestPoint(const QPointF& currentPosition)
    {
      QPointF nPoint = currentPosition ;

      // Check the hinting points
      qreal minDistance = bondLength / 4.;
      foreach(const QGraphicsItem* hintPoint, hintPointsGroup.childItems())
      {
        qreal distance = QLineF(hintPoint->scenePos(), currentPosition).length();
        if (distance > minDistance) continue;
        minDistance = distance;
        nPoint = hintPoint->scenePos();
      }

      // Look whether a atom is nearby
      Atom* atom = parent->scene()->atomAt(currentPosition);
      if (atom) nPoint = atom->scenePos();

      return nPoint;
    }

    };

  drawAction::drawAction(MolScene *scene)
    : genericAction(scene),
      d(new privateData(this))
  {
    d->dock = new QDockWidget(tr("Draw mode"), parentWidget()) ;
    d->dock->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum); // TODO minimize size
    d->dock->setWidget(new QWidget(d->dock));
    d->dock->widget()->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    QVBoxLayout *layout = new QVBoxLayout(d->dock->widget()) ;
    layout->setAlignment(Qt::AlignLeft | Qt::AlignTop) ;
    d->periodicTable = new periodicTableWidget(d->dock) ;
    d->bondType = new bondTypeWidget(d->dock) ;
    d->dock->widget()->layout()->addWidget(d->periodicTable);
    d->dock->widget()->layout()->addWidget(d->bondType);
    connect(d->periodicTable, SIGNAL(elementChanged(QString)),
            this, SLOT(refreshIcon()));
    connect(d->bondType, SIGNAL(bondTypeChanged(const bondTypeWidget::BondType&)),
            this, SLOT(refreshIcon()));
    connect(this, SIGNAL(toggled(bool)),
            this, SLOT(toggleVisibility(bool)));
    refreshIcon(); // TODO
    if (parentWidget())
      qobject_cast<QMainWindow*>(parentWidget())->addDockWidget(Qt::LeftDockWidgetArea, d->dock);
    d->dock->hide();
    setText(tr("Draw"));
  }

  drawAction::~drawAction()
  {
    delete d->dock ;
    delete d ;
  }

  void drawAction::mousePressEvent(QGraphicsSceneMouseEvent *event)
  {
    if (Qt::LeftButton != event->button()) return ;
    if (event->modifiers()) return ; // TODO change for "snap to grid"
    // TODO delete action

    // Get the position
    QPointF downPos = event->buttonDownScenePos(event->button());

    // TODO handle group abbreviations (Ts, Bn, Bz, tBu, iBu, nBu...) -> settings dialog, settings per file (to allow for different conventions)

    // TODO move to bond!!
    //    Bond *clickedBond = scene()->bondAt(downPos);
//    Atom *clickedAtom = scene()->atomAt(downPos);

    // TODO Check for bond click

    // Show hinting
    // hint points
    Atom* hintAtom = scene()->atomAt(downPos);
    d->hintPointsGroup.setPos(hintAtom
                              ? hintAtom->scenePos()
                              : downPos);
    scene()->addItem(&(d->hintPointsGroup)); // TODO attn: scene takes ownership...

    // hint line
    d->hintLine.setLine(QLineF(downPos, event->scenePos()));
    scene()->addItem(&(d->hintLine)); // TODO attn: scene takes ownership...
    d->hintLine.setVisible(true);
    event->accept();
  }

  void drawAction::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
  {
     // Check hinting conditions
    if (!(event->buttons() & Qt::LeftButton)) return;

    // Set hinting
    d->hintLine.setLine(QLineF(
                          d->nearestPoint(event->buttonDownScenePos(Qt::LeftButton)),
                          d->nearestPoint(event->scenePos())));
    event->accept();
  }

  // TODO test cases: insert single atom on blank
  //                  insert atom with existing beginAtom
  //                  insert atom with existing endAtom
  //                  link two existing atoms from different molecules
  //                  link two existing atoms from the same molecule
  void drawAction::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
  {
    event->accept();
    // Remove the hinting
    if (d->hintLine.scene())
      d->hintLine.scene()->removeItem(&(d->hintLine));
    if (d->hintPointsGroup.scene())
      d->hintPointsGroup.scene()->removeItem(&(d->hintPointsGroup));

    // Get atoms
    QPointF beginPos = event->buttonDownScenePos(event->button());
    Atom *beginAtom = scene()->atomAt(beginPos);
    QPointF endPos = event->scenePos();
    Atom *endAtom = scene()->atomAt(endPos);

    // check if they are already connected
    if (beginAtom
        && endAtom)
    {
      if (beginAtom == endAtom) return; // or the same (here: alternatively insert additional neighbor TODO )
      if (beginAtom->neighbours().contains(endAtom)) return; // TODO change bond type
    }

    // Ok, let's get to business...
    attemptBeginMacro(tr("Draw"));
    Molecule *newMolecule = new Molecule();
    attemptUndoPush(new Commands::AddItem(newMolecule, scene()));

    // At this point: check if no first atom exists, then insert new and add to molecule
    // otherwise merge first atom's molecule into newly created molecule.
    if (!beginAtom)
    {
      beginAtom = new Atom(beginPos,
                          d->periodicTable->currentElement(),
                          d->autoAddHydrogen);
      attemptUndoPush(new Commands::AddAtom(beginAtom, newMolecule));
    }
    else
    {
      *newMolecule += *(beginAtom->molecule());
      attemptUndoPush(new Commands::DelItem(beginAtom->molecule()));
      beginAtom = scene()->atomAt(beginPos); // TODO redo this...
      endAtom = scene()->atomAt(endPos); // Refresh...
    }

    // Now: If we don't have an end atom, check that it would not coincide with the
    // atom we just inserted, then insert and add to molecule
    if (!endAtom && scene()->atomAt(endPos) != beginAtom)
    {
      endAtom = new Atom(d->nearestPoint(endPos),
                         d->periodicTable->currentElement(),
                         d->autoAddHydrogen);
      attemptUndoPush(new Commands::AddAtom(endAtom, newMolecule));
    }

    // If we have an end atom at this point...
    if (endAtom)
    {
      // make sure it's part of the same molecule
      // (It may already be, if endAtom and beginAtom were initially part of the same molecule)
      if (endAtom->molecule() != newMolecule)
      {
        *newMolecule += *(endAtom->molecule());
        attemptUndoPush(new Commands::DelItem(endAtom->molecule())); // TODO: new function "absorb other molecule"
        endAtom = scene()->atomAt(endPos); // Refresh...
      }
      // Add bond
      if (d->bondType->backward()) qSwap(beginAtom, endAtom);
      attemptUndoPush(new Commands::AddBond(new Bond(beginAtom, endAtom,
                                                     d->bondType->bondType())));
    }

    // That should've been it...
    attemptEndEndMacro();

    scene()->update();

  }

  void drawAction::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
  {// TODO make part of mouseRelease (see above)
    event->accept();
    QUndoStack *stack = undoStack();
    QPointF downPos = event->buttonDownScenePos(event->button());

    //clicking on an atom spawns a new bond from that atom
    if (scene()->atomAt (downPos)) {
      QPointF new_atom_pos = downPos;
      Atom *at1 = scene()->atomAt (downPos);
      switch (at1 ->numBonds()) {
        case 0:
          {
            qreal x = new_atom_pos.x ();
            new_atom_pos.setX (x + d->bondLength);
            break;
          }
        case 1:
          {
            Atom *at2 = at1 ->neighbours()[0];
            if (at2 ->neighbours() .size ()< 2) {
              QPointF v = downPos - at1 ->neighbours()[0] ->pos ();

              QPointF rotated_v (v.x()*0.5 - v.y()*sqrt(3)*0.5, v.x()*0.5*sqrt(3) + v.y () *0.5);
              qreal mod = sqrt (rotated_v.x()*rotated_v.x() + rotated_v.y()*rotated_v.y());
              rotated_v *= d->bondLength/mod;
              new_atom_pos = rotated_v + downPos;
            } else {
              Atom *at3 = at2 ->neighbours()[0];
              if (at3 == at1) at3 = at2 ->neighbours()[1];
              QPointF rotated_v = at2 ->pos () - at3 ->pos ();
              qreal mod = sqrt (rotated_v.x()*rotated_v.x() + rotated_v.y()*rotated_v.y());
              rotated_v *= d->bondLength/mod;

              new_atom_pos = rotated_v + downPos;
            }
          }
          break;
        case 2:
          {
            Atom *n1 = at1 ->neighbours()[0];
            Atom * n2 = at1 ->neighbours()[1];
            QPointF v1 = n1 ->pos ();
            QPointF v2 = n2 ->pos ();
            QPointF v3 = v1 + v2;
            QPointF v4 (v3.x () / 2, v3.y () / 2);
            QPointF v5 =  at1 ->pos () - v4;
            qreal mod = sqrt (v5.x()*v5.x() + v5.y()*v5.y());
            v5 = QPointF (v5.x()/mod * d->bondLength, v5.y()/mod * d->bondLength);
            new_atom_pos = v5 + at1->pos();
          }
          break;
        default:
          break;
      }
      if (new_atom_pos != downPos) {
        stack->beginMacro("Add Bond");
        Atom* atom = new Atom(new_atom_pos,d->periodicTable->currentElement(),d->autoAddHydrogen);
        stack->push(new Commands::AddAtom(atom,at1 ->molecule()));
        if (d->bondType->backward()) qSwap(at1, atom);
        Bond* bond = new Bond(at1,atom);
        stack->push(new Commands::AddBond(bond));
        stack->push(new Commands::SetBondType(bond, d->bondType->bondType()));
        stack->endMacro();
      }
    }
  }

  void drawAction::refreshIcon()
  {// TODO redo
    QPixmap elementIcon = d->periodicTable->currentIcon(),
        bondIcon = d->bondType->bondIcon();
    QSize elementSize = elementIcon.size();
    QSize bondSize = bondIcon.size();
    QSize totalSize(bondSize + elementSize);
    totalSize.setWidth(qMax(totalSize.width(), totalSize.height()));
    totalSize.setHeight(totalSize.width());
    QPixmap pixmap(totalSize);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.drawPixmap(0, elementSize.height(), bondIcon);
    painter.drawPixmap(bondSize.width(), 0, elementIcon);
    setIcon(pixmap);
    /*
    QIcon bondIcon(d->bondType->bondIcon()) ;
    QString element = d->periodicTable->currentElement();
    int height = 0, width = 0;
    foreach (const QSize& size, bondIcon.availableSizes())
    {
      if (size.height() > height)
      {
        height = size.height();
        width  = size.width();
      }
    }
    QFont atomFont(font()); // TODO measuring is not quite right
    atomFont.setPixelSize(height);
    QFontMetrics metrics(atomFont);
    QRectF atomRect(QFontMetrics(atomFont).boundingRect(element));
    QImage compoundIcon(width + atomRect.width(), qMax((qreal) height, atomRect.height()), QImage::Format_RGB888);
    compoundIcon.fill(parentWidget() ? parentWidget()->palette().window().color() : Qt::white);
    QPainter painter(&compoundIcon);
    bondIcon.paint(&painter,0,.5*(compoundIcon.height()-height),width,height);
    painter.setFont(atomFont);
    //    painter.drawText(width,atomRect.height(),element);
    atomRect.moveTopLeft(QPointF(width,0));
    qDebug() << "AtomRect" << atomRect << atomRect.height()<< width << compoundIcon.size();
    painter.drawText(atomRect, element);
    setIcon(QPixmap::fromImage(compoundIcon));*/
  }

  void drawAction::toggleVisibility(bool visible)
  {
    if (visible) d->dock->show();
    else d->dock->hide();
  }

} // namespace Molsketch

