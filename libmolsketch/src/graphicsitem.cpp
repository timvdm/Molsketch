/***************************************************************************
 *   Copyright (C) 2015 Hendrik Vennekate                                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QUndoCommand>
#include "graphicsitem.h"
#include "molscene.h"
#include "actions/coloraction.h"
#include "actions/linewidthaction.h"
#if QT_VERSION >= 0x050000
#include <QtMath>
#else
#include <QtCore/qmath.h>
#endif
#include <QDebug>
#include <actions/rotateaction.h>

namespace Molsketch {

  class movePointCommand : public QUndoCommand // TODO eliminate transformcommand or merge
  {
  private:
    int index ;
    QPointF shift ;
    QSet<graphicsItem *> items ;
  public:
    movePointCommand(const int& i,
                     const QPointF& p,
                     QSet<graphicsItem *> a,
                     QUndoCommand *parent = 0)
      : QUndoCommand(parent),
        index(i),
        shift(p), // if index <0: implicitly the position of the first point
        items(a)
    {
      setText(QObject::tr("Move command"));
    }
    void redo()
    {
      for(auto item : items)
        item->movePointBy(shift, index);

      shift *= -1;
    }
    void undo()
    {
      redo() ;
    }
    bool mergeWith(const QUndoCommand *other)
    {
      const movePointCommand* mpc = dynamic_cast<const movePointCommand*>(other) ;
      if (!mpc) return false ;
      if (mpc->items != items)  return false ;
      if (mpc->index != index) return false ;
      shift += mpc->shift;
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
    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
  }

  graphicsItem::graphicsItem(const graphicsItem &other GRAPHICSSCENESOURCE)
    : QGraphicsItem(0 GRAPHICSSCENEINIT),
      m_color(other.m_color),
      lineWidthScaling(other.lineWidthScaling),
      d(new privateData)
  {
    setFlags(QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemIsSelectable);
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
  }

  graphicsItem::~graphicsItem()
  {
    delete d;
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

    foreach(const QPointF& p, moveablePoints())
    {
      qreal currentDistance = QLineF(eventPosition, p).length() ;
      if (currentDistance < minDistance
          && currentDistance < pointSelectionDistance()) // TODO use pointSelectionDistance as starting value of minDistance
      {
        minDistance = currentDistance ;
        d->selectedPoint = i ;
      }
      ++i ;
    }
    event->setAccepted(-1 != d->selectedPoint);
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
    QPointF shift = event->scenePos() - event->lastScenePos();
    QSet<graphicsItem*> selection;
    if (d->selectedPoint < 0 && scene())
      foreach(QGraphicsItem* gItem, scene()->selectedItems())
        selection << dynamic_cast<graphicsItem*>(gItem);
    selection.remove(0);
    if (selection.isEmpty())
      selection << this;
    // Snap shift to grid
    MolScene *sc = qobject_cast<MolScene*>(scene());
    if (sc && sc->snappingToGrid())
    {
      if (d->selectedPoint < 0)
      {
        QRectF br;
        foreach(graphicsItem* item, selection)
          if (item->parentItem())
            br |= item->mapToParent(item->boundingRect()).boundingRect();
          else
            br |= item->boundingRect();
        shift = sc->snapToGrid(event->scenePos()) - br.center();
      }
      else
        shift = sc->snapToGrid(event->scenePos()) - getPoint(d->selectedPoint);
    }

    attemptUndoPush(new movePointCommand(d->selectedPoint,
                                         shift,
                                         selection));
    event->accept();
  }

  void graphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
  { // TODO this function seems weird
    QGraphicsItem::mousePressEvent(event);
    event->accept(); // to prevent scene from clearing selection
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
      coords << QString::number(coordinate.x()) + "," + QString::number(coordinate.y());
    attributes.append("coordinates", coords.join(";"));
    return attributes ;
  }

  void graphicsItem::attemptUndoPush(QUndoCommand *command)
  {
    if (!command) return;
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
      AbstractItemAction *itemAction = dynamic_cast<AbstractItemAction*>(action);
      if (itemAction) itemAction->setItem(this);
    }

    qDebug() << "looking for scene";
    MolScene *sc = dynamic_cast<MolScene*>(scene());
    if (sc) {
      qDebug() << "adding scene context menu";
      contextMenu.addMenu(sc->sceneMenu());
    }

    contextMenu.exec(event->screenPos()) ;
    qDebug() << "removing item" ;
    foreach(QAction* action, contextMenu.actions())
    {
      AbstractItemAction *itemAction = dynamic_cast<AbstractItemAction*>(action);
      if (itemAction) itemAction->removeItem(this);
    }
    event->accept();
  }

  QVariant graphicsItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
  {
    QVariant retVal = QGraphicsItem::itemChange(change, value);
    if (QGraphicsItem::ItemSelectedChange == change)
    {
      if (parentItem() && parentItem()->isSelected())
        retVal.setValue(false);
      if (value.toBool())
        foreach(QGraphicsItem* child, childItems())
          child->setSelected(false);
    }
    return retVal;
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

  QPolygonF graphicsItem::moveablePoints() const
  {
    return coordinates();
  }

  void graphicsItem::movePointBy(const QPointF &offset, int pointIndex)
  {
    if (-1 == pointIndex)
    {
      moveItemBy(offset);
      return;
    }
    setCoordinate(pointIndex, offset + getPoint(pointIndex));
  }

  void graphicsItem::moveItemBy(const QPointF &offset)
  {
    QPolygonF coords(coordinates());
    coords.translate(offset);
    setCoordinates(coords);
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

  QWidget* graphicsItem::getPropertiesWidget()
  {
    return 0;
  }

  qreal graphicsItem::pointSelectionDistance() const
  {
    return 5.;
  }

} // namespace
