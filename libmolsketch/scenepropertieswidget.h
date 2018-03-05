#ifndef MOLSKETCH_SCENEPROPERTIESWIDGET_H
#define MOLSKETCH_SCENEPROPERTIESWIDGET_H

#include "scenesettings.h"
#include <QWidget>

namespace Molsketch {

  class MolScene;

  namespace Ui {
    class ScenePropertiesWidget;
  }

  class ScenePropertiesWidget : public QWidget
  {
    Q_OBJECT
  public:
    explicit ScenePropertiesWidget(MolScene *scene);
    ~ScenePropertiesWidget();
  private:
    class privateData;
    privateData *d;
  };


} // namespace Molsketch
#endif // MOLSKETCH_SCENEPROPERTIESWIDGET_H
