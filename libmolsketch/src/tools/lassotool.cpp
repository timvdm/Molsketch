#include "lassotool.h"

#include "../toolgroup.h"
#include "../atom.h"
#include "../bond.h"
#include "../molscene.h"
#include "../commands.h"
#include "../math2d.h"

#include <cmath>
#include <QDebug>

namespace Molsketch {

  using namespace Commands;

  LassoTool::LassoTool(MolScene *scene) : QObject(0), Tool(scene)
  {
    m_action = 0;

    // Create the lasso and hide it for now
    m_lassoPolygon = scene->addPolygon(QPolygon());
    m_lassoPolygon->hide();
  }

  LassoTool::~LassoTool()
  {
  }

  QList<QAction*> LassoTool::actions()
  {
    QList<QAction*> actions;

    if (m_action) {
      actions.append(m_action);
      return actions;
    }

    m_action = new QAction(QIcon(":images/lasso.png"), QObject::tr("Lasso Tool"), 0);
    connect(m_action, SIGNAL(triggered()), this, SLOT(actionClicked()));
    actions.append(m_action);

    return actions;
  }

  QString LassoTool::toolbarName(QAction *action)
  {
    Q_UNUSED(action);
    return tr("Tools");
  }

  void LassoTool::activated()
  {
    foreach(QGraphicsItem* item, scene()->items()) {
      switch (item->type()) {
        case Molecule::Type:
        case Atom::Type:
        case Bond::Type:
          item->setFlag(QGraphicsItem::ItemIsSelectable, true);
          break;
        default:
          break;
      }
    }
  }

  void LassoTool::deactivated()
  {
    scene()->clearSelection();
    foreach(QGraphicsItem* item, scene()->items()) {
      item->setFlag(QGraphicsItem::ItemIsSelectable, false);
    }

  }

  void LassoTool::mousePressEvent(QGraphicsSceneMouseEvent *event)
  {
    m_lassoPolygon->show();
    m_lassoTrail.clear();
    m_lassoTrail.push_back(event->scenePos());
  }

  void LassoTool::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
  {
    if (!(event->buttons() & Qt::LeftButton))
      return;

    m_lassoTrail.push_back(event->scenePos());
    if (m_lassoTrail.size () > 4) {
      m_lassoPolygon->setPolygon(QPolygonF (m_lassoTrail));
      lassoSelect();
    }

    event->accept();
  }

  void LassoTool::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
  {
    Q_UNUSED(event)

    m_lassoTrail.clear();
    lassoSelect();
    m_lassoPolygon->setPolygon(QPolygonF ());
    m_lassoPolygon->hide ();
  }

  void LassoTool::lassoSelect()
  {
    scene()->clearSelection();
    QList<QGraphicsItem *> its = scene()->items(m_lassoPolygon ->polygon(), Qt::ContainsItemShape);
    for (int i = 0; i < its.size (); i++) {
      its[i]->setSelected(true);
    }
  }



  void LassoTool::actionClicked()
  {
    ToolGroup *toolGroup = static_cast<ToolGroup*>(m_action->property("toolGroup").value<void*>());
    if (!toolGroup)
      return;

    toolGroup->setActiveTool(this);
  }





}

