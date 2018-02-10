/***************************************************************************
 *   Copyright (C) 2017 by Hendrik Vennekate                               *
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
#include "textaction.h"

#include <QGraphicsSceneMouseEvent>
#include <commands.h>
#include <textitem.h>

namespace Molsketch {

  struct TextAction::PrivateData {

  };

  TextAction::TextAction(MolScene *scene)
    : genericAction(scene),
      d(new PrivateData)
  {
    setIcon(QIcon::fromTheme("insert-text", QIcon(":icons/insert-text.svg")));
    setText(tr("Insert text"));
  }

  TextAction::~TextAction()
  {
    delete d;
  }

  void TextAction::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->modifiers() != Qt::NoModifier) return; // TOO extract function?
    if (event->button() != Qt::LeftButton) return;
    event->accept();
  }

  // TODO test do we need mouse leave?

  void TextAction::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (event->modifiers() != Qt::NoModifier) return;
    if (event->button() != Qt::LeftButton) return;
    event->accept();
  }

  void TextAction::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    if (event->modifiers() != Qt::NoModifier) return;
    if (event->button() != Qt::LeftButton) return;
    event->accept();
    TextItem *item = new TextItem;
    item->setPos(event->scenePos());
    Commands::ItemAction::addItemToScene(item, scene(), tr("Add text"));
    item->setFocus();
  }

} // namespace Molsketch
