/***************************************************************************
 *   Copyright (C) 2015 Hendrik Vennekate                                  *
 *      --- in part based on code originally written by: ---               *
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
#include "arrow.h"
#include <QPen>
#include <QPainter>
#include <QGraphicsSceneHoverEvent>
#include <QDialog>
#include <QDebug>
#include <QUndoCommand>
#include <QMenu>

#include "molscene.h"
#include "math2d.h"
#include "actions/arrowtypeaction.h"

namespace Molsketch {

  class ArrowDialog : public QDialog
  {
  public:
    ArrowDialog(Arrow* parent){}
  };

  struct Arrow::privateData
  {
    Arrow::ArrowType arrowType;
    QVector<QPointF> points;
    bool spline ;
    ArrowDialog *dialog;
  };

  Arrow::Arrow()
    : graphicsItem(),
      d(new privateData)
  {
    d->arrowType = LowerBackward | UpperBackward ;
    d->points << QPointF(0,0) << QPointF(50.0, 0.0),
    d->dialog = 0 ;
    d->spline = true ;
  }

  Arrow::~Arrow()
  {
    if (d->dialog)
      d->dialog->deleteLater() ;
    delete d ;
  }

  void Arrow::setArrowType(const ArrowType &type)
  {
    d->arrowType = type ;
  }

  Arrow::ArrowType Arrow::getArrowType() const
  {
    return d->arrowType;
  }

  void Arrow::setCoordinates(const QVector<QPointF> &c)
  {
    d->points = c ;
  }

  QPolygonF Arrow::coordinates() const
  {
    return d->points ;
  }

  inline QPainterPath generateArrowTip(const QPointF& target,
                                       const QPointF& origin,
                                       const QPointF& translate,
                                       bool up,
                                       bool down,
                                       qreal scaling)
  {
    QPainterPath path ;
    QPointF line(normalized(target - origin) * scaling) ;
    QPointF orthogonal(line.y(), -line.x()) ;
    path.moveTo(target) ;
    if (up) path.lineTo(target - 15*line + 5*orthogonal);
    path.lineTo(target - 12*line);
    if (down) path.lineTo(target - 15*line - 5*orthogonal);
    path.lineTo(target);
    path.translate(-translate);
    return path ;
  }

  void Arrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
  {
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if (d->points.size() < 2) return ;

    // draw the bounding rect if the arrow is selected
    if (isSelected() /* && !m_hoverBegin && !m_hoverEnd */) {
      painter->save();
      painter->setPen(Qt::blue);
      painter->drawRect(boundingRect());
      painter->restore();
    }

    if (d->points.isEmpty()) return ;

    // prepare pen and brush
    painter->save();
    QPen pen;
    pen.setWidthF(lineWidth()) ;
    pen.setCapStyle(Qt::RoundCap) ;
    pen.setJoinStyle(Qt::RoundJoin) ;
    pen.setColor(getColor()) ;
    painter->setPen(pen) ;

    QPainterPath path ;
    // draw the line
    path.moveTo(d->points.first()) ;
    if (d->spline && !((d->points.size()-1) % 3))
    {
      for (int i = 1 ; i+2 < d->points.size() ; i += 3) // TODO: alternatively: straight connection
        path.cubicTo(d->points[i],
                     d->points[i+1],
            d->points[i+2]);
      if (isSelected()) // Draw help lines
      {
        painter->save();
        painter->setPen(Qt::gray) ;
        QPointF previous(d->points.first()) ;
        for (int i = 1 ; i+2 < d->points.size() ; i += 3)
        {
          painter->drawLine(previous, d->points[i]) ;
          painter->drawLine(d->points[i+1], d->points[i+2]);
          previous = d->points[i+2] ;
        }
        painter->restore();
      }
    }
    else
      foreach(const QPointF p, d->points.mid(1))
        path.lineTo(p) ;

    path.translate(-pos());
    painter->drawPath(path) ;

    // draw arrow tips
    painter->setBrush(pen.color());
    if ((UpperBackward | LowerBackward) & d->arrowType)
      painter->drawPath(generateArrowTip(d->points.last(),
                                         d->points[d->points.size()-2],
                        pos(),
                        UpperBackward & d->arrowType,
                        LowerBackward & d->arrowType,
                        relativeWidth()
                        ));
    if ((UpperForward | LowerForward) & d->arrowType)
      painter->drawPath(generateArrowTip(d->points.first(),
                                         d->points[1],
                        pos(),
                        LowerForward & d->arrowType,
                        UpperForward & d->arrowType,
                        relativeWidth()
                        )) ;

    // draw red circles when hovering above one of the points
    painter->setPen(Qt::red);
    painter->setBrush(QBrush());
    if (selectedPoint() >= 0 && selectedPoint() < d->points.size())
        painter->drawEllipse(d->points[selectedPoint()], 5,5) ;
    painter->restore();
  }

  QRectF Arrow::ownBoundingRect() const
  {
    QRectF result ;
    foreach(const QPointF& p, d->points)
      result |= QRectF(p,QSizeF(1,1)) ;
    return result.adjusted(-10,-10,10,10) ;
  }

  void Arrow::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
  {
    Q_UNUSED(event)
    if (!d->dialog) d->dialog = new ArrowDialog(this) ;
    d->dialog->show() ;
  }

  void Arrow::setPoint(const int &index, const QPointF &p)
  {
    if (index == d->points.size()) setPos(p) ;
    if (index > d->points.size() || index < 0) return ;
    d->points[index] = p ;
  }

  void Arrow::setPoints(const QPolygonF &polygon)
  {
    d->points = polygon;
  }

  QPointF Arrow::getPoint(const int &index) const
  {
    if (index == d->points.size()) return pos() ;
    if (index > d->points.size() || index < 0) return QPointF() ;
    return d->points[index] ;
  }

  QPointF Arrow::lastPoint() const
  {
    if (d->points.isEmpty()) return QPointF();
    return d->points.last();
  }

  QPointF Arrow::firstPoint() const
  {
    if (d->points.isEmpty()) return QPointF();
    return d->points.first();
  }

  int Arrow::coordinateCount() const
  {
    return d->points.size() + 1 ;
  }

  void Arrow::swapPoint(const int &index, QPointF &p)
  {
    if (index == d->points.size())
    {
      QPointF t = pos() ;
      setPos(p) ;
      p = t ;
      return ;
    }
    if (index >= 0 && index < d->points.size())
      qSwap(d->points[index], p) ;
  }

  QString Arrow::xmlName() const
  {
    return "arrow";
  }

  void Arrow::readGraphicAttributes(const QXmlStreamAttributes &attributes)
  {
    d->arrowType = (ArrowType) (attributes.value("arrowType").toString().toInt()) ;
  }

  QXmlStreamAttributes Arrow::graphicAttributes() const
  {
    QXmlStreamAttributes attributes ;
    attributes.append("arrowType", QString::number(d->arrowType)) ;
    return attributes ;
  }

  void Arrow::prepareContextMenu(QMenu *contextMenu)
  {
    MolScene *sc = qobject_cast<MolScene*>(scene());
    if (sc)
    {
      arrowTypeAction *action = sc->findChild<arrowTypeAction*>();
      if (action)
      {
        contextMenu->addAction(action);
        QObject::connect(action, SIGNAL(triggered()), contextMenu, SLOT(close()));
      }
    }
    graphicsItem::prepareContextMenu(contextMenu);
  }

  qreal Arrow::sceneLineWidth(MolScene *scene) const
  {
    return scene->arrowWidth();
  }

  // TODO (maybe) highlight points if covered by other bounding rect (probably in scene class)
  // TODO shift responsibility for move to item level, remove move tool
  // TODO move as whole
  // TODO funny when moved while other tool active
  // TODO funny when first or last point moved
  // TODO change arrow tip, coords -> undo
} // namespace
