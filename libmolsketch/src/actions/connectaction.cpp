#include "connectaction.h"
#include "molecule.h"
#include "mimemolecule.h"
#include "molscene.h"
#include <QDrag>

namespace Molsketch {

  connectAction::connectAction(MolScene *scene) :
    genericAction(scene)
  {
    setIcon(QIcon(":images/connect.png"));
    setText(tr("Connect to item")) ;
  }

  void connectAction::mousePressEvent(QGraphicsSceneMouseEvent *event)
  {
    Molecule *mol = scene()->moleculeAt(event->buttonDownScenePos(event->button()));
    if (!mol) return ;

    QImage molImg = scene()->renderMolToImage(mol);

    MimeMolecule *data = new MimeMolecule;
    data->setMolecule(mol);
    data->setImageData(molImg);

    QDrag *drag = new QDrag(event->widget());
    drag->setMimeData(data);
    drag->setPixmap(QPixmap::fromImage(molImg));
    drag->start();
  }

  // TODO use undo stack
} // namespace
