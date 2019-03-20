/***************************************************************************
 *   Copyright (C) 2009 by Nicola Zonta                               *
 *                                                                      *
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
#ifndef TEXTINPUTITEM_H
#define TEXTINPUTITEM_H

#include "atom.h"

namespace Molsketch {
	

class TextInputItem : public QGraphicsTextItem {
public:
	TextInputItem (QGraphicsItem *parent = 0);
	void setAtom (Atom *at);
	void clickedOn (Atom *at);
	// Methods needed for qt typecasting
	/** Defines the type of the class. Needed for Qt typecasting.*/
  enum { Type = graphicsItem::TextInputType };
	/** Returns the type of the class. Needed fro Qt typecasting. */
        virtual int type() const override {return Type;}
	
protected:
        void paint (QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0) override;
        void focusOutEvent (QFocusEvent * event) override;
	void applyString ();
        void cleanUp();
        void keyPressEvent (QKeyEvent *event) override;
	Atom *_atom;
};
} //namespace

#endif //TEXTINPUTITEM_H
