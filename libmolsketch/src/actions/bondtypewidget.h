#ifndef BONDTYPEWIDGET_H
#define BONDTYPEWIDGET_H

#include <QWidget>
#include "bond.h"

namespace Molsketch {

  class bondTypeWidget : public QWidget
  {
    Q_OBJECT
  public:
    enum BondType
    {
      Single = 10,
      Wedge  = 11,
      Hash   = 12,
      WedgeOrHash = 13,
      Double = 20,
      CisOrTrans = 21,
      Triple = 30
    };

    explicit bondTypeWidget(QWidget *parent = 0);
    ~bondTypeWidget();
    bool backward() const ;
    BondType bondType() const ;
    Bond::BondType legacyType() const;
    QPixmap bondIcon() const ;
    int bondOrder() const ;

  signals:
    void bondTypeChanged(const bondTypeWidget::BondType& newType) ;

  private:
    class privateData ;
    privateData *d;
  private slots:
    void changeType() ;
  };

} // namespace

#endif // BONDTYPEWIDGET_H
