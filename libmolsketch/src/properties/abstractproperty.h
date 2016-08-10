#ifndef ABSTRACTPROPERTY_H
#define ABSTRACTPROPERTY_H

class AbstractProperty
{
protected:
  AbstractProperty();
public:
  virtual ~AbstractProperty();
  virtual QWidget *generateController(QWidget* parent = 0) const = 0;
  virtual void initializeController(QWidget* controller) const = 0;
  virtual void applyController(const QWidget* controller) = 0;
  virtual bool operator==(const AbstractProperty&) = 0;
  virtual QString toXml() const = 0;
  virtual void fromXml(const QString&) = 0;
};

#endif // ABSTRACTPROPERTY_H
