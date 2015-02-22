#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QUndoCommand>
#include "graphicsitem.h"
#include "molscene.h"
#include "actions/coloraction.h"
#include "actions/linewidthaction.h"
#include <cmath>
#include <QDebug>
#include <actions/rotateaction.h>

namespace Molsketch {

  class movePointCommand : public QUndoCommand
  {
  private:
    int index ;
    QPointF point ;
    graphicsItem *item ;
  public:
    movePointCommand(const int& i,
                     const QPointF& p,
                     graphicsItem *a,
                     QUndoCommand *parent = 0)
      : QUndoCommand(parent),
        index(i),
        point(p), // if index <0: implicitly the position of the first point
        item(a)
    {}
    void redo()
    {
      if (index < 0)
      {
        QPolygonF coords = item->coordinates();
        QPointF shift = point - coords.first();
        point = coords.first();
        coords.translate(shift);
        item->setCoordinates(coords);
      }
      else
        item->swapPoint(index, point) ;
    }
    void undo()
    {
      redo() ;
    }
    bool mergeWith(const QUndoCommand *other)
    {
      const movePointCommand* mpc = dynamic_cast<const movePointCommand*>(other) ;
      if (!mpc) return false ;
      if (mpc->item  != item)  return false ;
      if (mpc->index != index) return false ;
//      if (index < 0) point += mpc->point;
      return true ;
    }
    int id() const { return 5000; } // TODO define command IDs in central place
  };

  class graphicsItem::privateData
  {
  public:
    int selectedPoint;
    privateData() : selectedPoint(-1) {}
  };

  graphicsItem::graphicsItem(QGraphicsItem *parent GRAPHICSSCENESOURCE)
    : QGraphicsItem(parent GRAPHICSSCENEINIT),
      lineWidthScaling(1),
      d(new privateData)
  {
    //    setCacheMode(QGraphicsItem::NoCache);
    setFlags(QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemIsSelectable);
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
  }

  graphicsItem::graphicsItem(const graphicsItem &other GRAPHICSSCENESOURCE)
    : QGraphicsItem(0 GRAPHICSSCENEINIT),
      m_color(other.m_color),
      lineWidthScaling(other.lineWidthScaling),
      d(new privateData)
  {
    setFlags(QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemIsSelectable);
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
  }

  void graphicsItem::setColor(const QColor& color)
  {
    m_color = color ;
  }

  QColor graphicsItem::getColor() const
  {
    return m_color ;
  }

  void graphicsItem::setRelativeWidth(const double &w)
  {
    lineWidthScaling = w ;
  }

  qreal graphicsItem::lineWidth() const
  {
    qreal lw = lineWidthScaling ;
    MolScene *myScene = qobject_cast<MolScene*>(scene()) ;
    if (myScene) lw *= sceneLineWidth(myScene) ;
    return lw ;
  }

  qreal graphicsItem::relativeWidth() const
  {
    return lineWidthScaling ;
  }

  void graphicsItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
  {
    hoverMoveEvent(event); // TODO highlighting
  }

  void graphicsItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
  { // TODO highlighting
    d->selectedPoint = -1 ;
    QPointF eventPosition = event->scenePos() ;
    int i = 0 ;
    double minDistance = INFINITY ;

    foreach(const QPointF& p, coordinates())
    {
      qreal currentDistance = QLineF(eventPosition, p).length() ;
      if (currentDistance < minDistance
          && currentDistance < 5.)
      {
        minDistance = currentDistance ;
        d->selectedPoint = i ;
      }
      ++i ;
    }
    update() ;
  }

  void graphicsItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
  {
    Q_UNUSED(event)
    d->selectedPoint = -1 ;
    update() ;
  }

  void graphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
  {
//    if (event->button() != Qt::LeftButton) return;
//    if (event->modifiers()) return;
    qDebug() << "Button scene pos:" << event->scenePos() << event->buttonDownScenePos(Qt::LeftButton) << "added shift" << event->scenePos() - event->buttonDownScenePos(Qt::LeftButton);
    if (d->selectedPoint < 0)
    {
      QPointF shift = event->scenePos() - event->lastScenePos();
      QList<QGraphicsItem*> selection;
      if (scene()) selection = scene()->selectedItems();
      if (!selection.empty())
      {
        attemptBeginMacro(QObject::tr("Move"));
        foreach(QGraphicsItem* gItem, selection)
        {
          graphicsItem *item = dynamic_cast<graphicsItem*>(gItem);
          if (!item) continue;
          attemptUndoPush(new movePointCommand(-1,
                                               item->firstPoint() + shift,
                                               item));
        }
        attemptEndEndMacro();
      }
      else
        attemptUndoPush(new movePointCommand(d->selectedPoint,
                                             shift + firstPoint(),
                                             this));
    }
    else
      attemptUndoPush(new movePointCommand(d->selectedPoint,
                                           event->scenePos(),
                                           this));
    event->accept();
  }

  void graphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
  {
    QGraphicsItem::mousePressEvent(event);
    if (event->button() != Qt::LeftButton) return;
    if (event->modifiers()) return;
    event->accept();
  }

