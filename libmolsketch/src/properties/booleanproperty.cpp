#include "booleanproperty.h"

#include <QCheckBox>

struct BooleanProperty::PrivateData {
  bool value;
};

BooleanProperty::BooleanProperty(bool value)
  : d(new PrivateData)
{
  d->value = value;
}

BooleanProperty::~BooleanProperty()
{
  delete d;
}

QWidget *BooleanProperty::generateController(QWidget *parent) const
{
  return new QCheckBox(parent);
}

void BooleanProperty::initializeController(QCheckBox *controller) const
{
  controller->setChecked();
}
