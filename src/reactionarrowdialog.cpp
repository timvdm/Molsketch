#include "reactionarrowdialog.h"

#include "reactionarrow.h"

#include "ui_reactionarrowdialog.h"

namespace Molsketch {

  ReactionArrowDialog::ReactionArrowDialog(ReactionArrow *arrow, QWidget *parent) :
    QDialog(parent), m_ui(new Ui::ReactionArrowDialog), m_arrow(arrow)
  {
    m_ui->setupUi(this);

    connect(m_ui->typeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(typeChanged(int)));
  }

  void ReactionArrowDialog::typeChanged(int t)
  {
    m_arrow->setArrowType(static_cast<ReactionArrow::ArrowType>(t));  
    m_arrow->update();
  }

  ReactionArrowDialog::~ReactionArrowDialog()
  {
    delete m_ui;
  }

}
