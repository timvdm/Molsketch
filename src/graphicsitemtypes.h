#ifndef GRAPHICSITEMTYPES_H
#define GRAPHICSITEMTYPES_H

#include <QGraphicsItem>

namespace Molsketch {

  struct GraphicsItemTypes
  {
    enum Types {
      MoleculeType = QGraphicsItem::UserType + 1,
      AtomType = QGraphicsItem::UserType + 2,
      BondType = QGraphicsItem::UserType + 3,
      ResidueType = QGraphicsItem::UserType + 4,
      TextInputType = QGraphicsItem::UserType + 5,
      ReactionArrowType = QGraphicsItem::UserType + 6,
      MechanismArrowType = QGraphicsItem::UserType + 7
    };

  };

}

#endif // GRAPHICSITEMTYPES_H
