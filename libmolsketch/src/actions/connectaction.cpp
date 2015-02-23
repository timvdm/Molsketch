/***************************************************************************
 *   Copyright (C) 2015 Hendrik Vennekate                                  *
 *      --- based on code originally written by: ---                       *
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
#include "connectaction.h"
#include "molecule.h"
#include "mimemolecule.h"
#include "molscene.h"
#include <QDrag>
#include <QGraphicsSceneMouseEvent>

namespace Molsketch {

  connectAction::connectAction(MolScene *scene) :
    genericAction(scene)
  {
    setIcon(QIcon(":images/connect.png"));
    setText(tr("Connect to plugin")) ;
  }

  void connectAction::mousePressEvent(QGraphicsSceneMouseEvent *event)
  {
    Molecule *mol = scene()->moleculeAt(event->buttonDownScenePos(event->button()));
    if (!mol) return ;

    QImage molImg = scene()->renderMolToImage(mol);

    MimeMolecule *data = new MimeMolecule;
    data->setMolecule(mol);
    data->setImageData(molImg);

    QDrag *drag = new QDrag(event->widget());
    drag->setMimeData(data);
    drag->setPixmap(QPixmap::fromImage(molImg));
    drag->start();
  }

  // TODO use undo stack
} // namespace
