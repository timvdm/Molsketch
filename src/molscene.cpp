/***************************************************************************
 *   Copyright (C) 2007-2008 by Harm van Eersel                            *
 *   devsciurus@xs4all.nl                                                  *
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

#include <cmath>
#include <math.h>
#include <iostream>

#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QClipboard>
#include <QApplication>
#include <QListWidgetItem>
#include <QTableWidgetItem>
#include <QKeyEvent>
#include <QUndoStack>
#include <QDebug>

#include "molscene.h"

#include "atom.h"
#include "bond.h"
#include "molecule.h"
#include "mollibitem.h"
#include "commands.h"

namespace Molsketch {

using namespace Commands;


// Constructor & destructor

MolScene::MolScene(QObject* parent) : QGraphicsScene(parent)
{
  //Initializing properties
  m_currentElementSymbol = "C";
  m_editMode = MolScene::AddMode;
  m_bondLength = 40;
  m_bondOrder = 1;
  m_bondWidth = 0;
  m_bondType = 0;
  m_atomSize = 5;
  m_bondAngle = 30;
  m_carbonVisible = false;
  m_hydrogenVisible = true;
  m_chargeVisible = true;
  m_autoAddHydrogen = true;

  // Prepare undo m_stack
  m_stack = new QUndoStack(this);
  connect(m_stack, SIGNAL(indexChanged(int)), this, SIGNAL(documentChange()));
  connect(m_stack, SIGNAL(indexChanged(int)), this, SIGNAL(selectionChange()));
//   connect(m_stack, SIGNAL(indexChanged(int)), this, SIGNAL());
  connect(m_stack, SIGNAL(indexChanged(int)), this, SLOT(update()));

  // Prepare hinting items
  initHintItems();

  // Set initial size
  QRectF sizerect(-5000,-5000,10000,10000);
  setSceneRect(sizerect);
}


  MolScene::~MolScene()
  {
    // Clear the scene
    clear();

    // Destroy the hint items
    foreach(QGraphicsItem* item, m_hintPoints) 
      delete item;
    delete m_hintLine;
    //delete m_hoverRect;
  }

// Commands

void MolScene::setCarbonVisible( bool value )
{
  m_carbonVisible = value;
}

void MolScene::setHydrogenVisible( bool value )
{
  m_hydrogenVisible = value;
}

void MolScene::setBondLength( double length )
{
  //pre: bondlength > 0
  Q_ASSERT(length > 0);
//   if (m_bondLength <= 0) return;
  
  // Set the bondlength
  m_bondLength = length;

  // Reinitialize hintitems
  foreach (QGraphicsItem* item, m_hintPoints) delete item;
  m_hintPoints.clear();
  initHintItems();
}

void MolScene::setMSKAtomSize( qreal size )
{
  m_atomSize = size;
}

void MolScene::alignToGrid()
{
  m_stack->beginMacro(tr("aligning to grid"));
  foreach(QGraphicsItem* item,items()) 
  if (item->type() == Molecule::Type) 
    m_stack->push(new MoveItem(item,toGrid(item->scenePos()) - item->scenePos()));
  m_stack->endMacro();
  update();
}

  void MolScene::setEditMode(int mode)
  {
    // Reset moveflag (movebug)
    foreach(QGraphicsItem* item, items()) 
      item->setFlag(QGraphicsItem::ItemIsMovable, false);

    // enable moving for all Molecule and atom items
    foreach(QGraphicsItem* item, items())
      if (item->type() == Molecule::Type || item->type() == MSKAtom::Type) 
        item->setFlag(QGraphicsItem::ItemIsSelectable,mode == MolScene::MoveMode);

    // Set the new edit mode and signal other components
    m_editMode = mode;
    emit editModeChange( mode );
  }

void MolScene::cut()
{
  /* TODO Using the desktop clipboard*/
  // Check if something is selected
  if (selectedItems().isEmpty()) return;

  // Then do a copy
  copy();

  // Finally delete the selected items
  m_stack->beginMacro(tr("cutting items"));
  foreach (QGraphicsItem* item, selectedItems())
    if (item->type() == Molecule::Type) m_stack->push(new DelItem(item));
  m_stack->endMacro();
}

