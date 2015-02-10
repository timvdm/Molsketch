#include "reactionarrowaction.h"
#include "arrow.h"
#include "molscene.h"
#include "commands.h"
#include <QDebug>

namespace Molsketch {

  class reactionArrowAction::privateData
  {
  public:
    privateData(QObject* parent)
#define ARROWACTION(NAME,DESCRIPTION,ICON) NAME(new QAction(QIcon(":images/" ICON ".png"), tr(DESCRIPTION), parent))
      : ARROWACTION(normalArrow, "Reaction arrow", "reactionarrow"),
        ARROWACTION(doubleArrow, "Resonance arrow", "resonancearrow"),
        ARROWACTION(hookArrow, "Half arrow", "halfarrow"),
        currentArrow(0)
    {}
    ~privateData()
    {

    }
    QAction *normalArrow,
    *doubleArrow,
    *hookArrow ;
    Arrow* currentArrow ;
  };

  reactionArrowAction::reactionArrowAction(MolScene *scene)
    : multiAction(scene),
      d(new privateData(this))
  {
    addSubAction(d->normalArrow) ;
    addSubAction(d->doubleArrow) ;
    addSubAction(d->hookArrow) ;
  }

  reactionArrowAction::~reactionArrowAction()
  {
    delete d ;
  }

  void reactionArrowAction::mousePressEvent(QGraphicsSceneMouseEvent *event)
  {
    if (event->button() != Qt::LeftButton) return ;
    if (event->modifiers() != Qt::NoModifier) return ;
    if (d->currentArrow) delete d->currentArrow ;
    d->currentArrow = new Arrow() ;
    if (activeSubAction() == d->doubleArrow)
      d->currentArrow->setArrowType(Arrow::LowerForward
                                    | Arrow::UpperForward
                                    | Arrow::LowerBackward
                                    | Arrow::UpperBackward);
    if (activeSubAction() == d->hookArrow)
      d->currentArrow->setArrowType(Arrow::UpperBackward);
    QPointF pos = event->scenePos() ;
    d->currentArrow->setCoordinates(QVector<QPointF>(2, pos)) ;
    scene()->addItem(d->currentArrow) ;
    scene()->update(d->currentArrow->boundingRect());
    qDebug() << "Arrow origin" << pos ;
  }

  void reactionArrowAction::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
  {
    if (!d->currentArrow) return ;
    d->currentArrow->setPoint(0,event->scenePos()) ;
    scene()->update(d->currentArrow->boundingRect());
  }

  void reactionArrowAction::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
  {
    Q_UNUSED(event)
    if (!d->currentArrow) return ;
    if (!scene() || !scene()->stack()) return ; // TODO
    scene()->stack()->push(new Commands::AddItem(d->currentArrow, scene())) ;
    d->currentArrow = 0 ;
  }

} // namespace
