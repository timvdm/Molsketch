#ifndef GRAPHSYMITEM_H
#define GRAPHSYMITEM_H

#include <Molsketch/ItemPlugin>

namespace Molsketch {

  class Molecule;

  class GraphSymItem : public ItemPlugin
  {
    public:
      GraphSymItem();

      QString input() const { return "Molecule"; }
      QString output() const { return "Symmetry Classes"; }

      QRectF boundingRect() const;
      void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
      void dropEvent(QGraphicsSceneDragDropEvent *event);
    private:
      Molecule *m_molecule;
      QRectF m_rect;
  };

  ITEM_PLUGIN_FACTORY(GraphSymItem, "Molecule", "Symmetry Classes")

}

#endif // SMILESITEM_H
