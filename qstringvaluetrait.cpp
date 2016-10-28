#include "qstringvaluetrait.h"


CxxTest::ValueTraits<QString>::ValueTraits(const QString &str)
{
  value = str;
}


const char *CxxTest::ValueTraits<QString>::asString() const
{
  return value.toStdString().c_str();
}
