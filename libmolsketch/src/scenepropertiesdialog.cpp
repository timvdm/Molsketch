#include "scenepropertiesdialog.h"

#include "molscene.h"
#include "ui_scenepropertiesdialog.h"

namespace Molsketch {

  struct ScenePropertiesDialog::privateData
  {
    MolScene *scene;
    Ui::ScenePropertiesDialog *ui;

    void transferSceneToUi() {
      ui->arrowLineWidth->setValue(scene->arrowWidth());
//      ui->atomFont->sele
      ui->atomSize->setValue(scene->atomSize());
      ui->autoHydrogensVisible->setChecked(scene->autoAddHydrogen());
      ui->boldFont->setChecked(scene->atomFont().bold());
      ui->bondAngle->setValue(scene->bondAngle());
      ui->bondLength->setValue(scene->bondLength());
      ui->bondLineWidth->setValue(scene->bondWidth());
      ui->carbonsVisible->setChecked(scene->carbonVisible());
      ui->chargesVisible->setChecked(scene->chargeVisible());
//      ui->
      // this is getting cumbersome. How about a generic solution (TDD!)
      // - property owner interface:
      //    - set/get propertylist
      //    - set/get property by name?
      //    - prepare context menu to contain properties
      // - property:
      //    - produce widget
      //    - setup widget
      //    - read out widget (-> shared by multiple items!)
      //    - name and category
      //    - compare to others (setting if multiple)
      //    - XML output
      // - templates:
      //    - int (positive int)
      //    - (positive) float
      //    - Combobox
      //    - bool
    }
  };

  ScenePropertiesDialog::ScenePropertiesDialog(QWidget *parent) :
    QDialog(parent),
    d(new privateData)
  {
    d->ui = new Ui::ScenePropertiesDialog;
    d->ui->setupUi(this);
  }

  ScenePropertiesDialog::~ScenePropertiesDialog()
  {
    delete d->ui;
    delete d;
  }

  void ScenePropertiesDialog::setScene(MolScene *scene)
  {
    d->scene = scene;
  }

} // namespace Molsketch
