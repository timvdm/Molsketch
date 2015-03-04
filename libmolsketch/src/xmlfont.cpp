#include "xmlfont.h"

#include <QDataStream>

namespace Molsketch {

  xmlFont::xmlFont(const QFont &other)
    : QFont(other)
  {}

  xmlFont::xmlFont()
    : QFont()
  {}

  xmlFont::~xmlFont()
  {}

  QString xmlFont::xmlData() const
  {
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out << *this;
    out.setDevice(0);
    return ba.toBase64();
  }

  void xmlFont::fromXmlData(const QString &data)
  {
    QByteArray ba(QByteArray::fromBase64(data.toUtf8()));
    QDataStream in(&ba, QIODevice::ReadOnly);
    in >> *this;
  }

  xmlFont xmlFont::generateFromXml(const QString &data)
  {
    xmlFont font;
    font.fromXmlData(data);
    return font;
  }

} // namespace Molsketch
