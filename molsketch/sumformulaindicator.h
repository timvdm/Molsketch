#ifndef SUMFORMULAINDICATOR_H
#define SUMFORMULAINDICATOR_H

#include <QLabel>

namespace Molsketch {
  class MolScene;
}

class SumFormulaIndicator : public QLabel
{
  Q_OBJECT
private:
  Molsketch::MolScene *scene;
public:
  explicit SumFormulaIndicator(Molsketch::MolScene *scene, QWidget *parent = nullptr);
public slots:
  void updateSumFormula();
};

#endif // SUMFORMULAINDICATOR_H
