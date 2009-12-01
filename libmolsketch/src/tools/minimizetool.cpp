#include "minimizetool.h"

#include "../toolgroup.h"
#include "../atom.h"
#include "../bond.h"
#include "../molscene.h"
#include "../minimise.h"
#include "../math2d.h"

#include <cmath>
#include <QDebug>

namespace Molsketch {

  MinimizeTool::MinimizeTool(MolScene *scene) : QObject(0), Tool(scene)
  {
    m_action = 0;
  }

  MinimizeTool::~MinimizeTool()
  {
  }

  QList<QAction*> MinimizeTool::actions()
  {
    QList<QAction*> actions;

    if (m_action) {
      actions.append(m_action);
      return actions;
    }

    m_action = new QAction(tr("min"), 0);
    connect(m_action, SIGNAL(triggered()), this, SLOT(actionClicked()));
    actions.append(m_action);

    return actions;
  }

  QString MinimizeTool::toolbarName(QAction *action)
  {
    Q_UNUSED(action);
    return tr("Tools");
  }

  void MinimizeTool::mousePressEvent(QGraphicsSceneMouseEvent *event)
  {
    Bond *bond = scene()->bondAt (event ->scenePos());
    if (bond) {
      Molecule *molecule = bond ->molecule();
      mirrorBondInMolecule(molecule, bond);
      return;
    }

    Molecule * molecule = scene()->moleculeAt(event->scenePos());
    if (molecule)
      minimiseMolecule (molecule);
  }

  void MinimizeTool::mirrorBondInMolecule (Molecule *mol, Bond *bo)
  {
    Minimise *minimise = new Minimise (40 /*m_bondLength*/); // FIXME
    //minimise ->minimiseMolecule (mol);
    minimise ->mirrorBondInMolecule(mol, bo);
  }

  void MinimizeTool::minimiseMolecule (Molecule *mol)
  {
    Minimise *minimise = new Minimise (40 /*m_bondLength*/); // FIXME
    //minimise ->minimiseMolecule (mol);
    minimise ->conformationalSearchMolecule(mol);
  }

  void MinimizeTool::actionClicked()
  {
    ToolGroup *toolGroup = static_cast<ToolGroup*>(m_action->property("toolGroup").value<void*>());
    if (!toolGroup)
      return;

    toolGroup->setActiveTool(this);
  }

}
