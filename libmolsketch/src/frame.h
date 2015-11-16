#include "graphicsitem.h"

#ifndef MOLSKETCH_FRAME_H
#define MOLSKETCH_FRAME_H

namespace Molsketch {

  class frame : public graphicsItem
  {
  private:
    class privateData;
    privateData *d;
  public:
    frame();
    ~frame();
    void setCoordinates(const QVector<QPointF> &c);
    void setCoordinates(const QPointF& topLeft, const QPointF& bottomRight);
    QPolygonF coordinates() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    int coordinateCount() const;

    QString xmlName() const;
  protected:
    void readGraphicAttributes(const QXmlStreamAttributes &attributes);
    QXmlStreamAttributes graphicAttributes() const;
    void prepareContextMenu(QMenu *contextMenu);
  private:
    qreal sceneLineWidth(MolScene *scene) const;
  };

} // namespace Molsketch

#endif // MOLSKETCH_FRAME_H
