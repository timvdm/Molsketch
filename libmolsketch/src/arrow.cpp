#include "arrow.h"
#include <QPen>
#include <QPainter>
#include <QGraphicsSceneHoverEvent>
#include <QDialog>
#include <QDebug>
#include <QUndoCommand>

#include "molscene.h"
#include "math2d.h"

namespace Molsketch {

  class moveArrowPointCommand : public QUndoCommand
  {
  private:
    int index ;
    QPointF point ;
    Arrow *arrow ;
  public:
    moveArrowPointCommand(const int& i,
                          const QPointF& p,
                          Arrow* a,
                          QUndoCommand* parent = 0)
      : QUndoCommand(parent),
        index(i),
        point(p),
        arrow(a)
    {}
    void redo()
    {
      arrow->swapPoint(index, point) ;
    }
    void undo()
    {
      redo() ;
    }
    bool mergeWith(const QUndoCommand *other)
    {
      const moveArrowPointCommand* mapc = dynamic_cast<const moveArrowPointCommand*>(other) ;
      if (!mapc) return false ;
      if (mapc->arrow != arrow) return false ;
      if (mapc->index != index) return false ;
      return true ;
    }
    int id() const { return arrow->type(); } // TODO define command IDs in central place
  };

  class ArrowDialog : public QDialog
  {
  public:
    ArrowDialog(Arrow* parent){}
  };

  struct Arrow::privateData
  {
    Arrow::ArrowType arrowType;
    QVector<QPointF> points;
    int selectedPoint ;
    bool spline ;
    ArrowDialog *dialog;
  };

  Arrow::Arrow()
    : arrowGraphicsItem(),
      d(new privateData)
  {
    d->arrowType = LowerBackward | UpperBackward ;
    d->points << QPointF(0,0) << QPointF(50.0, 0.0),
    d->selectedPoint = -1 ;
    d->dialog = 0 ;
    d->spline = true ;

    setFlags(QGraphicsItem::ItemIsMovable|QGraphicsItem::ItemIsSelectable|QGraphicsItem::ItemIsFocusable);
#if QT_VERSION < 0x050000
    setAcceptsHoverEvents(true);
#else
    setAcceptHoverEvents(true) ;
#endif
  }

  Arrow::~Arrow()
  {
    if (d->dialog)
      d->dialog->deleteLater() ;
    delete d ;
  }

  void Arrow::setArrowType(Arrow::ArrowType type)
  {
    d->arrowType = type ;
  }

  void Arrow::setCoordinates(const QVector<QPointF> &c)
  {
    d->points = c ;
  }

  QVector<QPointF> Arrow::coordinates() const
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
      painter->drawPath(generateArrowTip(d->points.first(),
                        d->points[1],
                        pos(),
                        UpperBackward & d->arrowType,
                        LowerBackward & d->arrowType,
                        relativeWidth()
                        ));
    if ((UpperForward | LowerForward) & d->arrowType)
      painter->drawPath(generateArrowTip(d->points.last(),
                                         d->points[d->points.size()-2],
                        pos(),
                        LowerForward & d->arrowType,
                        UpperForward & d->arrowType,
                        relativeWidth()
                        )) ;

    // draw red circles when hovering above one of the points
    painter->setPen(Qt::red);
    painter->setBrush(QBrush());
    if (d->selectedPoint >= 0 && d->selectedPoint < d->points.size())
        painter->drawEllipse(d->points[d->selectedPoint], 5,5) ;
    painter->restore();
  }

  QRectF Arrow::boundingRect() const
  {
    QRectF result ;
    foreach(const QPointF& p, d->points)
      result |= QRectF(p,QSizeF(1,1)) ;
    return result.adjusted(-10,-10,10,10) ;
  }

  void Arrow::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
  {
    hoverMoveEvent(event) ; // TODO ???
  }

  void Arrow::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
  {
    d->selectedPoint = -1 ;
    QPointF eventPosition = event->pos() - scenePos() ;
    int i = 0 ;
    double minDistance = INFINITY ;

    foreach(const QPointF& p, coordinates())
    {
      qreal currentDistance = QLineF(eventPosition, p).length() ;
      if (currentDistance < minDistance
          && currentDistance < 5.)
      {
        minDistance = currentDistance ;
        d->selectedPoint = i ;
      }
      ++i ;
    }
    update() ;
//    if (d->selectedPoint >= 0) event->accept(); // TODO
//    arrowGraphicsItem::hoverMoveEvent(event) ;
  }

  void Arrow::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
  {
    Q_UNUSED(event)
    d->selectedPoint = -1 ;
    update() ;
  }

  void Arrow::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
  {
    if (d->selectedPoint >= 0)
    {
      attemptUndoPush(new moveArrowPointCommand(d->selectedPoint,
                                                event->pos(),
                                                this));
      return ;
    }

//    arrowGraphicsItem::mouseMoveEvent(event) ;
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

  QPointF Arrow::getPoint(const int &index)
  {
    if (index == d->points.size()) return pos() ;
    if (index > d->points.size() || index < 0) return QPointF() ;
    return d->points[index] ;
  }

  int Arrow::numPoints() const
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

#define POINTNAMEMACRO(POINTINDEX) "p" + QString::number(i)
  void Arrow::readGraphicAttributes(const QXmlStreamAttributes &attributes)
  {
    d->arrowType = (ArrowType) (attributes.value("arrowType").toInt()) ;
    QVector<QPointF> newCoords ;
    int i = 0 ;
    while (attributes.hasAttribute(POINTNAMEMACRO(i)+"x"))
      newCoords << QPointF(attributes.value(POINTNAMEMACRO(i)+"x").toDouble(),
                           attributes.value(POINTNAMEMACRO(i)+"y").toDouble()) ;
    setCoordinates(newCoords) ;
  }

  QXmlStreamAttributes Arrow::graphicAttributes() const
  {
    QXmlStreamAttributes attributes ;
    attributes.append("arrowType", QString::number(d->arrowType)) ;
    int i = 0 ;
    foreach(const QPointF& p, coordinates())
    {
      attributes.append(POINTNAMEMACRO(i) + "x", QString::number(p.x())) ;
      attributes.append(POINTNAMEMACRO(i) + "y", QString::number(p.y())) ;
      ++i;
    }
    return attributes ;
  }

  // TODO (maybe) highlight points if covered by other bounding rect (probably in scene class)
  // TODO shift responsibility for move to item level, remove move tool
  // TODO move as whole
  // TODO funny when moved while other tool active
  // TODO funny when first or last point moved
  // TODO change arrow tip, coords -> undo
} // namespace
