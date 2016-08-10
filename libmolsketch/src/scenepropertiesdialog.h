#ifndef MOLSKETCH_SCENEPROPERTIESDIALOG_H
#define MOLSKETCH_SCENEPROPERTIESDIALOG_H

#include <QDialog>

namespace Molsketch {

  class MolScene;

  namespace Ui {
    class ScenePropertiesDialog;
  }

  class ScenePropertiesDialog : public QDialog
  {
    Q_OBJECT

  public:
    explicit ScenePropertiesDialog(QWidget *parent = 0);
    ~ScenePropertiesDialog();

    void setScene(MolScene* scene);
  private:
    class privateData;
    privateData *d;
  };


} // namespace Molsketch
#endif // MOLSKETCH_SCENEPROPERTIESDIALOG_H
