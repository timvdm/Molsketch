#include "scenepropertieswidget.h"

#include "molscene.h"
#include "ui_scenepropertieswidget.h"

#include "scenesettings.h"
#include "settingsitem.h"
#include "settingsconnector.h"
#include "settingsfacade.h"
#include <QDebug>

namespace Molsketch {

  struct ScenePropertiesWidget::privateData // TODO actually we only need the setup function
  {
    Ui::ScenePropertiesWidget *ui;
    SceneSettings *settings;
    QUndoStack *stack;

    privateData(SceneSettings *settings, QUndoStack *stack)
      : ui(new Ui::ScenePropertiesWidget),
        settings(settings),
        stack(stack)
    {}

    void setup() {
      SettingsConnector::connect(ui->bondLineWidth, settings->bondWidth(), stack, tr("Change bond line width"));
      SettingsConnector::connect(ui->arrowLineWidth, settings->arrowWidth(), stack, tr("Change arrow line width"));
      SettingsConnector::connect(ui->frameLineWidth, settings->frameLineWidth(), stack, tr("Change frame line width"));

      SettingsConnector::connect(ui->bondLength, settings->bondLength(), stack, tr("Change default bond length"));
      SettingsConnector::connect(ui->bondAngle, settings->bondAngle(), stack, tr("Change default bond angle"));


      SettingsConnector::connect(ui->autoHydrogensVisible, settings->autoAddHydrogen(), stack, tr("Toggle hydrogens"));
      SettingsConnector::connect(ui->carbonsVisible, settings->carbonVisible(), stack, tr("Toggle carbon visibility"));
      SettingsConnector::connect(ui->lonePairsVisible, settings->lonePairsVisible(), stack, tr("Toggle automatic lone pairs"));
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

  ScenePropertiesWidget::ScenePropertiesWidget(SceneSettings *settings, QUndoStack *stack, QWidget *parent) :
    QWidget(parent),
    d(new privateData(settings, stack))
  {
    d->ui->setupUi(this);
    d->setup();
  }

  ScenePropertiesWidget::ScenePropertiesWidget(SceneSettings* settings, QWidget *parent)
    : QWidget(parent),
      d(new privateData(settings, nullptr))
  {
    d->ui->setupUi(this);
    d->setup();
  }

  ScenePropertiesWidget::~ScenePropertiesWidget()
  {
    qDebug() << "deleting properties widget" << this;
    delete d->ui;
    delete d;
  }
} // namespace Molsketch
