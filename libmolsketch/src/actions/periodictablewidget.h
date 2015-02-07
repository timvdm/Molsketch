#ifndef PERIODICTABLEWIDGET_H
#define PERIODICTABLEWIDGET_H

#include <QWidget>

namespace Molsketch {
  class periodicTableWidget : public QWidget
  {
    Q_OBJECT
  public:
    explicit periodicTableWidget(QWidget *parent = 0);
    void setAdditionalElements(const QString&elements);
    QString currentElement() const ;
    ~periodicTableWidget();
  signals:
    void elementChanged(const QString& element) ;
  private:
    class privateData ;
    privateData *d ;
  private slots:
    void changeElement() ;
  };

} // namespace
#endif // PERIODICTABLEWIDGET_H
