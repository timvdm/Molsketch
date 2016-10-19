#ifndef RECTANGLEVALUETRAIT_H
#define RECTANGLEVALUETRAIT_H

#include <cxxtest/ValueTraits.h>
#include <string>
#include <QRectF>

namespace CxxTest
{
  CXXTEST_TEMPLATE_INSTANTIATION
  class ValueTraits<QRectF>
  {
    std::string value;

  public:
    ValueTraits(const QRectF& rect);
    const char *asString() const;
  };
};

#endif // RECTANGLEVALUETRAIT_H
