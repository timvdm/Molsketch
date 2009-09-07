#ifndef SMILESITEM_H
#define SMILESITEM_H

#include <Molsketch/ItemPlugin>

namespace Molsketch {

  class Molecule;

  class SmilesItem : public ItemPlugin
  {
    public:
      SmilesItem();
      virtual ~SmilesItem();

      QString input() const { return "Molecule"; }
      QString output() const { return "SMILES"; }

      QRectF boundingRect() const;
      void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

      void dropEvent(QGraphicsSceneDragDropEvent *event);

   private:
      Molecule *m_molecule;
      QRectF m_rect;
  };

  class SmilesItemFactory : public ItemPluginFactory
  {
    public:
      SmilesItemFactory();
      QString input() const { return "Molecule"; }
      QString output() const { return "SMILES"; }
      ItemPlugin* createInstance() const { return new SmilesItem; }
  };

}

#endif // SMILESITEM_H
