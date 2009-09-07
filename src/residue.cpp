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

#include "residue.h"
#include <QPainter>


namespace Molsketch {
	Residue::Residue (const QPointF & position, const QString & residue, 
					  QGraphicsItem* parent, QGraphicsScene* scene)  : QGraphicsItem (parent,scene), residueString (residue) {
    //pre: position is a valid position in scene coordinates
    setPos(position);
	setColor (QColor (0, 0, 255));
		setFlag(QGraphicsItem::ItemIsSelectable);

	}
	
	
	void Residue::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
	//	MolScene* molScene = dynamic_cast<MolScene*>(scene());
	//	Q_CHECK_PTR(molScene);
		
		painter->save();
		QRadialGradient gradient(0, 0, 40, 0, 0);
		gradient.setColorAt(0, m_color);
		gradient.setColorAt(1, QColor (0,0,0));
		
        painter->setBrush(gradient);
		
		painter->drawEllipse(-30, -30, 60, 60);
		painter ->setPen (Qt::white);
		painter ->drawText(boundingRect(), Qt::AlignCenter, residueString);
		
        painter->restore();
		
	}
	QRectF Residue::boundingRect() const
	{

			return QRectF(-30, -30, 60,60);
		
	}
	
} // namespace
