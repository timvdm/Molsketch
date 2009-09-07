/***************************************************************************
 *   Copyright (C) 2007-2008 by Harm van Eersel                            *
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


#include "molview.h"

#include <QDebug>
#include <QGraphicsScene>
#include <QWheelEvent>

#include <math.h>

namespace Molsketch {

MolView::MolView(QGraphicsScene* scene) : QGraphicsView(scene)
{
	setContextMenuPolicy(Qt::ActionsContextMenu);
	setMouseTracking(true);
	setAcceptDrops(true);
	setRenderHints(QPainter::Antialiasing);
	setResizeAnchor(QGraphicsView::AnchorViewCenter);
	setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        setStyleSheet("");
}

void MolView::wheelEvent(QWheelEvent* event)
{
	scaleView(pow((double)2, -event->delta() / 240.0));
}

void MolView::scaleView(qreal scaleFactor)
{
	qreal factor = matrix().scale(scaleFactor, scaleFactor).mapRect(QRect(0,0,1,1)).width();
	if (factor < 0.07 || factor > 100) return;
	scale(scaleFactor, scaleFactor);
}


// void MolView::mouseMoveEvent(QMouseEvent * e)
// {
//     if(e->buttons() == Qt::MidButton && !scene()->itemAt(mapToScene(e->pos())))
//         translate(-e->x(), -e->y());
//     e->ignore();
// }

}
