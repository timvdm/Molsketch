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
      auto settings = scene->settings();
      auto stack = scene->stack();
      SettingsConnector::connect(ui->bondLineWidth, settings->bondWidth(), stack, tr("Change bond line width"));
      SettingsConnector::connect(ui->arrowLineWidth, settings->arrowWidth(), stack, tr("Change arrow line width"));
      SettingsConnector::connect(ui->frameLineWidth, settings->frameLineWidth(), stack, tr("Change frame line width"));

      SettingsConnector::connect(ui->bondLength, settings->bondLength(), stack, tr("Change default bond length"));
      SettingsConnector::connect(ui->bondAngle, settings->bondAngle(), stack, tr("Change default bond angle"));


      SettingsConnector::connect(ui->autoHydrogensVisible, settings->autoAddHydrogen(), stack, tr("Toggle hydrogens"));
      SettingsConnector::connect(ui->carbonsVisible, settings->carbonVisible(), stack, tr("Toggle carbon visibility"));
      SettingsConnector::connect(ui->lonePairsVisible, settings->lonePairsVisible(), stack, tr("Simple lone pairs"));
      SettingsConnector::connect(ui->electronSystemsVisible, settings->electronSystemsVisible(), stack, tr("Toggle electron systems"));
      SettingsConnector::connect(ui->chargesVisible, settings->chargeVisible(), stack, tr("Toggle charges"));

      SettingsConnector::connect(ui->atomFont, settings->atomFont(), stack, tr("Change default atom font"));
      SettingsConnector::connect(ui->defaultColor, settings->defaultColor(), stack, tr("Change default color"));

      SettingsConnector::connect(ui->gridColor, settings->gridColor(), stack, tr("Change grid color"));
      SettingsConnector::connect(ui->gridLineWidth, settings->gridLineWidth(), stack, tr("Change grid line width"));
      SettingsConnector::connect(ui->gridHorizontalSpacing, settings->horizontalGridSpacing(), stack, tr("Grid horizontal spacing"));
      SettingsConnector::connect(ui->gridVerticalSpacing, settings->verticalGridSpacing(), stack, tr("Change vertical grid spacing"));

      SettingsConnector::connect(ui->lonePairLineWidth, settings->lonePairLineWidth(), stack, tr("Change default lone pair line width"));
      SettingsConnector::connect(ui->lonePairLength, settings->lonePairLength(), stack, tr("Change default lone pair length"));
      SettingsConnector::connect(ui->radicalDiameter, settings->radicalDiameter(), stack, tr("Change default radical diameter"));
    }
  };

  ScenePropertiesWidget::ScenePropertiesWidget(MolScene *scene) :
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
