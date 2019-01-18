#include "sumformulaindicator.h"

#include <QSet>
#include <sumformula.h>
#include <molscene.h>
#include <molecule.h>

using namespace Molsketch;

SumFormulaIndicator::SumFormulaIndicator(MolScene *scene, QWidget *parent)
  : QLabel(parent),
    scene(scene)
{}

QSet<Molecule*> moleculesFromItems(QList<QGraphicsItem*> items) {
  QSet<Molecule*> result;
  for (auto item : items) {
    if (auto molecule = dynamic_cast<Molecule*>(item)) result += molecule;
    result += moleculesFromItems(item->childItems());
  }
  return result;
}

void SumFormulaIndicator::updateSumFormula() {
  if (!scene) return;
  SumFormula sumFormula;
  for (auto molecule : moleculesFromItems(scene->selectedItems()))
    sumFormula += molecule->sumFormula();
  setText(sumFormula.toHtml());
}
