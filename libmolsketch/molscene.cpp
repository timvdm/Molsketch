/***************************************************************************
 *   Copyright (C) 2007-2008 by Harm van Eersel                            *
 *   Copyright (C) 2009 Tim Vandermeersch                                  *
 *   Copyright (C) 2009 by Nicola Zonta                                    *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#include <iostream>

#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QClipboard>
#include <QApplication>
#include <QListWidgetItem>
#include <QTableWidgetItem>
#include <QKeyEvent>
#include <QUndoStack>
#include <QMenu>
#include <QProcess>
#include <QDir>
#include <QLibrary>
#include <QMessageBox>
#include <QSvgGenerator>
#include <QPushButton>
#include <QBuffer>
#if QT_VERSION < 0x050000
#include <QtMath>
#include <QDesktopServices>
#else
#include <QtCore/qmath.h>
#include <QStandardPaths>
#endif
#include <QDebug>
#include <QDockWidget>
#include <QLabel>
#include <QMainWindow>
#include <QPair>
#include <QVBoxLayout>

#include "molscene.h"

#include "element.h"
#include "atom.h"
#include "bond.h"
#include "molecule.h"
#include "commands.h"
#include "mimemolecule.h"
#include "TextInputItem.h"
#include "math2d.h"
#include "grid.h"
#include "molview.h"

#include "arrow.h"
#include "actions/abstractitemaction.h"
#include "actions/genericaction.h"

#include "frame.h"
#include "scenesettings.h"
#include "textitem.h"
#include "constants.h"
#include "settingsfacade.h"
#include "settingsitem.h"
#include "scenepropertieswidget.h"

#ifdef QT_STATIC_BUILD
inline void initToolBarIcons() { Q_INIT_RESOURCE(toolicons); }
#endif

namespace Molsketch {

#ifdef QT_STATIC_BUILD
  void initToolBarIconsMsk() { initToolBarIcons(); }
#endif

  const QString MolScene::mouseWheelForCyclingTools = "mouse-wheel-cycle-tools";

  struct MolScene::privateData
  {
    QGraphicsRectItem *selectionRectangle;
    TextInputItem *inputItem;
    Grid *grid;
    MolScene *scene;
    QUndoStack *stack;
    SceneSettings *settings;
    graphicsItem* dragItem;

    privateData(MolScene* scene, SceneSettings* settings)
      : selectionRectangle(new QGraphicsRectItem),
        inputItem(new TextInputItem),
        grid(new Grid(settings)),
        scene(scene),
        stack(new QUndoStack(scene)),
        settings(settings),
        dragItem(0)
    {
      selectionRectangle->setPen(QPen(Qt::blue,0,Qt::DashLine));
      selectionRectangle->setZValue(INFINITY);
      connect(scene, SIGNAL(sceneRectChanged(QRectF)), scene, SLOT(updateGrid(QRectF)));

      connect(stack, SIGNAL(indexChanged(int)), scene, SIGNAL(documentChange()));
      connect(stack, SIGNAL(indexChanged(int)), scene, SLOT(update()));
      connect(stack, SIGNAL(indexChanged(int)), scene, SLOT(updateAll())) ;
    }

    ~privateData()
    {
//      if (inputItem && !inputItem->scene()) // TODO compare with this scene
//        delete inputItem; // TODO should clean up this item...
//      delete selectionRectangle; // TODO why?
      if (!grid->scene()) delete grid;
      if (!selectionRectangle->scene()) delete selectionRectangle;
      delete stack;
    }

    bool gridOn()const { return grid->scene(); }

    void moveDragItem(QGraphicsSceneDragDropEvent* event) {
      if (!dragItem) return;
      dragItem->moveItemBy(event->scenePos() - dragItem->boundingRect().center());
    }
  };

  using namespace Commands;

  MolScene::MolScene(QObject* parent)
    : MolScene(nullptr, parent)
  {}

  MolScene::MolScene(SceneSettings* settings, QObject *parent)
    : QGraphicsScene(parent),
      m_editMode(MolScene::DrawMode),
      m_renderMode(RenderLabels),
      d(new privateData(this, nullptr == settings ? new SceneSettings(SettingsFacade::transientSettings(), this) : settings))
  {
    setSceneRect(QRectF(-5000,-5000,10000,10000));
    connect(this, &MolScene::selectionChanged, this, &MolScene::selectionSlot);
    connect(QApplication::clipboard(), &QClipboard::dataChanged, this, &MolScene::clipboardChanged);

    // TODO - add text item
    // - subclass QGraphicsTextItem?
    // - make movable
    // - make serializable
    // - make readable
    // - create action
    // (- extract parent item from graphicsItem)
//    QGraphicsTextItem *textItem = new QGraphicsTextItem();
//    textItem->setHtml("some <b>text</b> example");
//    textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
//    addItem(textItem);
  }

  MolScene::~MolScene() {
    for(QObject *child : QObject::children())
      if (QAction *action = dynamic_cast<QAction*>(child))
        action->setChecked(false);
    delete d;
  }

  SceneSettings *MolScene::settings() const {
    return d->settings;
  }

  QFont MolScene::getAtomFont() const {
    return d->settings->atomFont()->get(); // FIXME connect signal/slot
  }

  qreal MolScene::getRadicalDiameter() const {
    return d->settings->radicalDiameter()->get(); // FIXME connect signal/slot
  }

  qreal MolScene::getLonePairLength() const {
    return d->settings->lonePairLength()->get(); // FIXME connect signal/slot
  }

  qreal MolScene::getLonePairLineWidth() const {
    return d->settings->lonePairLineWidth()->get(); // FIXME connect signal/slot
  }

  QWidget *MolScene::getPropertiesWidget() {
    auto result = new ScenePropertiesWidget(d->settings, d->stack);
    result->setEnabled(true);
    return result;
  }

  QString MolScene::mimeType()
  {
    return moleculeMimeType;
  }

  void MolScene::setEditMode(int mode)
  {
    // TODO
        // Set the new edit mode and signal other components
        m_editMode = mode;
        emit editModeChange( mode );
  }

  void MolScene::cut() {
        if (selectedItems().isEmpty()) return;
        copy();
        d->stack->beginMacro(tr("cutting items"));
        foreach (QGraphicsItem* item, selectedItems())
          ItemAction::removeItemFromScene(item);
        d->stack->endMacro();
  }

  void MolScene::copy() {
    if (selectedItems().isEmpty()) return;
    QRectF selectionRect;
    QList<const graphicsItem*> items;
    for (auto item : selectedItems()) {
      selectionRect |= item->boundingRect();
      items << dynamic_cast<graphicsItem*>(item);
    }

    // Clear selection
    QList<QGraphicsItem*> selList(selectedItems());
    clearSelection();
    QMimeData *mimeData = new QMimeData;
    mimeData->setImageData(renderImage(selectionRect));
    mimeData->setData(moleculeMimeType, graphicsItem::serialize(items));
    QApplication::clipboard()->setMimeData(mimeData);

    foreach(QGraphicsItem* item, selList) item->setSelected(true);
  }

  void MolScene::paste() {
    const QMimeData *mimeData = QApplication::clipboard()->mimeData();
    if (!mimeData->hasFormat(moleculeMimeType)) return;
    d->stack->beginMacro(tr("Paste"));
    for (auto newItem : graphicsItem::deserialize(mimeData->data(moleculeMimeType)))
      ItemAction::addItemToScene(newItem, this);
    d->stack->endMacro();
  }

#ifdef QT_DEBUG
  void MolScene::debugScene()
  {
    QtMessageHandler originalMessageHander = qInstallMessageHandler(0);
    qDebug() << "================= Scene debug =================";
    foreach(QGraphicsItem *item, items())
    {
      qDebug() << "Item:" << item
               << "Type:" << item->type()
               << "Pos:"  << item->pos()
               << "Scene Pos:" << item->scenePos()
               << "Bounds:" << item->boundingRect()
               << "Children:" << item->childItems() ;
    }
    qDebug() << "============== Undo stack debug ===============";
    qDebug() << "position:" << stack()->index();
    for (int i = 0 ; i < stack()->count() ; ++i)
    {
      const QUndoCommand* command = stack()->command(i);
      qDebug() << i << command << command->id() << command->text();
    }
    qDebug() << "===============================================";
    qInstallMessageHandler(originalMessageHander);
  }
#endif

  void MolScene::clear()
  {
    clearSelection();
    d->stack->clear();
    SceneSettings *settings = d->settings;
    delete d; // TODO this seems a little extreme and dangerous!
    QGraphicsScene::clear();
    d = new privateData(this, settings);
  }

  QByteArray MolScene::toSvg()
  {
    QList<QGraphicsItem*> selection(selectedItems());
    clearSelection();
    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QBuffer::WriteOnly);
    QSvgGenerator svgGenerator;
    svgGenerator.setTitle(tr("MolsKetch Drawing"));
    QRectF bounds(itemsBoundingRect());
    svgGenerator.setSize(bounds.size().toSize()); // TODO round up
    svgGenerator.setViewBox(bounds);
    svgGenerator.setOutputDevice(&buffer);
    QPainter painter;
    painter.begin(&svgGenerator);
    render(&painter, bounds, bounds);
    painter.end();
    buffer.close();
    // TODO reselect items
    return ba;
  }


  QImage MolScene::renderImage(const QRectF &rect)
  {
        // Creating an image
        QImage image(int(rect.width()),int(rect.height()),QImage::Format_RGB32);
        image.fill(QColor("white").rgb());

        // Creating and setting the painter
        QPainter painter(&image);
        painter.setRenderHint(QPainter::Antialiasing);

        // Rendering in the image and saving to file
        render(&painter,QRectF(0,0,rect.width(),rect.height()),rect);

        return image;
  }

  void MolScene::addMolecule(Molecule* mol) // TODO decommission this
  {
        Q_CHECK_PTR(mol);
        if (!mol) return;
        d->stack->beginMacro(tr("add molecule"));
        ItemAction::addItemToScene(mol, this);
        if (mol->canSplit()) {
          for(Molecule* molecule : mol->split())
            ItemAction::addItemToScene(molecule, this);
          ItemAction::removeItemFromScene(mol);
        }
        d->stack->endMacro();
  }

  void MolScene::selectAll()
  {
        // Switch to move mode to make selection posible
        setEditMode(MolScene::MoveMode);

        // Clear any previous selection
        clearSelection();

        // Mark all atoms as selected
        foreach (QGraphicsItem* item, items())
        {
          if (item->type() == Molecule::Type || item->type() == Arrow::Type)
                item->setSelected(true);
        }
  }


  void MolScene::setHoverRect( QGraphicsItem* item )
  {
        if (item)
        {
          m_hoverRect->setPath(item->shape());
          m_hoverRect->setPos(item->scenePos());
          addItem(m_hoverRect);
        }
        else
          removeItem(m_hoverRect);
  }

  int MolScene::editMode() const
  {
    return m_editMode;
  }

  qreal MolScene::bondAngle() const {
    return d->settings->bondAngle()->get(); // TODO replace with signals/slots
  }

  MolScene::RenderMode MolScene::renderMode() const
  {
        return m_renderMode;
  }

  void MolScene::setRenderMode(MolScene::RenderMode mode)
  {
        m_renderMode = mode;
  }

  QPointF MolScene::snapToGrid(const QPointF &point, bool force)
  {
    if (!d->gridOn() && !force) return point;
    return d->grid->alignPoint(point);
  }

  bool MolScene::snappingToGrid() const
  {
    return d->gridOn();
  }

  bool MolScene::cyclingByMouseWheelEnaled() const
  {
    if (d->settings->getMouseWheelMode() == SceneSettings::Unset) {
      QMessageBox promptForMouseWheelUsage;
      promptForMouseWheelUsage.setWindowTitle(tr("Mouse wheel configuration"));
      promptForMouseWheelUsage.setText(tr("Mouse wheel use has not been configured."
                                          "Should the wheel be used to zoom, "
                                          "or to cycle tool settings?"));
      QAbstractButton *zoomButton = promptForMouseWheelUsage.addButton(tr("Use to zoom"), QMessageBox::YesRole);
      QAbstractButton *cycleButton = promptForMouseWheelUsage.addButton(tr("Use to cycle tool options"), QMessageBox::NoRole);
      promptForMouseWheelUsage.exec();
      if (promptForMouseWheelUsage.clickedButton() == zoomButton)
        d->settings->setMouseWheelMode(SceneSettings::Zoom);
      if (promptForMouseWheelUsage.clickedButton() == cycleButton)
        d->settings->setMouseWheelMode(SceneSettings::CycleTools);
    }
    return d->settings->getMouseWheelMode() == SceneSettings::CycleTools;
  }

  void MolScene::setGrid(bool on)
  {
    if (on) addItem(d->grid);
    else removeItem(d->grid);
  }

  XmlObjectInterface *MolScene::produceChild(const QString &childName, const QString &type)
  {
    XmlObjectInterface *object = 0 ;
    if ("frame" == childName) object = new Frame;
    if (childName == "molecule") // TODO move those names to their classes.
      object = new Molecule;
    if (childName == "arrow")
      object = new Arrow;
    if (childName == "object")
    {
      if (type == "ReactionArrow") object = new Arrow ;
      if (type == "MechanismArrow") object = new Arrow ;
    }
    if ("textItem" == childName) object = new TextItem;
    if (d->settings->xmlName() == childName) object = d->settings;
    if (QGraphicsItem* item = dynamic_cast<QGraphicsItem*>(object)) // TODO w/o dynamic_cast
      addItem(item) ;
    return object ;
  }

  QList<const XmlObjectInterface *> MolScene::children() const
  {
        QList<const XmlObjectInterface*> childrenList ;
        childrenList << d->settings;
        foreach(QGraphicsItem* item, items())
        {
          if (item && item->parentItem()) continue;
          XmlObjectInterface *object = dynamic_cast<XmlObjectInterface*>(item);
          if (object) childrenList << object;
        }
        return childrenList;
  }

  void MolScene::readAttributes(const QXmlStreamAttributes &attributes)
  {
    foreach(const QXmlStreamAttribute& attribute, attributes) // TODO remove
      setProperty(attribute.name().toLatin1(), attribute.value().toString());
    clear();
    d->settings->setFromAttributes(attributes);
  }

  QXmlStreamAttributes MolScene::xmlAttributes() const
  {
    QXmlStreamAttributes attributes;
    foreach (const QByteArray& name, dynamicPropertyNames()) // TODO handle int, double, bool appropriately
      attributes.append(name, property(name).toString());
    return attributes;
  }

  Molecule* MolScene::moleculeAt(const QPointF &pos)
  {
        // Check if there is a molecule at this position
        foreach(QGraphicsItem* item,items(pos))
          if (item->type() == Molecule::Type) return dynamic_cast<Molecule*>(item);

        // Else return NULL
        return 0;

  }

  TextInputItem *MolScene::inputItem()
  {
    return d->inputItem;
  }

  QList<Atom *> MolScene::atoms() const
  {
    QList<Atom*> result;
    foreach(auto item, items())
    {
      Atom* atom = dynamic_cast<Atom*>(item);
      if (atom) result << atom;
    }
    return result;
  }

  void MolScene::selectionSlot() {
    foreach(AbstractItemAction* itemAction, findChildren<AbstractItemAction*>())
      itemAction->setItems(selectedItems());
    emit copyAvailable(!selectedItems().empty());
  }

  void MolScene::clipboardChanged() {
    const QMimeData *clipboardContent = QApplication::clipboard()->mimeData();
    if (clipboardContent) qDebug() << "Clipboard types available:" << clipboardContent->formats();
    emit pasteAvailable(clipboardContent && clipboardContent->hasFormat(Molsketch::moleculeMimeType));
  }

  void MolScene::updateGrid(const QRectF& newSceneRect)
  {
    d->grid->update(newSceneRect);
  }

  Atom* MolScene::atomAt(const QPointF &pos) // TODO consider replacing with itemAt()
  {
    foreach(Atom* atom, atoms())
      if (atom->scenePos() == pos)
        return atom;
    return 0;
  }

  Atom* MolScene::atomNear(const QPointF &pos, qreal tolerance)
  {
    Atom* pickedAtom = atomAt(pos);
    if (pickedAtom) return pickedAtom;
    foreach(auto atom, atoms())
    {
      qreal newDistance = QLineF(atom->scenePos(), pos).length();
      if (newDistance < tolerance)
      {
          pickedAtom = atom;
          tolerance = newDistance;
      }
    }
    return pickedAtom;
  }

  Bond* MolScene::bondAt(const QPointF &pos)
  {
        foreach( QGraphicsItem* item, items(pos))
          if (item->type() == Bond::Type) return dynamic_cast<Bond*>(item);

        return 0;
  }

  void MolScene::updateAll()
  {
    invalidate() ;
    update() ;
  }

  void MolScene::keyPressEvent(QKeyEvent* keyEvent)
  {
    // execute default behaviour (needed for text tool)
    keyEvent->ignore();
    QGraphicsScene::keyPressEvent(keyEvent);
    update();
    if (keyEvent->isAccepted()) return;
    if (keyEvent->key() == Qt::Key_Escape)
    {
        keyEvent->accept();
        clearSelection();
        for (genericAction* action : sceneActions())
          if (action->isChecked())
            action->setChecked(false);
    }
  }

  void MolScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
  {
        QMenu contextMenu;
        qDebug() << "context menu";
        foreach(QGraphicsItem* qgItem, selectedItems()) {
          graphicsItem *item = dynamic_cast<graphicsItem*>(qgItem);
          if (!item) continue;
          item->prepareContextMenu(&contextMenu);
        }

        qDebug() << "-------- context menu for no of items:" << selectedItems().size();
        if (contextMenu.actions().empty()) return;
        contextMenu.exec(event->screenPos());
        event->accept();
  }

  void MolScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
  {
    event->ignore();
    QGraphicsScene::mousePressEvent(event);
    if (event->isAccepted()) return;
    if (event->button() != Qt::LeftButton || event->modifiers()) return;
    d->selectionRectangle->setRect(QRectF(event->scenePos(), event->scenePos()));
    addItem(d->selectionRectangle);
    d->selectionRectangle->show();
    event->accept();
  }

  void MolScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
  {
    event->ignore();
    QGraphicsScene::mouseMoveEvent(event);
    if (event->isAccepted()) return;
    if (!d->selectionRectangle->scene()) return;
    if (!(event->buttons() & Qt::LeftButton)) return;
    d->selectionRectangle->setRect(QRectF(event->buttonDownScenePos(Qt::LeftButton), event->scenePos()));
    QPainterPath selectArea;
    selectArea.addRect(d->selectionRectangle->rect());
    setSelectionArea(selectArea);
    event->accept();
  }

  void MolScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
  {
    event->ignore();
    QGraphicsScene::mouseReleaseEvent(event);
    if(event->isAccepted()) return;
    if (event->button() != Qt::LeftButton) return;
    removeItem(d->selectionRectangle);
    event->accept();
  }

  void MolScene::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
  {
    if (!event->mimeData() || !event->mimeData()->hasFormat(mimeType())) return;
    if (!event->proposedAction() == Qt::CopyAction) return;
    event->accept();
    for (genericAction* action : sceneActions())
      action->setChecked(false);
    d->dragItem = new Molecule;
    QXmlStreamReader reader(event->mimeData()->data(mimeType()));
    reader >> *(d->dragItem);
    d->moveDragItem(event);
    addItem(d->dragItem); // TODO loop until stream is exhausted
    updateAll();
  }

  void MolScene::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
  {
    if (!d->dragItem) return;
    removeItem(d->dragItem);
    delete d->dragItem;
    d->dragItem = 0;
    event->accept();
  }

  void MolScene::dropEvent(QGraphicsSceneDragDropEvent *event)
  {
    if (!d->dragItem) return;
    event->accept();
    ItemAction::addItemToScene(d->dragItem, this, tr("insert molecule"));
  }

  void MolScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
  {
    qDebug() << "drag moving:" << event->proposedAction();
    if (!d->dragItem) return;
    d->moveDragItem(event);
    event->accept();
  }

  void MolScene::wheelEvent(QGraphicsSceneWheelEvent *event)
  {
    foreach(QGraphicsView* vp, views()) // TODO track back event to originator
    {
      MolView *mvp = qobject_cast<MolView*>(vp);
      if (!mvp) continue;
      mvp->scaleView(pow((double)2, -event->delta() / MOUSEWHEELDIVIDER));
    }
  }

  QString MolScene::xmlName() const { return xmlClassName(); }

  QString MolScene::xmlClassName() { return "molscene" ; }


  QUndoStack * MolScene::stack() const {
    return d->stack;
  }

  QList<genericAction *> MolScene::sceneActions() const
  {
    QList<genericAction *> actions;
#if QT_VERSION < 0x050000
        QList<genericAction*> allActionChildren = findChildren<genericAction*>() ;
        std::copy_if(allActionChildren.begin(), allActionChildren.end(), std::back_inserter(actions),
                     [&](QAction* a) { return a->parent() == this; });
#else
        actions = findChildren<genericAction*>(QString(), Qt::FindDirectChildrenOnly);
#endif

        return actions;
  }

  template <class T>
  T *MolScene::itemNear(const QPointF &pos, qreal tolerance) { // TODO unit test
    qreal minDistance = tolerance;
    T *result = nullptr;
    for(QGraphicsItem* item : items(QRectF(pos.x()-tolerance, pos.y()-tolerance, tolerance, tolerance))) {
      if (!(QLineF(item->scenePos(), pos).length() < minDistance)) continue;
      if (T *itemPointer = dynamic_cast<T*>(item)) result = itemPointer;
    }
    return result;
  }

} // namespace
