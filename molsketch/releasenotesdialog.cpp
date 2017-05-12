#include "releasenotesdialog.h"
#include "ui_releasenotesdialog.h"

ReleaseNotesDialog::ReleaseNotesDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::ReleaseNotesDialog)
{
  ui->setupUi(this);
}

ReleaseNotesDialog::~ReleaseNotesDialog()
{
  delete ui;
}
