#include <QSet>
#include <algorithm>
#include "alignmentaction.h"
#include "commands.h"
#include <QDebug>

namespace Molsketch {

  AlignmentAction::AlignmentAction(const QString &description, MolScene *scene)
    : AbstractItemAction(scene) {
    setCheckable(false);
    setMinimumItemCount(2);
    setText(description);
    // TODO when selecting a hidden atom with a molecule, it gets hidden again, but the molecule's boundingRect expands and remains so.
  }


  void AlignmentAction::execute() {
    QList<graphicsItem*> itemList = items();
    qreal targetValue = std::accumulate(itemList.begin(), itemList.end(),
                                        initialValue(), getAccumulator(itemList.size()));

    attemptBeginMacro(text());
    for (auto item : itemList) {
      // TODO this a workaround for molecules not using relative coordinates
      attemptUndoPush(
            new Commands::SetCoordinateCommand(item,item->coordinates().translated(getShift(item, targetValue))));
    }
    attemptEndMacro();
  }

  QSet<graphicsItem *> AlignmentAction::filterItems(const QList<QGraphicsItem *> &inputItems) const {
    QSet<graphicsItem*> result;
    for (auto item : inputItems)
      result << dynamic_cast<Molecule*>(item);
    result.remove(nullptr);
    return result;
  }

#define ALIGNMENT_ACTION(DESCRIPTION, START, ICON, SHIFT, ACCUMULATOR) \
  class AlignmentActionImpl : public AlignmentAction { \
  Accumulator getAccumulator(int count) const override { Q_UNUSED(count) \
  return [=](const qreal& current, const graphicsItem* item) -> qreal { return ACCUMULATOR; } ; } \
  QPointF getShift(const graphicsItem* item, const qreal& targetValue) const override {return SHIFT; } \
  qreal initialValue() const override { return START; } \
  public: AlignmentActionImpl(MolScene *scene) : AlignmentAction(DESCRIPTION, scene) \
  { setIcon(QIcon(":images/" ICON ".svg")); } \
}; \
  return new AlignmentActionImpl(scene);

  AlignmentAction *AlignmentAction::flushLeft(MolScene *scene) {
    ALIGNMENT_ACTION("Align left", qInf(), "align-left",
                     QPointF(targetValue - item->boundingRect().left(), 0),
                     qMin(current, item->boundingRect().left()))
  }

  AlignmentAction *AlignmentAction::flushRight(MolScene *scene) {
    ALIGNMENT_ACTION("Align right", -qInf(), "align-right",
                     QPointF(targetValue - item->boundingRect().right(), 0),
                     qMax(current, item->boundingRect().right()))
  }

  AlignmentAction *AlignmentAction::atTop(MolScene *scene) {
    ALIGNMENT_ACTION("Align top", qInf(), "align-top",
                     QPointF(0, targetValue - item->boundingRect().top()),
                     qMin(current, item->boundingRect().top()))
  }

  AlignmentAction *AlignmentAction::atBottom(MolScene *scene) {
    ALIGNMENT_ACTION("Align bottom", -qInf(), "align-bottom",
                     QPointF(0, targetValue - item->boundingRect().bottom()),
                     qMax(current, item->boundingRect().bottom()))
  }

  AlignmentAction *AlignmentAction::atVerticalCenter(MolScene *scene) {
    ALIGNMENT_ACTION("Align vertically", 0, "align-v-center",
                     QPointF(0, targetValue - item->boundingRect().center().y()),
                     current + item->boundingRect().center().y() / count)
  }

  AlignmentAction *AlignmentAction::atHorizontalCenter(MolScene *scene) {
    ALIGNMENT_ACTION("Align horizontally", 0, "align-h-center",
                     QPointF(targetValue - item->boundingRect().center().x(), 0),
                     current + item->boundingRect().center().x() / count)
  }
} // namespace Molsketch
