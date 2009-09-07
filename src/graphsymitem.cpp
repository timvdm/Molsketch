#include "graphsymitem.h"
#include "molscene.h"
#include "mimemolecule.h"

#include <QPainter>
#include <QGraphicsSceneDragDropEvent>
#include <QDebug>

#include <openbabel/graphsym.h>

namespace Molsketch {

  GraphSymItem::GraphSymItem() : m_molecule(0), m_rect(QRectF(0, 0, 150, 100))
  {
    setAcceptDrops(true);
    setFlags(QGraphicsItem::ItemIsMovable|QGraphicsItem::ItemIsSelectable);
  }

  QRectF GraphSymItem::boundingRect() const
  {
    if (m_molecule)
      return m_rect;
    else
      return defaultBoundingRect();
  }

  void GraphSymItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
  {
    painter->save();
    painter->setPen(Qt::red);
    if (m_molecule) {
      const QList<Atom*> &atoms = m_molecule->atoms();

      OpenBabel::OBMol *obmol = m_molecule->OBMol();
      std::vector<unsigned int> symmetry_classes;
      OpenBabel::OBGraphSym graphsym(obmol);
      graphsym.GetSymmetry(symmetry_classes);

      for (int i = 0; i < atoms.size(); ++i) {
        painter->drawText(mapFromItem(m_molecule, atoms[i]->pos()), QString::number(symmetry_classes.at(i)));
      }

      QFontMetrics fm = painter->fontMetrics();
      m_rect = QRectF(0, 0, fm.width(QObject::tr("Symmetry Classes")), fm.height());
      painter->drawText(m_rect, Qt::AlignCenter | Qt::TextDontClip, QObject::tr("Symmetry Classes"));
    } else {
      paintDefault(painter);
    }
    painter->restore();
  }

  void GraphSymItem::dropEvent(QGraphicsSceneDragDropEvent *event)
  {
    const MimeMolecule *mimeMol = dynamic_cast<const MimeMolecule*>(event->mimeData());
    if (!mimeMol)
      return;

    m_molecule = mimeMol->molecule();
    QRectF rect = m_molecule->boundingRect();
    setPos(rect.bottomLeft());

    m_molecule->addToGroup(this);

    if (scene())
      scene()->update();
  }


}
