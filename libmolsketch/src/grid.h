#ifndef MOLSKETCH_GRID_H
#define MOLSKETCH_GRID_H

#include <QGraphicsItem>



namespace Molsketch {

  class grid : public QGraphicsItem
  {
  public:
    grid();
    ~grid();
    virtual QPointF alignPoint(const QPointF& point);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;
    void setHorizontalInterval(qreal h);
    void setVerticalInterval(qreal v);
    qreal horizontalInterval() const;
    qreal verticalInterval() const;
  private:
    class privateData;
    privateData *d;
  };

} // namespace Molsketch

#endif // MOLSKETCH_GRID_H
