#ifndef BOOLEANPROPERTY_H
#define BOOLEANPROPERTY_H

#include "abstractproperty.h"

class BooleanProperty : public AbstractProperty
{
public:
  BooleanProperty(bool value = true);
  ~BooleanProperty();
  QWidget *generateController(QWidget* parent = 0) const = 0;
  void initializeController(QCheckBox *controller) const = 0;
  void applyController(const QWidget* controller) = 0;
  bool operator==(const AbstractProperty&) = 0;
  QString toXml() const = 0;
  void fromXml(const QString&) = 0;
private:
  class PrivateData;
  PrivateData *d;
};

#endif // BOOLEANPROPERTY_H
