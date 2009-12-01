#include "tool.h"

namespace Molsketch {

  Tool::Tool(MolScene *scene) : m_scene(scene)
  {
  }

  Tool::~Tool()
  {
  }

  void Tool::mousePressEvent(QGraphicsSceneMouseEvent *event)
  {
    Q_UNUSED(event);
  }

  void Tool::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
  {
    Q_UNUSED(event);
  }

  void Tool::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
  {
    Q_UNUSED(event);
  }

  void Tool::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
  {
    Q_UNUSED(event);
  }
      
  MolScene* Tool::scene() const
  {
    return m_scene;
  }

  void Tool::activated()
  {
  }

  void Tool::deactivated()
  {
  }

}
