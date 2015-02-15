#include "bondtypewidget.h"
#include <QButtonGroup>
#include <QToolButton>
#include <QHBoxLayout>

namespace Molsketch {

  class bondTypeWidget::privateData
  {
    void createButton(const QString& IconName,
                      const bondTypeWidget::BondType& type,
                      bool forwardAndBackward = false) ;
  public:
    QButtonGroup *buttonGroup ;
    QHBoxLayout *layout ;
    bondTypeWidget *parent;
    privateData(bondTypeWidget* p)
      : buttonGroup(new QButtonGroup(p)),
        layout(new QHBoxLayout(p)),
        parent(p)
    {
      buttonGroup->setExclusive(true) ;
      createButton("single", Single) ;
      createButton("hash", Hash, true) ;
      createButton("wedge", Wedge, true) ;
      createButton("hashOrWedge", WedgeOrHash) ;
      createButton("double", Double) ;
      createButton("cistrans", CisOrTrans) ;
      createButton("triple", Triple) ;
      buttonGroup->button(Single)->setChecked(true) ;
    }
  };

  void bondTypeWidget::privateData::createButton(const QString &IconName, const BondType &type, bool forwardAndBackward)
  {
    QToolButton *button = new QToolButton(parent) ;
    buttonGroup->addButton(button, type) ;
    QImage icon(":images/" + IconName + ".png") ;
    button->setIcon(QPixmap::fromImage(icon)) ;
    button->setAutoRaise(true);
    button->setCheckable(true);
    layout->addWidget(button);

    if (!forwardAndBackward) return;
    button = new QToolButton(parent);
    buttonGroup->addButton(button, -(int) type);
    button->setIcon(QPixmap::fromImage(icon.mirrored(true, true)));
    button->setAutoRaise(true);
    button->setCheckable(true);
    layout->addWidget(button);
  }

  bondTypeWidget::bondTypeWidget(QWidget *parent)
    : QWidget(parent),
      d(new privateData(this))
  {
    connect(d->buttonGroup, SIGNAL(buttonToggled(int,bool)),
            this, SLOT(changeType())) ;
  }

  bondTypeWidget::~bondTypeWidget()
  {
    delete d ;
  }

  bool bondTypeWidget::backward() const
  {
    return d->buttonGroup->checkedId() < 0 ;
  }

  bondTypeWidget::BondType bondTypeWidget::bondType() const
  {
    return (BondType) qAbs(d->buttonGroup->checkedId()) ;
  }

  Bond::BondType bondTypeWidget::legacyType() const
  {
    switch (bondType())
    {
      case Triple:
      case Double:
      case Single: return Bond::InPlane;
      case CisOrTrans: return Bond::CisOrTrans;
      case WedgeOrHash: return Bond::WedgeOrHash;
      case Wedge:
        return backward() ? Bond::InvertedWedge : Bond::Wedge;
      case Hash:
        return backward() ? Bond::InvertedHash : Bond::Hash;
    }
    return Bond::NoType;
  }

  QIcon bondTypeWidget::bondIcon() const
  {
    return d->buttonGroup->checkedButton()->icon() ;
  }

  int bondTypeWidget::bondOrder() const
  {
    return d->buttonGroup->checkedId() / 10;
  }

  void bondTypeWidget::changeType()
  {
    emit bondTypeChanged(bondType());
  }

} // namespace