void MolScene::copy()
{
  // Check if something is selected
  if (selectedItems().isEmpty()) return;

  /* TODO Using the desktop clipboard */
  // Access the clipboard
  QClipboard* clipboard = qApp->clipboard();

  // Calculate total boundingrect
  QRectF totalRect;
  foreach(QGraphicsItem* item, selectedItems())
  {
    QRectF itemRect = item->boundingRect();
    itemRect.translate(item->scenePos());
    totalRect |= itemRect;
  }
  // Add to internal clipboard
  foreach(QGraphicsItem* item, m_clipItems) delete item;
  m_clipItems.clear();
  foreach(QGraphicsItem* item, selectedItems())
  if (item->type() == Molecule::Type)
    m_clipItems.append(new Molecule(dynamic_cast<Molecule*>(item)));

  // Clear selection
  QList<QGraphicsItem*> selList(selectedItems());
  clearSelection();

  // Choose the datatype
  //   clipboard->setText("Test");
  clipboard->setImage(renderImage(totalRect));
  //   clipboard->mimeData( );

  // Restore selection
  foreach(QGraphicsItem* item, selList) item->setSelected(true);
  
  // Emit paste available signal
  emit pasteAvailable(!m_clipItems.isEmpty());
}

void MolScene::paste()
{
  // Access the clipboard
//   QClipboard* clipboard = qApp->clipboard();
  /* TODO Using the system clipboard*/

  // Paste all items on the internal clipboard
  m_stack->beginMacro(tr("pasting items"));
  foreach(Molecule* item, m_clipItems) m_stack->push(new AddItem(new Molecule(item),this));
  m_stack->endMacro();
}


