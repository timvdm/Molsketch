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

#include "atomnumberitem.h"
#include "molscene.h"
#include "mimemolecule.h"

#include <QPainter>
#include <QGraphicsSceneDragDropEvent>
#include <QDebug>

namespace Molsketch {

  AtomNumberItem::AtomNumberItem() : m_molecule(0), m_rect(QRectF(0, 0, 150, 100))
  {
    setAcceptDrops(true);
    setFlags(QGraphicsItem::ItemIsMovable|QGraphicsItem::ItemIsSelectable);
  }

  QRectF AtomNumberItem::boundingRect() const
  {
    if (m_molecule)
      return m_rect;
    else
      return defaultBoundingRect();
  }

  void AtomNumberItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
  {
    painter->save();
    painter->setPen(Qt::blue);
    if (m_molecule) {
      const QList<Atom*> &atoms = m_molecule->atoms();
      QFontMetrics fm = painter->fontMetrics();

      QPointF offset(0.0, fm.height() - fm.descent());
      for (int i = 0; i < atoms.size(); ++i) {
        painter->drawText(mapFromItem(m_molecule, atoms[i]->pos()) + offset, QString::number(i+1));
      }

      m_rect = QRectF(0, 0, fm.width(QObject::tr("Atom Numbers")), fm.height());
      painter->drawText(m_rect, Qt::AlignCenter | Qt::TextDontClip, QObject::tr("Atom Numbers"));
    } else {
      paintDefault(painter);
    }
    painter->restore();
  }

  void AtomNumberItem::dropEvent(QGraphicsSceneDragDropEvent *event)
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


}
