#include "rectanglevaluetrait.h"
#include <QString>

CxxTest::ValueTraits<QRectF>::ValueTraits(const QRectF &rect)
{
  value = ("("
           + QString::number(rect.topLeft().x()) + ", "
           + QString::number(rect.topLeft().y()) + ", "
           + QString::number(rect.width()) + ", "
           + QString::number(rect.height()) + ")"
           ).toStdString();
}


const char *CxxTest::ValueTraits<QRectF>::asString() const
{
  return value.c_str();
}
