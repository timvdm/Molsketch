#include "graphicsitem.h"

#include "transformcommand.h"

namespace Molsketch {

  struct transformCommand::privateData
  {
    QVector<QPointF> coordinates ;
    graphicsItem *item ;
    int originalTrafoType ;
  };

  transformCommand::transformCommand(graphicsItem *item,
                                     const QTransform& trafo,
                                     const QPointF& itemCenter,
                                     QUndoCommand* parent)
    : QUndoCommand(parent),
      d(new transformCommand::privateData)
  {
    d->item = item ;
    QTransform transformation ;
    transformation.translate(itemCenter.x(), itemCenter.y()) ;
    transformation =
        transformation.inverted() *
        trafo *
        transformation
        ;
    foreach(const QPointF& p, item->coordinates())
      d->coordinates << transformation.map(p) ;
    d->originalTrafoType = trafo.type() ;
  }

  transformCommand::~transformCommand()
  {
    delete d ;
  }

  void transformCommand::undo()
  {
    redo() ;
  }

  void transformCommand::redo()
  {
    QVector<QPointF> temp(d->item->coordinates()) ;
    d->item->setCoordinates(d->coordinates) ;
    d->coordinates.swap(temp);
  }

  bool transformCommand::mergeWith(const QUndoCommand *other)
  {
    const transformCommand* otherTrafo = dynamic_cast<const transformCommand*>(other) ;
    if (!otherTrafo) return false ;
    if (d->item != otherTrafo->d->item) return false ;
    if (d->coordinates.size() != otherTrafo->d->coordinates.size()) return false ;
    return true ;
  }

  int transformCommand::id() const
  {
    return d->originalTrafoType ;
  }

} // namespace Molsketch