  void graphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
  {
    QGraphicsItem::mouseReleaseEvent(event);
    event->accept();
  }

  void graphicsItem::readAttributes(const QXmlStreamAttributes &attributes)
  {
    readGraphicAttributes(attributes) ;
    setColor(QColor(attributes.value("colorR").toString().toInt(),
                    attributes.value("colorG").toString().toInt(),
                    attributes.value("colorB").toString().toInt())) ;
    if (attributes.hasAttribute("scalingParameter"))
      lineWidthScaling = attributes.value("scalingParameter").toString().toFloat() ;
    else
      lineWidthScaling = 1.0 ;
    if (attributes.hasAttribute("coordinates"))
    {
      QPolygonF coords;
      foreach(const QString& coordinate, attributes.value("coordinates").toString().split(";"))
        coords << QPointF(coordinate.section(",",0,0).toDouble(), coordinate.section(",",1,1).toDouble());
      setCoordinates(coords);
    }
  }

  QXmlStreamAttributes graphicsItem::xmlAttributes() const
  {
    QXmlStreamAttributes attributes = graphicAttributes() ;
    attributes.append("colorR", QString::number(getColor().red())) ;
    attributes.append("colorG", QString::number(getColor().green())) ;
    attributes.append("colorB", QString::number(getColor().blue())) ;
    attributes.append("scalingParameter", QString::number(lineWidthScaling));
    QStringList coords;
    foreach(const QPointF& coordinate, coordinates())
      coords << QString::number(coordinate.x()) << "," << QString::number(coordinate.y());
    attributes.append("coordinates", coords.join(";"));
    return attributes ;
  }

  void graphicsItem::attemptUndoPush(QUndoCommand *command)
  {
    MolScene *molscene = dynamic_cast<MolScene*>(scene());
    if (!molscene || !molscene->stack())
    {
      command->redo();
      delete command;
    }
    else
      molscene->stack()->push(command) ;
  }

  void graphicsItem::attemptBeginMacro(const QString &text)
  {
    MolScene *molscene = dynamic_cast<MolScene*>(scene());
    if (!molscene || !molscene->stack()) return;
    molscene->stack()->beginMacro(text);
  }

  void graphicsItem::attemptEndEndMacro()
  {
    MolScene *molscene = dynamic_cast<MolScene*>(scene());
    if (!molscene || !molscene->stack()) return;
    molscene->stack()->endMacro();
  }

  int graphicsItem::selectedPoint() const
  {
    return d->selectedPoint;
  }

  void graphicsItem::prepareItemContextMenu(QMenu *contextMenu)
  {
    prepareContextMenu(contextMenu);
  }

  qreal graphicsItem::sceneLineWidth(MolScene *scene) const
  {
    return scene->bondWidth() ;
  }

  void graphicsItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
  {
    QMenu contextMenu;
    prepareItemContextMenu(&contextMenu);
    foreach(QAction* action, contextMenu.actions())
    {
      abstractItemAction *itemAction = dynamic_cast<abstractItemAction*>(action);
      if (itemAction) itemAction->setItem(this);
    }

    contextMenu.exec(event->screenPos()) ;
    event->accept();
  }

  arrowGraphicsItem::arrowGraphicsItem(QGraphicsItem *parent GRAPHICSSCENESOURCE )
    : graphicsItem(parent GRAPHICSSCENEINIT )
  {}

  qreal arrowGraphicsItem::sceneLineWidth(MolScene *scene) const
  {
    return scene->arrowLineWidth() ;
  }

  void graphicsItem::setCoordinate(const int &index, const QPointF &p)
  {
    QPolygonF oldCoords(coordinates());
    if (index >= 0 && index < oldCoords.size())
      oldCoords[index] = p;
    setCoordinates(oldCoords);
  }

  QPointF graphicsItem::getPoint(const int &index) const
  {
    QPolygonF oldCoords(coordinates());
    if (index < 0 || index >= coordinateCount()) return QPointF();
    return coordinates().at(index);
  }

  QPointF graphicsItem::lastPoint() const
  {
    int cc = coordinateCount();
    if (!cc) return QPointF();
    return getPoint(cc-1);
  }

  QPointF graphicsItem::firstPoint() const
  {
    if (coordinateCount()) return coordinates().first();
    return QPointF();
  }

  int graphicsItem::coordinateCount() const
  {
    return coordinates().size();
  }

  void graphicsItem::swapPoint(const int &index, QPointF &p)
  {
    int cc = coordinateCount();
    if (index < 0 || index >= cc) return;
    QPolygonF oldCoords(coordinates());
    qSwap(p,oldCoords[index]);
  }

  void graphicsItem::prepareContextMenu(QMenu *contextMenu)
  {
    colorAction *caction = scene()->findChild<colorAction*>();
    lineWidthAction *lwaction = scene()->findChild<lineWidthAction*>();
    rotateAction *raction = scene()->findChild<rotateAction*>();
    if (caction)  contextMenu->addAction(caction);
    if (lwaction) contextMenu->addAction(lwaction);
    if (raction)  contextMenu->addAction(raction);
  }

} // namespace
