// TODO GPL headers

#ifndef CONNECTACTION_H
#define CONNECTACTION_H
#include "genericaction.h"

namespace Molsketch {

  class connectAction : public genericAction
  {
    Q_OBJECT
  public:
    explicit connectAction(MolScene* scene);
    void mousePressEvent(QGraphicsSceneMouseEvent *event) ;

  };

} // namespace

#endif // CONNECTACTION_H
