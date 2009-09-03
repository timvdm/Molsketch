#include "smilesitem.h"
#include "molscene.h"
#include "mimemolecule.h"

#include <QPainter>
#include <QGraphicsSceneDragDropEvent>
#include <QDebug>

namespace Molsketch {

  SmilesItem::SmilesItem() : m_molecule(0), m_rect(QRectF(0, 0, 150, 100))
  {
    setAcceptDrops(true);
    setFlags(QGraphicsItem::ItemIsMovable|QGraphicsItem::ItemIsSelectable);
  }

  QRectF SmilesItem::boundingRect() const
  {
    return m_rect;
  }

  void SmilesItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
  {
    if (m_molecule) {
      QFontMetrics fm = painter->fontMetrics();
      QString smiles = m_molecule->smiles();
      m_rect = QRectF(0, 0, fm.width(smiles), fm.height());
      painter->drawText(m_rect, Qt::AlignCenter | Qt::TextDontClip, smiles);
    } else {
      painter->drawRect(boundingRect());
      painter->drawText(boundingRect(), Qt::AlignCenter, "drag molecule here...");
    }
  }

  void SmilesItem::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
  {
    qDebug() << "dragEnterEvent";
  }

  void SmilesItem::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
  {
    qDebug() << "dragMoveEvent";
  }

  void SmilesItem::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
  {
    qDebug() << "dragLeaveEvent";
  }

  void SmilesItem::dropEvent(QGraphicsSceneDragDropEvent *event)
  {
    const MimeMolecule *mimeMol = dynamic_cast<const MimeMolecule*>(event->mimeData());
    if (!mimeMol)
      return;

    m_molecule = mimeMol->molecule();

    m_molecule->addToGroup(this);

    //QRectF rect = m_molecule->boundingRect();
    //m_rect = QRectF(0, 0, rect.width(), 30);
    //setPos(rect.bottomLeft());

    if (scene())
      scene()->update();
  }


}
