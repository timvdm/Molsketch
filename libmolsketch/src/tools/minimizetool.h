#ifndef MSK_MINIMIZETOOL_H
#define MSK_MINIMIZETOOL_H

#include "../tool.h"

#include <QGraphicsItemGroup>

namespace Molsketch {

  class Molecule;
  class Bond;

  class MinimizeTool : public QObject, public Tool
  {
    Q_OBJECT

    public:
      MinimizeTool(MolScene *scene);
      virtual ~MinimizeTool();

      QList<QAction*> actions();
      QString toolbarName(QAction *action);

      void mousePressEvent(QGraphicsSceneMouseEvent *event);


    public slots:
      void actionClicked();

    private:
      void mirrorBondInMolecule (Molecule *mol, Bond *bo);
      void minimiseMolecule (Molecule *mol);

      // QActions
      QAction *m_action;

  };

}

#endif // MSK_MINIMIZETOOL_H