void MolScene::clear()
{
  // Purge the undom_stack
  m_stack->clear();
  
  // Removing all objects from the scene
  while (!items().isEmpty()) delete items()[0];
//   foreach (QGraphicsItem* item,items()) /// Why doesn't this work?
//   {
//     removeItem(item);
//     delete item;
//   }

  // Reinitialize the scene
  m_hintPoints.clear();
  initHintItems();
  setEditMode(MolScene::AddMode);
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

void MolScene::setElement( QListWidgetItem * item )
{
  if (item->text() != "  ")
  m_currentElementSymbol = item->text();
  setEditMode(MolScene::AddMode);
}

void MolScene::setElement( QTableWidgetItem * item )
{
  m_currentElementSymbol = item->text();
  setEditMode(MolScene::AddMode);
}

Molecule * MolScene::merge( const Molecule * molA, const Molecule* molB )
{
  // pre: molA and molB are different molecules
  Q_CHECK_PTR(molA);
  Q_CHECK_PTR(molB);
  Q_ASSERT(molA != molB);

  // Creating a new molecule for the merge
  Molecule* molC = new Molecule;

  // Adding the bonds and atoms of the first two molecules
  foreach (MSKAtom* a, molA->atoms())
    {
      MSKAtom* a2 = new MSKAtom(a->scenePos(),a->element(),a->hasImplicitHydrogens());
      molC->addMSKAtom(a2);
    }
  foreach (MSKBond* b, molA->bonds())
    {
      molC->addBond( molC->atomAt(b->beginAtom()->scenePos()), molC->atomAt(b->endAtom()->scenePos()), b->bondOrder(), b->bondType());
    }
  foreach (MSKAtom* a, molB->atoms())
    {
      molC->addMSKAtom( a->element(), a->scenePos(), a->hasImplicitHydrogens());
    }
  foreach (MSKBond* b, molB->bonds())
    {
      molC->addBond( molC->atomAt(b->beginAtom()->scenePos()), molC->atomAt(b->endAtom()->scenePos()), b->bondOrder(), b->bondType());
    }

//         molC->setPos(molA->scenePos());

  return molC;
}

void MolScene::addMolecule(QListWidgetItem* mol )
{
  // Extract the molecule and add it to the sceneMolecule* m
  Molecule * m;
  if (dynamic_cast<MolLibItem*>(mol))
    m = dynamic_cast<MolLibItem*>(mol)->getMolecule();
  else
    return;

  addMolecule(m);
}

void MolScene::addMolecule(Molecule* mol)
{
  Q_CHECK_PTR(mol);
  if (!mol) return;
  m_stack->beginMacro(tr("add molecule"));
  m_stack->push(new AddItem(mol,this)); 
  if (mol->canSplit()) m_stack->push(new SplitMol(mol));
  m_stack->endMacro();
}

void MolScene::setBondAngle(int angle)
{
  Q_ASSERT (angle > 0 and angle <= 360);

  // Set the new hintPointCount and reinitialize
  m_bondAngle = angle;

  // Reinitialize hintitems
  foreach (QGraphicsItem* item, m_hintPoints) delete item;
  m_hintPoints.clear();
  initHintItems();
}

void MolScene::selectAll()
{
  // Switch to move mode to make selection posible
  setEditMode(MolScene::MoveMode);
  
  // Clear any previous selection
  clearSelection();
  
  // Mark all molecules as selected
  foreach (QGraphicsItem* item, items())
  {
    if (item->type() == Molecule::Type)
      item->setSelected(true);
  }
}

// Hinting methods

void MolScene::drawHintPoints(const QPointF &startPos )
{
  QPointF position = startPos;

  // If clicked on atom, use that as startpoint
  MSKAtom* atom = atomAt(position);
  if (atom) position = atom->scenePos();

  // Draw hintpoints
  m_hintPointsGroup->setPos(position);
  addItem(m_hintPointsGroup);
}

void MolScene::initHintItems()
{
  // hint molecule always starts as none
  m_hintMolecule = 0;

  // Initialize hintline
  m_hintLine = new QGraphicsLineItem(QLineF(0,0,0,0));
  m_hintLine->setAcceptedMouseButtons(0);
  m_hintLine->setZValue(-1);

//   // Initialize hover feedback
//   m_hoverRect = new QGraphicsPathItem(QPainterPath());
//   m_hoverRect->setZValue(5);
//   m_hoverRect->setAcceptedMouseButtons(0);

  // Initialize hintpoints
  m_hintPointsGroup = new QGraphicsItemGroup();
  int parts = 360/m_bondAngle;
  for (int i = 0; i < parts; i++)
    {
      // Calculate dot position
      double x = 2*3.14*i/parts;
      QPointF hintPoint = QPointF(sin(x) * m_bondLength, cos(x) * m_bondLength);
      // Create the dot
      QGraphicsEllipseItem* dot = new QGraphicsEllipseItem(-2.5,-2.5,5,5);
      dot->setBrush(Qt::lightGray);
      dot->setPen(Qt::NoPen);
      // Add the dot
      m_hintPoints.append(dot);
      m_hintPoints[i]->setPos(hintPoint);
      m_hintPointsGroup->addToGroup(m_hintPoints[i]);
      //     scene()->addRect(QRectF(hintPoint.x()-2.5,hintPoint.y()-2.5,5,5));
    }
//   m_hintPointsGroup->setVisible(false);
  m_hintPointsGroup->setAcceptedMouseButtons(0);
}

void MolScene::setHintLine( const QPointF &startPos, const QPointF &curPos )
{
  // Set the hintline between startPos and curPos
  m_hintLine->setLine(QLineF(startPos,curPos));
  
  // If not already added, add the hintline to the scene
  if (!items().contains(m_hintLine)) 
    addItem(m_hintLine);
}

  void MolScene::setHintMolecule(MolLibItem *item)
  {
    Q_CHECK_PTR(item);
    
    Molecule *mol = item->getMolecule();

    // delete previous hint molecule
    if (m_hintMolecule)
      delete m_hintMolecule;

    m_hintMolecule = new QGraphicsItemGroup();
      
    foreach (MSKBond *bond, mol->bonds()) {
      // Create the line
      QPointF a = bond->beginAtom()->pos();
      QPointF b = bond->endAtom()->pos();
      QGraphicsLineItem *line = new QGraphicsLineItem(a.x(), a.y(), b.x(), b.y());
      line->setPen(QPen(Qt::lightGray));
      // Add the line
      m_hintMolecule->addToGroup(line);
    }
    
    addItem(m_hintMolecule);
  }
  
  void MolScene::setHintMolecule(QListWidgetItem *item)
  {
    Q_CHECK_PTR(item);
    
    MolLibItem *molLibItem = dynamic_cast<MolLibItem*>(item);
    if (molLibItem)
      setHintMolecule(molLibItem);
  }

  void MolScene::setHoverRect( QGraphicsItem* item )
  {
    if (item) {
      m_hoverRect->setPath(item->shape());
      m_hoverRect->setPos(item->scenePos());
      //       m_hoverRect->setVisible(true);
      addItem(m_hoverRect);
    } else {
      //     m_hoverRect->setVisible(false);
      removeItem(m_hoverRect);
    }
  }

void MolScene::removeHintLine( )
{
  //   delete hintLine;
  //   scene()->removeItem(hintLine);
//   m_hintLine->setVisible(false);
  if (items().contains(m_hintLine)) removeItem(m_hintLine);
  //   scene()->update();
}

void MolScene::removeHintPoints( )
{
//   m_hintPointsGroup->setVisible(false);
  if (items().contains(m_hintPointsGroup)) removeItem(m_hintPointsGroup);
//     for (int i = 0; i<m_bondAngle;i++)
//     {
//         //     scene()->removeItem(hintPoints[i]);
//         hintPoints[i]->setVisible(false);
//         //     delete hintPoints[i];
//     }
}

// Queries

  bool MolScene::carbonVisible( ) const
  {
    return m_carbonVisible;
  }

  bool MolScene::hydrogenVisible( ) const
  {
    return m_hydrogenVisible;
  }

  qreal MolScene::bondLength( ) const
  {
    return m_bondLength;
  }

  qreal MolScene::atomSize( ) const
  {
    return m_atomSize;
  }
  
  int MolScene::editMode() const
  {
    return m_editMode;
  }


// Advanced queries
QPointF MolScene::nearestPoint(const QPointF &curPos)
{
  //pre: true
  //ret: nearest preferred point of curPos

  // Set the maximun length
  qreal lastLength = m_bondLength / 4;
  QPointF nPoint = curPos;

  // Check the hinting points
  for (int i = 0; i < m_hintPoints.size(); i++)
    {
      QPointF hPoint = m_hintPoints[i]->scenePos();
      QLineF iLine(curPos,hPoint);
      if (iLine.length() < lastLength)
        {
          nPoint = hPoint;
          lastLength = iLine.length();
        }
    }

  // Look whether a atom is nearby
  MSKAtom* atom = atomAt(curPos);
  if (atom) nPoint = atom->scenePos();

  return nPoint;
}

QPointF MolScene::toGrid(const QPointF &position)
{
  QPointF p = position;
//   p.rx() = floor(p.x() / 100) * 100;
//   p.ry() = floor(p.y() / 100) * 100;
//   m_bondLength;
  int factor = 40;
  p.rx() = floor(p.x() / factor) * factor;
  p.ry() = floor(p.y() / factor) * factor;
//   p.rx() = (p.x() / 10) * 10;
//   p.ry() = (p.y() / 10) * 10;

  return p;
}


Molecule* MolScene::moleculeAt(const QPointF &pos)
{
  // Check if there is a molecule at this position
  foreach(QGraphicsItem* item,items(pos))
  if (item->type() == Molecule::Type) return dynamic_cast<Molecule*>(item);

  // Else return NULL
  return 0;

}

MSKAtom* MolScene::atomAt(const QPointF &pos)
{
  // Check if there is a atom at this position
  foreach(QGraphicsItem* item,items(pos))
  if (item->type() == MSKAtom::Type) return dynamic_cast<MSKAtom*>(item);
//   // Maybe the atom is hidden, try an alternative detection
//   for (int i = 0; i < items(pos).size(); i++)
//   {
//     QGraphicsItem* item = items(pos).at(i);
//     MSKAtom* atom;
//     if (item->type() == Molecule::Type) atom = dynamic_cast<Molecule*>(item)->getMSKAtom(pos);
//     if (atom) return atom;
//   }

  // Can't find an atom at that location
  return 0;
}

MSKBond* MolScene::bondAt(const QPointF &pos)
{
  // Check if there is a bond at this position
  foreach( QGraphicsItem* item,items(pos))
  if (item->type() == MSKBond::Type) return dynamic_cast<MSKBond*>(item);

  // Else return NULL
  return 0;
}

// Event handlers

bool MolScene::event(QEvent* event)
{
  // Execute default behaivior
  bool accepted = QGraphicsScene::event(event);
  
  // Check whether copying is available
  if ((event->type() == QEvent::GraphicsSceneMouseRelease) || (event->type() == QEvent::KeyRelease))
  {
    emit copyAvailable(!selectedItems().isEmpty());
//     emit pasteAvailable(!m_clipItems.isEmpty());
    emit selectionChange( );
  }
  
  // Execute default behavior
  return accepted;
}

void MolScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
  //   // Execute default behavior
  //   QGraphicsScene::mousePressEvent(event);

  // Execute extended behavior depending on edit mode and mouse button
  switch (event->button())
    {
    case Qt::RightButton:
      delModePress( event );
      break;
    case Qt::MidButton:
      moveModePress( event );
      break;
    case Qt::LeftButton:
      switch (m_editMode)
        {
        case MolScene::AddMode:
          addModePress( event );
          break;
        case MolScene::RemoveMode:
          delModePress( event );
          break;
        case MolScene::MoveMode:
          moveModePress( event );
          break;
        case MolScene::RotateMode:
          rotateModePress( event );
          break;
        default:
          break;
        };
      default:
          break;
    }

  // Execute default behavior
  QGraphicsScene::mousePressEvent(event);
}

void MolScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
  // Determin the right action
  if (m_hintMolecule) {
    m_hintMolecule->setPos(event->scenePos());
  } 
    
  switch (event->buttons())
    {
//     case Qt::MidButton:
//       moveModeMove(event);
//       break;
    default:
      switch (m_editMode)
        {
        case MolScene::AddMode:
          addModeMove(event);
          break;
//         case MolScene::MoveMode:
//           moveModeMove(event);
//           break;
        case MolScene::RotateMode:
          rotateModeMove(event);
          break;
        }
    }

  // Execute default behavior
  QGraphicsScene::mouseMoveEvent(event);
}


void MolScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{

  // Determin the right action
  switch (event->button())
    {
    case Qt::RightButton:
      break;
    case Qt::MidButton:
      moveModeRelease(event);
      break;
    default:
      switch (m_editMode)
        {
        case MolScene::AddMode:
          addModeRelease(event);
          clearSelection();
          break;
        case MolScene::MoveMode:
          moveModeRelease(event);
          break;
        case MolScene::RotateMode:
          rotateModeRelease(event);
          break;
        }
    }

  // Execute the normal behavior
  QGraphicsScene::mouseReleaseEvent(event);
}


void MolScene::mouseDoubleClickEvent( QGraphicsSceneMouseEvent * event )
{

  // Determin the corresponding action
  switch (event->button())
    {
    case Qt::RightButton:
      delModeDoubleClick(event);
      break;
    default:
      switch (m_editMode)
        {
        case MolScene::AddMode:
          break;
        case MolScene::RemoveMode:
          delModeDoubleClick(event);
          break;
        case MolScene::MoveMode:
          break;
        }
    }

  // Execute default behavior
  QGraphicsScene::mouseDoubleClickEvent( event );
}

