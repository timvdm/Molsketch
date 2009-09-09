#ifndef REACTIONARROWDIALOG_H
#define REACTIONARROWDIALOG_H

#include <QtGui/QDialog>


namespace Ui {
    class ReactionArrowDialog;
}

namespace Molsketch {

  class ReactionArrow;

  class ReactionArrowDialog : public QDialog 
  {
    Q_OBJECT
    public:
      ReactionArrowDialog(ReactionArrow *arrow, QWidget *parent = 0);
      virtual ~ReactionArrowDialog();
  
    public Q_SLOTS:
      void typeChanged(int);

    private:
      ReactionArrow *m_arrow;
      Ui::ReactionArrowDialog *m_ui;
  };

}

#endif // REACTIONARROWDIALOG_H
