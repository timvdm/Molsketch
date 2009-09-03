#ifndef SMILESITEM_H
#define SMILESITEM_H

#include <QGraphicsItem>

namespace Molsketch {

  class Molecule;

  class SmilesItem : public QGraphicsItem
  {
    public:
      SmilesItem();

      QRectF boundingRect() const;
      void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

      void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
      void dragMoveEvent(QGraphicsSceneDragDropEvent *event);
      void dragLeaveEvent(QGraphicsSceneDragDropEvent *event);

      void dropEvent(QGraphicsSceneDragDropEvent *event);
    private:
      Molecule *m_molecule;
      QRectF m_rect;

  };

}

#endif // SMILESITEM_H
