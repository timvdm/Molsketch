#include "frame.h"

#include "molscene.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>

namespace Molsketch {

  class frame::privateData
  {
  public:
    QPointF topLeft, bottomRight, offset;
    bool checkingBoundingRect;
    void refreshCoords(QGraphicsItem* parent)
    {
      if (!parent) return;
      checkingBoundingRect = true;
      topLeft = parent->boundingRect().topLeft();
      bottomRight = parent->boundingRect().bottomRight();
      topLeft -= offset;
      bottomRight += offset;
      checkingBoundingRect = false;
    }
  };

  frame::frame()
    : d(new privateData)
  {
    d->offset = QPointF(10,10);
    d->checkingBoundingRect = false;
  }

  frame::~frame()
  {
    delete d;
  }

  void frame::setCoordinates(const QVector<QPointF> &c)
  {
    if (c.size() != 2) return;
    setCoordinates(c[0], c[1]);
  }

  void frame::setCoordinates(const QPointF &topLeft, const QPointF &bottomRight)
  {
    d->topLeft = topLeft;
    d->bottomRight = bottomRight;
  }

  QPolygonF frame::coordinates() const
  {
    return QPolygonF() << d->topLeft << d->bottomRight;
  }

  void frame::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
  {
    Q_UNUSED(option)
    Q_UNUSED(widget)
    d->refreshCoords(parentItem());

    painter->save();

    QPen pen; // TODO make function in graphicsItem for this
    pen.setWidthF(lineWidth());
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    pen.setColor(getColor());
    painter->setPen(pen);

    painter->drawRect(QRectF(d->topLeft, d->bottomRight));

    painter->restore();
  }

  QRectF frame::boundingRect() const
  {
    if (d->checkingBoundingRect) return QRectF();
    d->refreshCoords(parentItem());
    return QRectF(d->topLeft,d->bottomRight);
  }

  void frame::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
  {
    event->ignore();
  }

  int frame::coordinateCount() const
  {
    return 2;
  }

  QString frame::xmlName() const
  {
    return "frame";
  }

  void frame::readGraphicAttributes(const QXmlStreamAttributes &attributes)
  {
  }

  QXmlStreamAttributes frame::graphicAttributes() const
  {
  }

  void frame::prepareContextMenu(QMenu *contextMenu)
  {
  }

  qreal frame::sceneLineWidth(MolScene *scene) const
  {
    return scene->frameLinewidth();
  }


} // namespace Molsketch
