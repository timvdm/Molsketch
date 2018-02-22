#include "scenepropertieswidget.h"

#include "molscene.h"
#include "ui_scenepropertieswidget.h"

#include "scenesettings.h"
#include "settingsitem.h"
#include "settingsconnector.h"

namespace Molsketch {

  struct ScenePropertiesWidget::privateData
  {
    Ui::ScenePropertiesWidget *ui;
    MolScene *scene;

    privateData(MolScene *scene)
      : ui(new Ui::ScenePropertiesWidget),
        scene(scene)
    {}

    void setup() {
      if (!scene) return;
      SettingsConnector::connect(ui->bondLineWidth, scene->settings()->bondWidth(), scene->stack(), tr("Change bond line width"));

    }
  };

  ScenePropertiesWidget::ScenePropertiesWidget(SceneSettings *settings, MolScene *scene) :
    QWidget(0),
    d(new privateData(scene))
  {
    d->ui->setupUi(this);
    d->setup();
  }

  ScenePropertiesWidget::~ScenePropertiesWidget()
  {
    delete d->ui;
    delete d;
  }
} // namespace Molsketch
