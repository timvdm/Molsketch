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
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "TextInputItem.h"
#include "molscene.h"
#include <iostream>
#include <QKeyEvent>
#include <QPainter>

namespace Molsketch {



TextInputItem::TextInputItem (QGraphicsItem *parent): QGraphicsTextItem ( parent ), _atom (NULL)
	{
		setZValue(1); //draw on top
		setFlag(QGraphicsItem::ItemIsMovable, false);
		setTextInteractionFlags (Qt::TextEditable);
		setFlag(QGraphicsItem::ItemIsSelectable);
		setTextInteractionFlags(Qt::TextEditorInteraction);
	}


	void TextInputItem::applyString () {
		if (_atom) {
			_atom ->setElement (toPlainText());
			_atom ->hoverOut();
			_atom  ->show ();
			hide ();
		}
	}
	void TextInputItem::focusOutEvent ( QFocusEvent * event ) {
	//	std::cerr << " focuse" << std::endl;
		if (_atom) {
		_atom ->hoverOut();
		_atom ->show();
		hide ();
		}
		QGraphicsItem::focusOutEvent ( event );
	}
	
	void TextInputItem::clickedOn (Atom *at) {		
		setAtom (at);
		setPos (at ->mapToScene (at ->boundingRect ().topLeft ()));

		setPlainText(at ->string ());
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
		switch (event->key()) {
			case Qt::Key_Return:
				applyString ();
				break;
			default :
				break;
		}

		
		QGraphicsTextItem::keyPressEvent (event);
	}


void TextInputItem::setAtom (Atom *at) {
	_atom = at;
}
}//namespace