void MolScene::moveModePress(QGraphicsSceneMouseEvent* event)
{
  // Check whether to select an item
  QGraphicsItem * item = itemAt(event->scenePos());
  if (item && !item->isSelected()) {
    clearSelection();
    item->setSelected(true);
  }

  // Flag the selected items as moveable
  clearFocus();
  foreach(QGraphicsItem* item,selectedItems()) item->setFlag(QGraphicsItem::ItemIsMovable,true);
  emit copyAvailable(!selectedItems().isEmpty());
}

void MolScene::moveModeMove(QGraphicsSceneMouseEvent* event)
{
  event->accept();
}

void MolScene::moveModeRelease(QGraphicsSceneMouseEvent* event)
{

  QPointF moveVector = event->scenePos() - event->buttonDownScenePos(event->button());
  if(moveVector.isNull()) 
    {
      clearFocus();
      return;
    }

  m_stack->beginMacro(tr("moving item(s)"));

  foreach(QGraphicsItem* item,selectedItems())
  {
    item->moveBy(-moveVector.x(), -moveVector.y());
    if (!moveVector.isNull()) m_stack->push(new MoveItem(item,moveVector,tr("moving item(s)")));
    item->setFlag(QGraphicsItem::ItemIsMovable,false);
  }
  m_stack->endMacro();
  clearFocus();
}

void MolScene::rotateModePress(QGraphicsSceneMouseEvent* event)
{
  // Execute default behavior
  QGraphicsScene::mousePressEvent(event);
}

void MolScene::rotateModeMove(QGraphicsSceneMouseEvent* event)
{
  // Do nothing if no buttons are pressed
  if (!(event->buttons() & Qt::LeftButton)) return;

  // Find the item to rotate and it's rotation point
  QGraphicsItem * item = itemAt(event->buttonDownScenePos(Qt::LeftButton));
  if (!item) return;
  if (item->type() == MSKAtom::Type) item = item->parentItem();
  if (item->type() == MSKBond::Type) item = item->parentItem();
  QPointF rotatePoint = item->boundingRect().center();

  // Calculate the rotation angle
  QPointF vec1 = event->buttonDownScenePos(Qt::LeftButton) - item->mapToScene(rotatePoint);
  QPointF vec2 = event->scenePos() - item->mapToScene(rotatePoint);
  qreal alpha = std::atan2(vec1.y(), vec1.x());
  qreal beta = std::atan2(vec2.y(), vec2.x());
  qreal rotateAngle = beta - alpha;
//   if (event->modifiers() != Qt::AltModifier) rotateAngle = toRoundedAngle(rotateAngle);
  if (rotateAngle == 0) return;

  // Temporary rotate the item
  QTransform transform;
  transform.translate(rotatePoint.x(), rotatePoint.y());
  switch (event->modifiers()) {
    case Qt::ControlModifier: transform.rotate(rotateAngle, Qt::XAxis); break;
    case Qt::ShiftModifier: transform.rotate(rotateAngle, Qt::YAxis); break;
    default: transform.rotate(rotateAngle, Qt::ZAxis);
  };
  transform.translate(-rotatePoint.x(), -rotatePoint.y());
  item->setTransform(transform, true);
//   item->rotate(rotateAngle);
}

