#ifndef ALIGNMENTACTION_H
#define ALIGNMENTACTION_H

#include <functional>
#include "abstractitemaction.h"

namespace Molsketch {

  class AlignmentAction : public AbstractItemAction	{
	public:
    static AlignmentAction* flushLeft(MolScene *scene = 0);
    static AlignmentAction* flushRight(MolScene *scene = 0);
    static AlignmentAction* atTop(MolScene *scene = 0);
    static AlignmentAction* atBottom(MolScene *scene = 0);
    static AlignmentAction* atVerticalCenter(MolScene *scene = 0);
    static AlignmentAction* atHorizontalCenter(MolScene *scene = 0);
  private:
    QSet<graphicsItem *> filterItems(const QList<QGraphicsItem *> &inputItems) const override;
    AlignmentAction(const QString& description, MolScene *scene = 0);
    void execute() override;
    typedef std::function<qreal (const qreal&, const graphicsItem*)> Accumulator;
    virtual Accumulator getAccumulator(int count) const = 0;
    virtual QPointF getShift(const graphicsItem* item, const qreal& targetValue) const = 0;
    virtual qreal initialValue() const = 0;
  };

} // namespace Molsketch

#endif // ALIGNMENTACTION_H
