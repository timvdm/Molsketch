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
#include "smilesitem.h"
#include "molscene.h"
#include "mimemolecule.h"

#include <QPainter>
#include <QGraphicsSceneDragDropEvent>
#include <QDebug>

namespace Molsketch {

  SmilesItem::SmilesItem() : ItemPlugin(), m_molecule(0), m_rect(QRectF(0, 0, 150, 100))
  {
  }

  SmilesItem::~SmilesItem()
  {
  }

  QRectF SmilesItem::boundingRect() const
  {
    if (m_molecule)
      return m_rect;
    else
      return defaultBoundingRect();
  }

  void SmilesItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
  {
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if (m_molecule) {
      QFontMetrics fm = painter->fontMetrics();
      QString smiles = m_molecule->smiles();
      m_rect = QRectF(0, 0, fm.width(smiles), fm.height());
      painter->drawText(m_rect, Qt::AlignCenter | Qt::TextDontClip, smiles);
    } else {
      paintDefault(painter);
    }
  }

  void SmilesItem::dropEvent(QGraphicsSceneDragDropEvent *event)
  {
    const MimeMolecule *mimeMol = dynamic_cast<const MimeMolecule*>(event->mimeData());
    if (!mimeMol)
      return;

    m_molecule = mimeMol->molecule();
    QRectF rect = m_molecule->boundingRect();
    setPos(rect.bottomLeft());

    m_molecule->addToGroup(this);

    if (scene())
      scene()->update();
  }

  SmilesItemFactory::SmilesItemFactory()
  {
    instanceList().append(this);
    qDebug() << instanceList().size();
  }

  SmilesItemFactory theSmilesItemFactory;

}