void MolScene::rotateModeRelease(QGraphicsSceneMouseEvent* event)
{
  // Find the item to rotate find the rotate point
  QGraphicsItem * item = itemAt(event->buttonDownScenePos(Qt::LeftButton));
  if (!item) return;
  if (item->type() == MSKAtom::Type) item = item->parentItem();
  if (item->type() == MSKBond::Type) item = item->parentItem();
  QPointF rotatePoint = item->boundingRect().center();

  // Calculate the rotation angle
  QPointF vec1 = event->buttonDownScenePos(Qt::LeftButton) - item->mapToScene(rotatePoint);
  QPointF vec2 = event->scenePos() - item->mapToScene(rotatePoint);
  qreal alpha = std::atan2(vec1.y(), vec1.x());
  qreal beta = std::atan2(vec2.y(), vec2.x());
  qreal rotateAngle = beta - alpha;
//   if (event->modifiers() != Qt::AltModifier) rotateAngle = toRoundedAngle(rotateAngle);
  if (rotateAngle == 0) return;

  // Rotate the item
  QTransform transform;
  transform.translate(rotatePoint.x(), rotatePoint.y());
  switch (event->modifiers()) {
    case Qt::ControlModifier: transform.rotate(rotateAngle, Qt::XAxis); break;
    case Qt::ShiftModifier: transform.rotate(rotateAngle, Qt::YAxis); break;
    default: transform.rotate(rotateAngle, Qt::ZAxis);
  };
  transform.translate(-rotatePoint.x(), -rotatePoint.y());
  item->setTransform(transform.inverted(), true);
//   item->rotate(-rotateAngle/(2*M_PI)*360);

  m_stack->beginMacro(tr("rotating item(s)"));
  m_stack->push(new RotateItem(item, transform, tr("rotating item(s)")));
  m_stack->endMacro();
}

void MolScene::addModePress(QGraphicsSceneMouseEvent* event)
{
  // Get the position
  QPointF downPos = event->buttonDownScenePos(event->button());

  // Check for bond click
  if (bondAt(downPos) && !atomAt(downPos))
    {
//       if (bondAt(downPos)->bondOrder() != m_bondOrder or bondAt(downPos)->bondType() != m_bondType) {
//         m_stack->beginMacro("Change bond style");
//         MSKBond * bond = bondAt(downPos);
//         while (bond->bondOrder() != m_bondOrder)
//           m_stack->push(new IncOrder(bond));
//         while (bond->bondType() != m_bondType)
//           m_stack->push(new IncType(bond));
//         m_stack->endMacro();
//         return;
//       } else {
        if (event->modifiers() == Qt::SHIFT)
          m_stack->push(new IncType(bondAt(downPos),tr("change of bondtype")));
        else
          m_stack->push(new IncOrder(bondAt(downPos),tr("change of bondorder")));
        return;
//       }
    }
  
  // Show hinting
  if (m_currentElementSymbol != "+" && m_currentElementSymbol != "-" && m_currentElementSymbol != "H+" && m_currentElementSymbol != "H-")
  {
    drawHintPoints(downPos);
    setHintLine(downPos,event->scenePos());
    m_hintLine->setVisible(true);
  }

  // Check which molecule has been clicked on
  MSKAtom* atom = atomAt(downPos);

  // Add a new molecule if necesarry
  if (!atom && m_currentElementSymbol!="+" && m_currentElementSymbol!="-" && m_currentElementSymbol != "H+" && m_currentElementSymbol != "H-")
    {
      Molecule* mol = new Molecule;
      mol->setPos(downPos);
      m_stack->beginMacro("Add Molecule");
      m_stack->push(new AddItem(mol,this));
      m_stack->push(new AddAtom(new MSKAtom( downPos, m_currentElementSymbol, m_autoAddHydrogen ), mol));
      m_stack->endMacro();
    }
}

