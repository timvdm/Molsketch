/***************************************************************************
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

#include "TextInputItem.h"
#include "molscene.h"
#include <iostream>
#include <QKeyEvent>
#include <QPainter>
#include "commands.h"

namespace Molsketch {



  TextInputItem::TextInputItem (QGraphicsItem *parent): QGraphicsTextItem ( parent ), _atom (NULL)
  {
    setZValue(4); //draw on top
    setFlag(QGraphicsItem::ItemIsMovable, false);
    setTextInteractionFlags (Qt::TextEditable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setTextInteractionFlags(Qt::TextEditorInteraction);
  }


  void TextInputItem::applyString () {
    if (!_atom) return;
    MolScene *mscene = dynamic_cast<MolScene*>(scene());
    if (!mscene) return;
    Commands::ChangeElement *ceCommand = new Commands::ChangeElement(_atom, toPlainText());
    if (!mscene->stack())
    {
      ceCommand->redo();
      delete ceCommand;
    }
    else
      mscene->stack()->push(ceCommand);
  }

  void TextInputItem::cleanUp()
  {
    if (_atom)
    {
//      _atom->hoverOut();
      _atom->show();
    }
    _atom = 0;
    if (scene()) scene()->removeItem(this);
  }

  void TextInputItem::focusOutEvent ( QFocusEvent * event ) {
    applyString();
    cleanUp();
    QGraphicsTextItem::focusOutEvent ( event );
  }

  void TextInputItem::clickedOn (Atom *at) {
    setAtom (at);
    setPos (at->scenePos()); // TODO fix
    setPlainText(at ->element());
    if (scene()) setFont(scene()->font());
    at ->hide ();
    show ();
    setSelected(true);
    setFocus ();
  }

  void TextInputItem::paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget ) {
    painter ->save ();
    painter ->setBrush (Qt::white);

    painter ->setPen (Qt::white);
    painter ->setBackgroundMode (Qt::OpaqueMode);
    painter ->setBackground (Qt::white);
    painter->drawRect(boundingRect());
    painter->restore();

    QGraphicsTextItem::paint (painter, option, widget);
  }

  void TextInputItem::keyPressEvent ( QKeyEvent * event ) {
    QGraphicsTextItem::keyPressEvent (event);
    if (event->isAccepted()) return;
    switch (event->key()) {
      case Qt::Key_Escape:
        cleanUp();
        clearFocus();
        event->accept();
        break;
      case Qt::Key_Return:
        clearFocus();
        event->accept();
      default: ;
    }

  }


  void TextInputItem::setAtom (Atom *at) {
    _atom = at;
  }
}//namespace
