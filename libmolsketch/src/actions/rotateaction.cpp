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
#include "rotateaction.h"
#include "QUndoCommand"
#include "molscene.h"
#include "transformcommand.h"

#include <QGraphicsSceneMouseEvent>
#include <QToolTip>

namespace Molsketch {

  struct transformAction::privateData
  {
    graphicsItem *transformItem;
    QLineF originalLine ;
  };

  transformAction::transformAction(MolScene *scene) :
    genericAction(scene),
    d(new privateData)
  {
    d->transformItem = 0;
  }

  void transformAction::mousePressEvent(QGraphicsSceneMouseEvent *event)
  {
    if (event->button() != Qt::LeftButton) return ;
    QGraphicsItem * item = scene()->itemAt(event->buttonDownScenePos(Qt::LeftButton)
                                       #if QT_VERSION >= 0x050000
                                           , QTransform()
                                       #endif
                                           );
    if (!item) return;

    if (item->type() == graphicsItem::AtomType
        || item->type() == graphicsItem::BondType)
      item = item->parentItem();
    d->transformItem = dynamic_cast<graphicsItem*>(item) ;
    if (!d->transformItem) return;

    d->originalLine = QLineF(item->mapToScene(item->boundingRect().center()),
                            event->buttonDownScenePos(Qt::LeftButton)) ;
    event->accept();
  }

  void transformAction::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
  {
    if (!d->transformItem) return;

    // Calculate the rotation angle
    QLineF newLine(d->originalLine.p1(), event->scenePos()) ;
    if (event->modifiers() & Qt::ShiftModifier)
      snapLine(QLineF(d->originalLine.p1(), event->buttonDownScenePos(Qt::LeftButton)), newLine);

    QToolTip::showText(event->screenPos(),
                       cursorLabel(QLineF(d->originalLine.p1(), event->buttonDownScenePos(Qt::LeftButton)), newLine),
                       parentWidget(),
                       QRect());
    transformCommand *cmd = new transformCommand(d->transformItem,
                                                 generateTransform(d->originalLine,
                                                                   newLine), // TODO doesn't work
                                                 d->originalLine.p1());
    cmd->setText(text());
    scene()->stack()->push(cmd) ;
    d->originalLine = newLine ;
    event->accept();
  }

  void transformAction::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
  {
    Q_UNUSED(event)
    d->transformItem = 0 ;
    event->accept();
  }

  QString transformAction::cursorLabel(const QLineF &originalLine, QLineF &currentLine)
  {
    Q_UNUSED(originalLine)
    Q_UNUSED(currentLine)
    return QString();
  }

  rotateAction::rotateAction(MolScene *scene)
    : transformAction(scene)
  {
    setIcon(QIcon(":images/rotate.png")) ;
    setText(tr("Rotate")) ;
  }

  void rotateAction::snapLine(const QLineF &originalLine, QLineF &currentLine)
  {
    const qreal step = 30;
    qreal angle = step*qRound(currentLine.angleTo(originalLine)/step);
    currentLine.setAngle(originalLine.angle()-angle);
  }

  QString rotateAction::cursorLabel(const QLineF &originalLine, QLineF &currentLine)
  {
    return QString::number(currentLine.angleTo(originalLine)) + "°";
  }

  QTransform rotateAction::generateTransform(const QLineF &originalLine, const QLineF &currentLine)
  {
    return QTransform().rotate(currentLine.angleTo(originalLine));
  }

  translateAction::translateAction(MolScene *scene)
    : transformAction(scene)
  {
    setIcon(QIcon(":images/translate.png")) ;
    setText(tr("Move")) ;
  }

  QTransform translateAction::generateTransform(const QLineF &originalLine, const QLineF &currentLine)
  {
    QPointF diff = currentLine.p2() - originalLine.p2() ;
    return QTransform().translate(diff.x(), diff.y()) ;
  }

  void translateAction::snapLine(const QLineF& originalLine, QLineF &currentLine)
  {
    Q_UNUSED(originalLine)
    Q_UNUSED(currentLine)
    // TODO
  }

} // namespace Molsketch
