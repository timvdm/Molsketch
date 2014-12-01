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
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef RESIDUE_H
#define RESIDUE_H

#include "graphicsitemtypes.h"

#include <QGraphicsItem>
#include <QList>
#include <qgraphicsscene.h>


namespace Molsketch {
		
		/**
		 * Residue class to disply protein residues
		 */
		class Residue : public QGraphicsItem
			{
				
			public:
				/**
				 * Creates a new residue.
				 *
				 * @param position the position of the new residue
				 * @param residue the residue string to be displayed.
				 */
				Residue(const QPointF & position, const QString & residue, 
					  QGraphicsItem* parent = 0
		#if QT_VERSION < 0x050000
				      , QGraphicsScene *scene = 0
		#endif
);
				
				void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
				QRectF boundingRect() const;
				QString residueString;
				void setColor (QColor col) {m_color = col;}
				QColor getColor () {return m_color;}
				
        enum { Type = GraphicsItemTypes::ResidueType };
				/** Returns the type of the class. Needed fro Qt typecasting. */
				virtual int type() const {return Residue::Type;};
				
				private :
				QColor m_color;
			};	
				
} // namespace

#endif // RESIDUE_H