void MolScene::addModeMove(QGraphicsSceneMouseEvent* event)
{
  //   QPointF startPos = event->buttonDownScenePos(Qt::LeftButton);
  //   QPointF curPos = event->scenePos();
  //   MSKAtom* a1 = getMSKAtom(startPos);
  //   MSKAtom* a2 = getMSKAtom(curPos);
  //   MSKBond* b = getBond(startPos);

  // Check if hovering over an atom
//   MSKAtom* atom = atomAt(event->scenePos());

  // Check hinting conditions
  if (!(event->buttons() & Qt::LeftButton)) return;
//   if (nearestPoint(event->buttonDownScenePos(Qt::LeftButton))==QPointF(0,0)) return;
  if (m_currentElementSymbol == "+" || m_currentElementSymbol == "-" || m_currentElementSymbol == "H+" || m_currentElementSymbol == "H-") return;
  if (!atomAt(event->buttonDownScenePos(Qt::LeftButton))) return;

  // Set hinting
  setHintLine(nearestPoint(event->buttonDownScenePos(Qt::LeftButton)), nearestPoint(event->scenePos()));
//   setHoverRect(atom);
}

void MolScene::addModeRelease(QGraphicsSceneMouseEvent* event)
{

  // Remove the hinting
  removeHintLine();
  removeHintPoints();

  // Get position
  QPointF downPos = event->buttonDownScenePos(event->button());
  QPointF upPos = nearestPoint(event->scenePos());

  // Check possible targets
  MSKAtom* a1 = atomAt(downPos);
  MSKAtom* a2 = atomAt(upPos);
  MSKBond* b = bondAt(downPos);
  Molecule* m1 = a1 ? a1->molecule() : 0;
   
  if (!a1 && !b) return;
  if (b && !a1) return;
  

  if (m_currentElementSymbol=="+")
    {
      m_stack->push(new IncCharge(a1, tr("increasing charge")));
      return;
    }
  if (m_currentElementSymbol=="-")
    {
      m_stack->push(new DecCharge(a1,tr("decreasing charge")));
      return;
    }
  if (m_currentElementSymbol=="H+")
    {
      m_stack->push(new AddImplicitHydrogen(a1, tr("add implicit hydrogen")));
      return;
    }
  if (m_currentElementSymbol=="H-")
    {
      if (a1->numberOfImplicitHydrogens() > 0) m_stack->push(new RemoveImplicitHydrogen(a1,tr("remove implicit hydrogen")));
      return;
    }

  // Check for atom release
  if (a2)
    {
      Molecule* m2 = a2->molecule();

      // Check for atom click
      if (a1==a2)
        {
          if (a1->element()!=m_currentElementSymbol)
            m_stack->push(new ChangeElement(a1,m_currentElementSymbol,tr("changing element")));
// 		else
// 			m_stack->push(new AddAtom(new MSKAtom(QPointF(0,0),m_currentElementSymbol),a1->molecule()));
          return;
        }

      // Begin adding macro
      m_stack->beginMacro("adding bond");

      // Check for merge
      if (m1 != m2)
        {
          m_stack->push(new MergeMol(m1,m2,m1));
          a1 = m1->atomAt(downPos);
          a2 = m1->atomAt(upPos);
          m1->setFocus();
        }

      // Adding bond
      MSKBond* bond = new MSKBond(a1,a2);
      m_stack->push(new AddBond(bond));
      for (int i = 0; i < m_bondOrder - 1; i++) 
        m_stack->push(new IncOrder(bond));
      for (int i = 0; i < m_bondType; i++) 
        m_stack->push(new IncType(bond));

      // End adding macro
      m_stack->endMacro();
      return;
    };
  
  // Else scene release
  m_stack->beginMacro(tr("adding atom"));
  MSKAtom* atom = new MSKAtom(upPos,m_currentElementSymbol,m_autoAddHydrogen);
  m_stack->push(new AddAtom(atom,m1));
  MSKBond* bond = new MSKBond(a1,atom);
  m_stack->push(new AddBond(bond));
  for (int i = 0; i < m_bondOrder - 1; i++) 
    m_stack->push(new IncOrder(bond));
  for (int i = 0; i < m_bondType; i++) 
    m_stack->push(new IncType(bond));
  m_stack->endMacro();

  update();
}

void MolScene::addModeDoubleRelease( QGraphicsSceneMouseEvent * event )
{
  /* TODO Find some nice use for double clicks */
  event->accept();
}

void MolScene::delModePress(QGraphicsSceneMouseEvent* event)
{
  MSKAtom* a = atomAt(event->scenePos());
  MSKBond* b = bondAt(event->scenePos());
  Molecule* mol;

  // Look for atomclick
  if (a)
    {
      mol = a->molecule();
      m_stack->beginMacro(tr("removing atom"));
      m_stack->push(new DelAtom(a));
      if (mol->canSplit()) m_stack->push(new SplitMol(mol));
      if (mol->atoms().isEmpty()) m_stack->push(new DelItem(mol));
      m_stack->endMacro();
      return;
    }

  // Look for bondclick
  if (b)
    {
      mol = b->molecule();
      m_stack->beginMacro(tr("removing bond"));
      m_stack->push(new DelBond(b));
      if (mol->canSplit()) m_stack->push(new SplitMol(mol));
      m_stack->endMacro();
      return;
    }
}

