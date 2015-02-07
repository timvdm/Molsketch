#ifndef MOLSKETCH_ROTATEACTION_H
#define MOLSKETCH_ROTATEACTION_H

#include "genericaction.h"

namespace Molsketch {
  class coordinateItem ;

  class transformAction : public genericAction
  {
    Q_OBJECT
  public:
    explicit transformAction(MolScene* scene);
    void mousePressEvent(QGraphicsSceneMouseEvent *event) ;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) ;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) ;

  protected:
    virtual QTransform generateTransform(const QLineF& originalLine, const QLineF& currentLine) = 0;
  private:
    coordinateItem *m_rotationItem;
    /** Used for rotations. Stores the last vector from mouse pointer to center of rotation */
    QLineF m_originalLine ;
  };

  class rotateAction : public transformAction
  {
    Q_OBJECT
  public:
    explicit rotateAction(MolScene* scene) ;
  private:
    QTransform generateTransform(const QLineF &originalLine, const QLineF &currentLine) ;
  };

  class translateAction : public transformAction
  {
    Q_OBJECT
  public:
    explicit translateAction(MolScene* scene) ;
  private:
    QTransform generateTransform(const QLineF &originalLine, const QLineF &currentLine) ;
  };

} // namespace Molsketch

#endif // MOLSKETCH_ROTATEACTION_H
