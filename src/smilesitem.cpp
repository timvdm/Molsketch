#include "smilesitem.h"
#include "molscene.h"
#include "mimemolecule.h"

#include <QPainter>
#include <QGraphicsSceneDragDropEvent>
#include <QDebug>

namespace Molsketch {

  SmilesItem::SmilesItem() : ItemPlugin(), m_molecule(0), m_rect(QRectF(0, 0, 150, 100))
  {
  }

  SmilesItem::~SmilesItem()
  {
  }

  QRectF SmilesItem::boundingRect() const
  {
    if (m_molecule)
      return m_rect;
    else
      return defaultBoundingRect();
  }

  void SmilesItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
  {
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if (m_molecule) {
      QFontMetrics fm = painter->fontMetrics();
      QString smiles = m_molecule->smiles();
      m_rect = QRectF(0, 0, fm.width(smiles), fm.height());
      painter->drawText(m_rect, Qt::AlignCenter | Qt::TextDontClip, smiles);
    } else {
      paintDefault(painter);
    }
  }

  void SmilesItem::dropEvent(QGraphicsSceneDragDropEvent *event)
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

  SmilesItemFactory::SmilesItemFactory()
  {
    instanceList().append(this);
    qDebug() << instanceList().size();
  }

}

