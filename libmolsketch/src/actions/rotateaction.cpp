#include "rotateaction.h"
#include "QUndoCommand"
#include "molscene.h"
#include "transformcommand.h"

namespace Molsketch {

  transformAction::transformAction(MolScene *scene) :
    genericAction(scene),
    m_rotationItem(0)
  {}

  void transformAction::mousePressEvent(QGraphicsSceneMouseEvent *event)
  {
    if (event->button() != Qt::LeftButton) return ;
    QGraphicsItem * item = scene()->itemAt(event->buttonDownScenePos(Qt::LeftButton)
                                       #if QT_VERSION >= 0x050000
                                           , QTransform()
                                       #endif
                                           );
    if (!item) return;

    if (item->type() == graphicsItem::AtomType
        || item->type() == graphicsItem::BondType)
      item = item->parentItem();
    m_rotationItem = dynamic_cast<coordinateItem*>(item) ;
    if (!m_rotationItem) return;

    m_originalLine = QLineF(item->mapToScene(item->boundingRect().center()),
                            event->buttonDownScenePos(Qt::LeftButton)) ;
    event->accept();
  }

  void transformAction::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
  {
    if (!m_rotationItem) return;

    // Calculate the rotation angle
    QLineF newLine(m_originalLine.p1(), event->scenePos()) ;
    transformCommand *cmd = new transformCommand(m_rotationItem,
                                                 generateTransform(m_originalLine,
                                                                   newLine),
                                                 m_originalLine.p1());
    cmd->setText(text());
    scene()->stack()->push(cmd) ;
    m_originalLine = newLine ;
    event->accept();
  }

  void transformAction::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
  {
    Q_UNUSED(event)
    m_rotationItem = 0 ;
    event->accept();
  }

  rotateAction::rotateAction(MolScene *scene)
    : transformAction(scene)
  {
    setIcon(QIcon(":images/rotate.png")) ;
    setText(tr("Rotate")) ;
  }

  QTransform rotateAction::generateTransform(const QLineF &originalLine, const QLineF &currentLine)
  {
    return QTransform().rotate(currentLine.angleTo(originalLine)) ;
  }

  translateAction::translateAction(MolScene *scene)
    : transformAction(scene)
  {
    setIcon(QIcon(":images/translate.png")) ;
    setText(tr("Move")) ;
  }

  QTransform translateAction::generateTransform(const QLineF &originalLine, const QLineF &currentLine)
  {
    QPointF diff = currentLine.p2() - originalLine.p2() ;
    return QTransform().translate(diff.x(), diff.y()) ;
  }

} // namespace Molsketch
