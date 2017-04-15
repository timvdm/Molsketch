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
#include "atom.h"
#include "molecule.h"

#include <QPainter>
#include <QDebug>


namespace Molsketch {

  MolInputItem::MolInputItem(OutputType output) : m_output(output), m_rect(QRectF(0, 0, 150, 100))
  {
    setAcceptDrops(true);
    setFlags(QGraphicsItem::ItemIsMovable|QGraphicsItem::ItemIsSelectable);
  }

  Molecule *MolInputItem::molecule() const
  {
     return dynamic_cast<Molecule*>(parentItem());
  }

  QRectF MolInputItem::boundingRect() const
  {
    if (molecule()) {
      return QRectF() ; // TODO
    } else
      return defaultBoundingRect();
  }

  QPainterPath MolInputItem::shape() const
  {
    QPainterPath path;
    if (molecule())
      path.addRect(m_rect);
    else
      path.addRect(defaultBoundingRect());
    return path;
  }

  void MolInputItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
  {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    if (molecule()) {

      QFontMetrics fm = painter->fontMetrics();
      m_rect = QRectF(0, 0, fm.width(label()), fm.height());
      painter->drawText(m_rect, Qt::AlignCenter | Qt::TextDontClip, label());
    } else {
      paintDefault(painter);
    }
  }

  QPointF getRectangleIntersectionVector(const QRectF& rectangle, qreal angle)
  {
    if (!rectangle.isValid()) return QPointF(0,0) ;
    QLineF diagonal(rectangle.center(), rectangle.topRight()) ;
    diagonal.setLength(diagonal.length()*1.5);
    diagonal.setAngle(angle);
    QVector<QLineF> sides ;
    sides << QLineF(rectangle.bottomRight(), rectangle.topRight())
          << QLineF(rectangle.topRight(), rectangle.topLeft())
          << QLineF(rectangle.topLeft(), rectangle.bottomLeft())
          << QLineF(rectangle.bottomLeft(), rectangle.bottomRight()) ;
    QPointF interPoint ;
    foreach(const QLineF side, sides)
      if (QLineF::BoundedIntersection == side.intersect(diagonal, &interPoint))
        return interPoint - rectangle.center() ;
    return QPointF() ;
  }

  void MolInputItem::drawTextNearAtom(QPainter *painter, Atom *atom, const QString &text, const Molecule* mol)
  {
    QRectF textRect(painter->fontMetrics().boundingRect(text)) ;
    qreal direction = atom->annotationDirection() ;
    QPointF offset(-textRect.center()) ;
    offset += 1.5*getRectangleIntersectionVector(textRect, direction) ;

    if (atom->isDrawn())
      offset += 1.5*getRectangleIntersectionVector(atom->boundingRect(), direction) ;

    painter->drawText(mapFromItem(mol, atom->pos()) + offset, text);
  }


}
