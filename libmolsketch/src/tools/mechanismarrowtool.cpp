#include "mechanismarrowtool.h"

#include "../toolgroup.h"
#include "../atom.h"
#include "../bond.h"
#include "../molscene.h"
#include "../commands.h"
#include "../math2d.h"
#include "../mechanismarrow.h"

#include <cmath>
#include <QDebug>

namespace Molsketch {

  using namespace Commands;

  MechanismArrowTool::MechanismArrowTool(MolScene *scene) : QObject(0), Tool(scene)
  {
    m_action = 0;
    m_mechanismArrowHint = 0;
  }

  MechanismArrowTool::~MechanismArrowTool()
  {
  }

  QList<QAction*> MechanismArrowTool::actions()
  {
    QList<QAction*> actions;

    if (m_action) {
      actions.append(m_action);
      return actions;
    }

    m_action = new QAction(QIcon(":images/mechanismarrow.png"), QObject::tr("Mechanism Arrow Tool"), 0);
    connect(m_action, SIGNAL(triggered()), this, SLOT(actionClicked()));
    actions.append(m_action);

    return actions;
  }

  QString MechanismArrowTool::toolbarName(QAction *action)
  {
    Q_UNUSED(action);
    return tr("Reaction");
  }

  void MechanismArrowTool::mousePressEvent(QGraphicsSceneMouseEvent *event)
  {
    if (Qt::LeftButton != event->button()) return ;
    QPointF downPos = event->buttonDownScenePos(event->button());

    QUndoStack *stack = scene()->stack();

    Atom *atom = scene()->atomAt(downPos);
    Bond *bond = scene()->bondAt(downPos);

    if (!atom && !bond) {
      // insert a mechanism arrow in an empty space on the scene
      MechanismArrow *arrow = new MechanismArrow;
      stack->push(new AddItem(arrow, scene()));
      arrow->setPos(downPos);
    }

    if (atom) {
      // start a mechanism arrow from an atom
      m_mechanismArrowHint = new QGraphicsLineItem(QLineF(atom->pos(), downPos));
      m_mechanismArrowHint->setPen(QPen(Qt::gray));
      scene()->addItem(m_mechanismArrowHint);
    }

    if (bond) {
      // start a mechanism arrow from an atom
      QPointF midPoint = (bond->beginAtom()->pos() + bond->endAtom()->pos()) / 2;
      m_mechanismArrowHint = new QGraphicsLineItem(QLineF(midPoint, downPos));
      m_mechanismArrowHint->setPen(QPen(Qt::gray));
      scene()->addItem(m_mechanismArrowHint);
    }
  }

  void MechanismArrowTool::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
  {
    if (!m_mechanismArrowHint)
      return;

    QPointF begin(m_mechanismArrowHint->line().x1(), m_mechanismArrowHint->line().y1());
    QPointF end = event->scenePos();

    m_mechanismArrowHint->setLine(QLineF(begin, end));
  }

  void MechanismArrowTool::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
  {
    if (!m_mechanismArrowHint)
      return;

    QUndoStack *stack = scene()->stack();

    QPointF begin(m_mechanismArrowHint->line().x1(), m_mechanismArrowHint->line().y1());
    QPointF end = event->scenePos();

    QPointF vb = end - begin;
    QPointF uvb = vb / sqrt(vb.x()*vb.x() + vb.y()*vb.y());
    QPointF orthogonal(uvb.y(), -uvb.x());

    if (scene()->atomAt(begin) && scene()->atomAt(end)) {
      QPointF c1 = (begin + end) / 2 + orthogonal * length(vb) / 3 - begin;

      MechanismArrow *arrow = new MechanismArrow(c1 - 10 * uvb, c1 + 10 * uvb, vb - 20 * uvb);
      stack->push(new AddItem(arrow, scene()));
      arrow->setPos(begin + 10 * uvb + 10 * orthogonal);
    } else if (scene()->bondAt(begin) && scene()->bondAt(end)) {
      QPointF c1 = (begin + end) / 2 + orthogonal * length(vb) / 2 - begin;

      MechanismArrow *arrow = new MechanismArrow(20 * orthogonal + vb / 2, 20 * orthogonal + vb / 2, vb);
      stack->push(new AddItem(arrow, scene()));
      arrow->setPos(begin + 10 * orthogonal);
    } else {
      QPointF c1 = (begin + end) / 2 + orthogonal * length(vb) / 2 - begin;

      MechanismArrow *arrow = new MechanismArrow(35 * orthogonal, vb + 35 * orthogonal, vb);
      stack->push(new AddItem(arrow, scene()));
      arrow->setPos(begin + 10 * orthogonal);
    }


    // remove the hint line
    scene()->removeItem(m_mechanismArrowHint);
    delete m_mechanismArrowHint;
    m_mechanismArrowHint = 0;
  }


  void MechanismArrowTool::actionClicked()
  {
    ToolGroup *toolGroup = static_cast<ToolGroup*>(m_action->property("toolGroup").value<void*>());
    if (!toolGroup)
      return;

    toolGroup->setActiveTool(this);
  }

}

