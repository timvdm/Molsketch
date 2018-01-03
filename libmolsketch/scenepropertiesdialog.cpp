#include "scenepropertiesdialog.h"

#include "molscene.h"
#include "ui_scenepropertiesdialog.h"

#include "scenesettings.h"
#include "settingsitem.h"

namespace Molsketch {

  struct ScenePropertiesDialog::privateData
  {
    MolScene *scene;
    Ui::ScenePropertiesDialog *ui;

    void transferSceneToUi() {
      ui->arrowLineWidth->setValue(scene->settings()->arrowWidth()->get());
      ui->autoHydrogensVisible->setChecked(scene->settings()->autoAddHydrogen()->get());
      ui->boldFont->setChecked(scene->settings()->atomFont()->get().bold());
      ui->bondAngle->setValue(scene->settings()->bondAngle()->get());
      ui->bondLength->setValue(scene->settings()->bondLength()->get());
      ui->bondLineWidth->setValue(scene->settings()->bondWidth()->get());
      ui->carbonsVisible->setChecked(scene->settings()->carbonVisible()->get());
      ui->chargesVisible->setChecked(scene->settings()->chargeVisible()->get());
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
