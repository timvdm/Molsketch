#include "periodictablewidget.h"
#include <QToolButton>
#include <QGridLayout>
#include <QButtonGroup>
#include <QPainter>

namespace Molsketch {


  class periodicTableWidget::privateData
  {
  public:
    QButtonGroup *buttonGroup ;
    periodicTableWidget* table ;
    explicit privateData(periodicTableWidget *parent)
      : buttonGroup(new QButtonGroup(parent)),
        table(parent)
    {
      buttonGroup->setExclusive(true);
    }
    void buildButtons(const QString &buttonDescription) ;
  };

  void periodicTableWidget::privateData::buildButtons(const QString& buttonDescription)
  {
    QGridLayout *layout = dynamic_cast<QGridLayout*>(table->layout()) ;
    if (!layout) return ;

    QString activeElement ;
    foreach (QToolButton* button, table->findChildren<QToolButton*>())
    {
      if (button->isChecked())
        activeElement = button->text() ;
      delete button ;
    }

    QStringList periodicTable(buttonDescription.split(QRegExp("(?=[A-Z \\n])")).mid(1)) ;
    int periodCounter = 0,
        groupCounter  = 0;
    foreach (const QString& element, periodicTable)
    {
      if ("\n" == element)
      {
        ++ periodCounter ;
        groupCounter = 0 ;
        continue ;
      }
      if (" " != element)
      {
        QToolButton *elementButton = new QToolButton(table) ;
        elementButton->setText(element) ;
        QFont font = elementButton->font() ;
        font.setPixelSize(qMax(8, font.pixelSize())) ;
        elementButton->setFont(font) ;
        elementButton->setAutoRaise(true) ;
        elementButton->setCheckable(true) ;
        elementButton->setChecked(element == activeElement) ;
        layout->addWidget(elementButton, periodCounter, groupCounter) ;
        buttonGroup->addButton(elementButton) ;
      }
      ++ groupCounter ;
    }

    // make sure one is checked
    if (!buttonGroup->checkedButton()
        && !buttonGroup->buttons().isEmpty())
    {
      QAbstractButton* button = buttonGroup->buttons().first() ;
      if (button) button->setChecked(true) ;
    }

    // cosmetics
    for (int i = 0 ; i < layout->rowCount() ; ++i)
    {
      layout->setRowMinimumHeight(i, 0);
      layout->setRowStretch(i,0);
    }
    for (int i = 0 ; i < layout->columnCount() ; ++i)
    {
      layout->setColumnMinimumWidth(i,0);
      layout->setColumnStretch(i,0);
    }
  }

  periodicTableWidget::periodicTableWidget(QWidget *parent)
    : QWidget(parent),
      d(new privateData(this))
  {
    QGridLayout *layout = new QGridLayout(this) ;
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);
    setAdditionalElements("");
    connect(d->buttonGroup, SIGNAL(buttonToggled(QAbstractButton*,bool)), this, SLOT(changeElement())) ;
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  }

  void periodicTableWidget::setAdditionalElements(const QString &elements)
  {
    d->buildButtons(// From ascii.periodni.com
                 QString("H                He\n"
                         "LiBe          BCNOFNe\n"
                         "NaMg          AlSiPSClAr\n"
                         "KCaScTiVCrMnFeCoNiCuZnGaGeAsSeBrKr\n"
                         "RbSrYZrNbMoTcRuRhPdAgCdInSnSbTeIXe\n"
                         "CsBa HfTaWReOsIrPtAuHgTlPbBiPoAtRn\n"
                         "FrRa RfDbSgBhHsMtDsRgCnUutFlUupLvUusUuo\n"
                         "\n"
                         "  LaCePrNdPmSmEuGdTbDyHoErTmYbLu\n"
                         "  AcThPaUNpPuAmCmBkCfEsFmMdNoLr")
                 + (elements.isEmpty() ? QString() : QString("\n\n"))
                 + elements);
  }

  QString periodicTableWidget::currentElement() const
  {
    QAbstractButton* button = d->buttonGroup->checkedButton();
    if (!button) return "";
    return button->text() ;
  }

  periodicTableWidget::~periodicTableWidget()
  {
    delete d ;
  }

  QPixmap periodicTableWidget::currentIcon() const
  {
    QAbstractButton *button = d->buttonGroup->checkedButton();
    if (!button) return QPixmap();
    QFont font = button->font();
    font.setPixelSize(2*font.pixelSize());
    QPixmap pixmap(QFontMetrics(font).boundingRect(currentElement()).size() + QSize(2,2)); // TODO why do we have to add (2,2)?
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setFont(font);
    painter.drawText(QRectF(0,0, pixmap.width(), pixmap.height()), currentElement());
    return pixmap;
  }

  void periodicTableWidget::changeElement()
  {
    emit elementChanged(currentElement());
  }

} // namespace
