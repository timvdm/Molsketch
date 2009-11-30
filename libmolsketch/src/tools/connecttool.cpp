#include "connecttool.h"

#include "../toolgroup.h"
#include "../atom.h"
#include "../bond.h"
#include "../molscene.h"
#include "../commands.h"
#include "../mimemolecule.h"

#include <cmath>
#include <QDebug>
#include <QDrag>

namespace Molsketch {

  using namespace Commands;

  ConnectTool::ConnectTool(MolScene *scene) : QObject(0), Tool(scene)
  {
    m_action = 0;
  }

  ConnectTool::~ConnectTool()
  {
  }

  QList<QAction*> ConnectTool::actions()
  {
    QList<QAction*> actions;

    if (m_action) {
      actions.append(m_action);
      return actions;
    }

    m_action = new QAction(QIcon(":images/connect.png"), QObject::tr("Connect Tool"), 0);
    connect(m_action, SIGNAL(triggered()), this, SLOT(actionClicked()));
    actions.append(m_action);

    return actions;
  }

  QString ConnectTool::toolbarName(QAction *action)
  {
    return tr("Tools");
  }

  void ConnectTool::mousePressEvent(QGraphicsSceneMouseEvent *event)
  {
    QPointF downPos = event->buttonDownScenePos(event->button());
    Molecule *mol = scene()->moleculeAt(downPos);
    if (mol) {
      QImage molImg = scene()->renderMolToImage(mol);

      MimeMolecule *data = new MimeMolecule;
      data->setMolecule(mol);
      data->setImageData(molImg);

      QDrag *drag = new QDrag(event->widget());
      drag->setMimeData(data);
      drag->setPixmap(QPixmap::fromImage(molImg));
      drag->start();
    }
  }

  void ConnectTool::actionClicked()
  {
    ToolGroup *toolGroup = static_cast<ToolGroup*>(m_action->property("toolGroup").value<void*>());
    if (!toolGroup)
      return;

    toolGroup->setActiveTool(this);
  }

}
