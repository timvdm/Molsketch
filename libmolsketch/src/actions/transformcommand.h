#ifndef MOLSKETCH_TRANSFORMCOMMAND_H
#define MOLSKETCH_TRANSFORMCOMMAND_H

#include <QUndoCommand>

class coordinateItem ;

namespace Molsketch {

  class transformCommand : public QUndoCommand
  {
  public:
    transformCommand(coordinateItem *item, const QTransform &trafo, const QPointF &center, QUndoCommand *parent= 0);
    ~transformCommand() ;
    void undo() ;
    void redo() ;
    bool mergeWith(const QUndoCommand *other) ;
    int id() const ;
  private:
    class privateData ;
    privateData* d ;
  };

} // namespace Molsketch

#endif // MOLSKETCH_TRANSFORMCOMMAND_H
