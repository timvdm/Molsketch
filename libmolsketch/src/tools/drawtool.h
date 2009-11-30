#ifndef MSK_DRAWTOOL_H
#define MSK_DRAWTOOL_H

#include "../tool.h"
#include "../molecule.h"
#include "../bond.h"

#include <QGraphicsItemGroup>

namespace Molsketch {

  class DrawTool : public QObject, public Tool
  {
    Q_OBJECT

    public:
      DrawTool(MolScene *scene);
      virtual ~DrawTool();

      QList<QAction*> actions();
      QString toolbarName(QAction *action);

      void deactivated();

      void mousePressEvent(QGraphicsSceneMouseEvent *event);
      void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
      void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    public slots:
      void hydrogenClicked();
      void carbonClicked();
      void nitrogenClicked();
      void oxygenClicked();
      void phosphorusClicked();
      void sulfurClicked();
      void fluorineClicked();
      void bromineClicked();
      void chlorineClicked();
      void iodineClicked();
      void rClicked();
      void xClicked();
      void anyClicked();

      void singleBondClicked();
      void doubleBondClicked();
      void tripleBondClicked();
      void hashBondClicked();
      void wedgeBondClicked();
      void wedgeOrHashBondClicked();
      void cisTransBondClicked();

      void cyclopropaneClicked();
      void cyclobutaneClicked();
      void cyclopentaneClicked();
      void cyclohexaneClicked();
      void ring7Clicked();
      void ring8Clicked();
      void aromatic5ringClicked();
      void aromatic6ringClicked();

    private:
      void initHintItems();
      /** Draws the dynamic grid around point @p curPos. */
      void drawHintPoints(const QPointF &curPos);
      /** Shows a hintline from @p startPos to @p curPos. */
      void setHintLine(const QPointF &startPos, const QPointF &curPos);
      /** Hides the hintline. */
      void removeHintLine();
      /** Hides the dynamic grid. */
      void removeHintPoints();
      void setHintRing(int ringSize, bool aromatic = false);

      /**
       * Make hint ring into real atoms/bonds
       */
      void insertRing(const QPointF &pos);
      /** Merges @p molA and @p molB. Used two merge two molecules when connected with a bond. */
      Molecule* merge(const Molecule* molA, const Molecule* molB);

      QPointF nearestPoint(const QPointF &curPos);

      void alignRingWithAtom(Atom *atom);
      void alignRingWithBond(Bond *bond, const QPointF &pos);

      QList<QAction*> m_actions;


      QGraphicsLineItem* m_hintLine;
      QList<QGraphicsItem*> m_hintPoints;
      QGraphicsItemGroup* m_hintPointsGroup;
      Molecule *m_hintMolecule;
      QGraphicsItemGroup *m_hintMoleculeItems;
      QList<QPointF> m_hintRingPoints;
      bool m_aromaticHintRing;
      Bond::BondType m_bondType;
      /** Stores the current bond order. */
      int m_bondOrder;
      bool m_autoAddHydrogen; //!< Stores whether hydrogens are to be added automaticly.
      QString m_currentElementSymbol;
      qreal m_bondAngle, m_bondLength;


      // QActions
      QAction *m_hydrogenAct;
      QAction *m_carbonAct;
      QAction *m_nitrogenAct;
      QAction *m_oxygenAct;
      QAction *m_phosphorusAct;
      QAction *m_sulfurAct;
      QAction *m_fluorineAct;
      QAction *m_chlorineAct;
      QAction *m_bromineAct;
      QAction *m_iodineAct;
      QAction *m_rAct;
      QAction *m_xAct;
      QAction *m_anyAct;
      QAction *m_singleBondAct;
      QAction *m_doubleBondAct;
      QAction *m_tripleBondAct;
      QAction *m_hashBondAct;
      QAction *m_wedgeBondAct;
      QAction *m_wedgeOrHashBondAct;
      QAction *m_cisTransBondAct;
      QAction *m_cyclopropaneAct;
      QAction *m_cyclobutaneAct;
      QAction *m_cyclopentaneAct;
      QAction *m_cyclohexaneAct;
      QAction *m_ring7Act;
      QAction *m_ring8Act;
      QAction *m_aromatic5ringAct;
      QAction *m_aromatic6ringAct;


  };

}

#endif // MSK_DRAWTOOL_H