void MolScene::delModeDoubleClick( QGraphicsSceneMouseEvent * event )
{
  Molecule* item = moleculeAt(event->scenePos());
  if (item) m_stack->push(new DelItem(item,tr("removing molecule")));
}

void MolScene::keyPressEvent(QKeyEvent* keyEvent)
{
  // Declare item
  QGraphicsItem* item;
  MSKAtom* atom;
//   MSKBond* bond;
//   Molecule* mol;
  QSet<Molecule*> molSet;
  
  switch (keyEvent->key())
    {
    case Qt::Key_Delete:
      m_stack->beginMacro(tr("removing item(s)"));
      // First delete all selected molecules
      foreach (item, selectedItems())
        if (item->type() == Molecule::Type)
          {
            m_stack->push(new DelItem(item));
          }
//       // Then delete 
//       foreach (item, selectedItems())
//         if (item->type() == MSKBond::Type)
//         {
//           bond = dynamic_cast<MSKBond*>(item);
//           mol = bond->molecule();
//           m_stack->push(new DelBond(bond));
//           if (mol->canSplit()) m_stack->push(new SplitMol(mol));
//         };

      // Then delete all selected atoms
      foreach (item, selectedItems())
      if (item->type() == MSKAtom::Type)
        {
          atom = dynamic_cast<MSKAtom*>(item);
          molSet << atom->molecule();
          m_stack->push(new DelAtom(atom));
        }
     
      // Cleanup the affected molecules
      foreach (Molecule* mol, molSet)
      {
        if (mol->canSplit()) m_stack->push(new SplitMol(mol));
        if (mol->atoms().isEmpty()) m_stack->push(new DelItem(mol));
      }
      
      // Finally delete all the residues
      foreach (item, selectedItems()) m_stack->push(new DelItem(item));
      
      m_stack->endMacro();
      keyEvent->accept();
      break;
    case Qt::Key_Up:
      m_stack->beginMacro("moving item(s)");
      foreach (item, selectedItems())
      m_stack->push(new MoveItem(item,QPointF(0,-10)));
      m_stack->endMacro();
      keyEvent->accept();
      break;
    case Qt::Key_Down:
      m_stack->beginMacro("moving item(s)");
      foreach (item, selectedItems())
      m_stack->push(new MoveItem(item,QPointF(0,10)));
      m_stack->endMacro();
      keyEvent->accept();
      break;
    case Qt::Key_Left:
      m_stack->beginMacro("moving item(s)");
      foreach (item, selectedItems())
      m_stack->push(new MoveItem(item,QPointF(-10,0)));
      m_stack->endMacro();
      keyEvent->accept();
      break;
    case Qt::Key_Right:
      m_stack->beginMacro("moving item(s)");
      foreach (item, selectedItems())
      m_stack->push(new MoveItem(item,QPointF(10,0)));
      m_stack->endMacro();
      keyEvent->accept();
      break;
    case Qt::Key_Escape:
      clearSelection();
      break;
    default:
      keyEvent->ignore();
    }
}

QUndoStack * MolScene::stack()
{
  return m_stack;
}

int MolScene::toRoundedAngle(int angle)
{
  return angle - angle % m_bondAngle;
}

void MolScene::setBondType(int type)
{
  Q_ASSERT( type >= 0 and type < 6);
  m_bondType = type;
}

void MolScene::setBondOrder(int order)
{
  Q_ASSERT( order > 0 and order < 4);
  m_bondOrder = order;
}

void MolScene::setElement(const QString & symbol)
{
  m_currentElementSymbol = symbol;
}

void MolScene::setIncChargeMode()
{
  m_currentElementSymbol = "+";
}

void MolScene::setDecChargeMode()
{
  m_currentElementSymbol = "-";
}

void MolScene::setIncHydrogenMode()
{
  m_currentElementSymbol = "H+";
}

void MolScene::setDecHydrogenMode()
{
  m_currentElementSymbol = "H-";
}

QFont MolScene::atomSymbolFont() const
{
  return m_atomSymbolFont;
}

void MolScene::setAtomSymbolFont(const QFont & font)
{
  m_atomSymbolFont = font;
}

void MolScene::setBondWidth(double width)
{
  Q_ASSERT(width > 0);
  m_bondWidth = width;
}

qreal MolScene::bondWidth() const
{
  return m_bondWidth;
}

} // namespace
