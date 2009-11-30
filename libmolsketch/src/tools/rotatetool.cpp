#include "rotatetool.h"

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

  RotateTool::RotateTool(MolScene *scene) : QObject(0), Tool(scene)
  {
    m_action = 0;
    m_rotationItem = 0;
  }

  RotateTool::~RotateTool()
  {
  }

  QList<QAction*> RotateTool::actions()
  {
    QList<QAction*> actions;

    if (m_action) {
      actions.append(m_action);
      return actions;
    }

    m_action = new QAction(QIcon(":images/rotate.png"), QObject::tr("Rotate Tool"), 0);
    connect(m_action, SIGNAL(triggered()), this, SLOT(actionClicked()));
    actions.append(m_action);

    return actions;
  }

  QString RotateTool::toolbarName(QAction *action)
  {
    return tr("Tools");
  }

  void RotateTool::mousePressEvent(QGraphicsSceneMouseEvent *event)
  {
    QGraphicsItem * item = scene()->itemAt(event->buttonDownScenePos(Qt::LeftButton));
    if (!item)
      return;

    if (item->type() == Atom::Type)
      item = item->parentItem();
    if (item->type() == Bond::Type)
      item = item->parentItem();

    QPointF rotatePoint = item->boundingRect().center();
    QPointF rotatePointAbs = item->mapToScene(rotatePoint);

    m_rotationCenter = rotatePointAbs;
    m_rotationItem = item;
    m_lastRotationVect = event->buttonDownScenePos(Qt::LeftButton) - rotatePointAbs ; //save vector for relative rotation step
  }

  void RotateTool::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
  {
     // Do nothing if no buttons are pressed or no item has been selected
    if (!m_rotationItem) return;
    if (!(event->buttons() & Qt::LeftButton)) return;
    /*
    // Find the item to rotate and it's rotation point
    QGraphicsItem * item = itemAt(event->buttonDownScenePos(Qt::LeftButton));
    if (!item) return;
    if (item->type() == Atom::Type) item = item->parentItem();
    if (item->type() == Bond::Type) item = item->parentItem();
    QPointF rotatePoint = item->boundingRect().center();
    QPointF rotatePointAbs = item->mapToScene(rotatePoint);
     */

  //  QPointF rotatePoint = m_rotationItem->boundingRect().center();
 //   QPointF rotatePointAbs = m_rotationItem->mapToScene(rotatePoint);


    // Calculate the rotation angle
    QPointF vec1 = m_lastRotationVect;
    QPointF vec2 = event->scenePos() - m_rotationCenter;

    qreal crossprod = vec1.x () * vec2.y () - vec1.y () * vec2.x ();
    qreal dotprod =   vec1.x () * vec2.x () + vec1.y () * vec2.y ();
    qreal rotateAngle = std::atan2 (crossprod, dotprod) * 180 / M_PI;


    //	std::cerr << rotateAngle<<"   "<<std::endl;
    //   if (event->modifiers() != Qt::AltModifier) rotateAngle = toRoundedAngle(rotateAngle);
    if (rotateAngle == 0) return;

    // Temporary rotate the item
    QTransform transform;
    transform.translate(m_rotationCenter.x(), m_rotationCenter.y());
    switch (event->modifiers()) {
      case Qt::ControlModifier: transform.rotate(rotateAngle, Qt::XAxis); break;
      case Qt::ShiftModifier: transform.rotate(rotateAngle, Qt::YAxis); break;
      default: transform.rotate(rotateAngle, Qt::ZAxis);
    };
    transform.translate(-m_rotationCenter.x(), -m_rotationCenter.y());
    m_rotationItem->setTransform(transform, true);
    m_lastRotationVect = vec2;
    //   item->rotate(rotateAngle);
  }

  void RotateTool::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
  {
    Q_UNUSED(event)
    m_rotationItem = 0;
  }



  void RotateTool::actionClicked()
  {
    ToolGroup *toolGroup = static_cast<ToolGroup*>(m_action->property("toolGroup").value<void*>());
    if (!toolGroup)
      return;

    toolGroup->setActiveTool(this);
  }





}
