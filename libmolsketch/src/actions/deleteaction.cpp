#include "deleteaction.h"
#include "molscene.h"
#include "commands.h"

namespace Molsketch {

  deleteAction::deleteAction(MolScene *scene)
    : genericAction(scene)
  {
    setCheckable(false);
    connect(this, SIGNAL(triggered()), this, SLOT(deleteSelection()));
    setText(tr("Delete"));
    setIcon(QIcon(":images/delete.png"));
    setShortcut(QKeySequence::Delete);
  }

  deleteAction::~deleteAction()
  {
  }

  void deleteAction::deleteSelection()
  {
    if (!scene()) return;
    QList<QGraphicsItem*> selectedItems(scene()->selectedItems());
    if (selectedItems.isEmpty()) return;
    attemptBeginMacro(tr("Delete items"));
    foreach(QGraphicsItem* item, selectedItems)
    {
      switch(item->type())
      {
        case Atom::Type:
          attemptUndoPush(new Commands::DelAtom(dynamic_cast<Atom*>(item)));
          break;
        case Bond::Type:
          attemptUndoPush(new Commands::DelBond(dynamic_cast<Bond*>(item)));
          break;
        default:
          attemptUndoPush(new Commands::DelItem(item));
      }
      // TODO check if molecule is split
    }
    attemptEndEndMacro();
  }

} // namespace Molsketch

