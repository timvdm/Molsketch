#ifndef MOLSKETCH_ITEMGROUPTYPEACTION_H
#define MOLSKETCH_ITEMGROUPTYPEACTION_H

#include "abstractitemaction.h"

namespace Molsketch {

  class ItemTypeWidget;

  class ItemGroupTypeAction : public abstractItemAction
  {
    Q_OBJECT
  public:
    explicit ItemGroupTypeAction(MolScene *scene = 0);
    ~ItemGroupTypeAction();
  protected:
    void setItemTypeWidget(ItemTypeWidget* widget);
    virtual QString undoName() const;
    virtual void applyType(int type, const QVariant& data) const = 0;
    virtual void getType(int& type, QVariant& data) const = 0;
    virtual int defaultType() const;
    virtual QVariant defaultData() const;
  private:
    class privateData;
    privateData *d;
    void execute();
  private slots:
    virtual void checkItemType();
  };

} // namespace Molsketch

#endif // MOLSKETCH_ITEMGROUPTYPEACTION_H
