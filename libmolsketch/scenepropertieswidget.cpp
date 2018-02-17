#include "scenepropertieswidget.h"

#include "molscene.h"
#include "ui_scenepropertieswidget.h"

#include "scenesettings.h"
#include "settingsitem.h"

namespace Molsketch {

  struct ScenePropertiesWidget::privateData
  {
    SceneSettings *settings;
    Ui::ScenePropertiesWidget *ui;

    privateData(SceneSettings *settings)
      : settings(settings),
        ui(new Ui::ScenePropertiesWidget)
    {}

    void setup() {
      if (!settings) return;
      ui->bondLineWidth->setValue(settings->bondWidth()->get());
    }

  };

  ScenePropertiesWidget::ScenePropertiesWidget(SceneSettings *settings, MolScene *scene) :
    QWidget(0),
    d(new privateData(settings))
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
