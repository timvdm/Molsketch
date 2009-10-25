/***************************************************************************
 *   Copyright (C) 2009 Tim Vandermeersch                                  *
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
#include "graphsymitem.h"
#include "molscene.h"
#include "mimemolecule.h"

#include <QPainter>
#include <QGraphicsSceneDragDropEvent>
#include <QDebug>


namespace Molsketch {

  MolInputItem::MolInputItem(OutputType output) : m_output(output), m_molecule(0), m_rect(QRectF(0, 0, 150, 100))
  {
    setAcceptDrops(true);
    setFlags(QGraphicsItem::ItemIsMovable|QGraphicsItem::ItemIsSelectable);
  }

  QRectF MolInputItem::boundingRect() const
  {
    if (m_molecule) {
      QRectF molRect = m_molecule->boundingRect();
      return QRectF(mapFromItem(m_molecule, molRect.topLeft()), mapFromItem(m_molecule, molRect.bottomRight())) | m_rect;
    } else
      return defaultBoundingRect();
  }

  QPainterPath MolInputItem::shape() const
  {
    QPainterPath path;
    if (m_molecule)
      path.addRect(m_rect);
    else
      path.addRect(defaultBoundingRect());
    return path;
  }

  void MolInputItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
  {
    if (m_molecule) {

      QFontMetrics fm = painter->fontMetrics();
      m_rect = QRectF(0, 0, fm.width(label()), fm.height());
      painter->drawText(m_rect, Qt::AlignCenter | Qt::TextDontClip, label());
    } else {
      paintDefault(painter);
    }
  }

  void MolInputItem::dropEvent(QGraphicsSceneDragDropEvent *event)
  {
    const MimeMolecule *mimeMol = dynamic_cast<const MimeMolecule*>(event->mimeData());
    if (!mimeMol)
      return;

    m_molecule = mimeMol->molecule();
    QRectF rect = m_molecule->boundingRect();
    setPos(rect.bottomLeft());

//    m_molecule->addToGroup(this);

    if (scene())
      scene()->update();
  }


}
