#ifndef XMLFONT_H
#define XMLFONT_H

#include <QFont>

namespace Molsketch {

  class xmlFont : public QFont
  {
  public:
    xmlFont(const QFont& other);
    xmlFont();
    ~xmlFont();
    QString xmlData() const;
    void fromXmlData(const QString& data) ;
    static xmlFont generateFromXml(const QString& data) ;
  };

} // namespace Molsketch

#endif // XMLFONT_H
