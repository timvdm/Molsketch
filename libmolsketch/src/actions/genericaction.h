#ifndef GENERICACTION_H
#define GENERICACTION_H

#include <QAction>
#include "tool.h"

class QGraphicsSceneMouseEvent ;
class QUndoStack ;

namespace Molsketch {

  class MolScene ;

  class genericAction : public QAction, public Tool // TODO obsolete Tool
  {
    Q_OBJECT
  public:
    explicit genericAction(MolScene* scene);
    virtual ~genericAction() {}

    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) { Q_UNUSED(event) }
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event) { Q_UNUSED(event) }
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) { Q_UNUSED(event) }
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) { Q_UNUSED(event) }

    // OBSOLETE
    QList<QAction*> actions() ;
    QString toolbarName(QAction *action) ;

  protected:
    MolScene *scene() const ;
    QUndoStack* undoStack() const ;
  public: // TODO Should be protected
    virtual void deactivated() {}
    virtual void activated() {}
  private slots:
    void activationSlot(const bool& b) ;
  };

} // namespace

#endif // ABSTRACTACTION_H
