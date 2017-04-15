#ifndef GENERICS
#define GENERICS

#include <QAbstractButton>
#include <QAction>
#include <QButtonGroup>
#include <QList>


template <class itemType,
          void (itemType::*checkItemFunction)(bool),
          class containerType,
          QList<itemType*> (containerType::*getItemsFunction)() const,
          itemType* (containerType::*getCurrentItemFunction)() const
          >
void cycleItems(containerType* container, bool reverse = false)
{
  QList<itemType*> subItems((container->*getItemsFunction)());
  if (subItems.size() <= 1) return;
  itemType *activeItem = (container->*getCurrentItemFunction)();
  if (!activeItem) return ;
  int index = subItems.indexOf(activeItem);
  if (-1 == index) return;
  index += reverse ? -1 : 1;
  if (index < 0) index = subItems.size()-1;
  if (index == subItems.size()) index = 0;
  (subItems.at(index)->*checkItemFunction)(true);
}

inline void cycleActions(QActionGroup* group, bool reverse = false)
{
  cycleItems<QAction, &QAction::setChecked,
      QActionGroup, &QActionGroup::actions, &QActionGroup::checkedAction>
      (group, reverse);
}

inline void cycleButtons(QButtonGroup* group, bool reverse = false)
{
  cycleItems<QAbstractButton, &QAbstractButton::setChecked,
      QButtonGroup, &QButtonGroup::buttons, &QButtonGroup::checkedButton>
      (group, reverse);
}

#endif // GENERICS

