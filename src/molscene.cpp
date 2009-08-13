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
#include <QProcess>
#include <QDir>
#include <QDesktopServices>
#include <QDebug>

#include "molscene.h"

#include "element.h"
#include "atom.h"
#include "bond.h"
#include "molecule.h"
#include "mollibitem.h"
#include "commands.h"

#include "minimise.h"
#include "math2d.h"

//#include "filio.h"
#include "osra.h"
#include <openbabel/mol.h>
#include <openbabel/atom.h>
#include <openbabel/bond.h>
#include <openbabel/obiter.h>

using namespace OpenBabel;




namespace Molsketch {

  using namespace Commands;


  // Constructor & destructor

  MolScene::MolScene(QObject* parent) : QGraphicsScene(parent)
  {
	  color = QColor (0, 0, 0);
	  rotationItem = NULL;  
	  inputTextItem = new TextInputItem ();
	  addItem (inputTextItem);


	  inputTextItem ->hide ();

	  


	 
	  
	  lassoPolygon = addPolygon (QPolygon ());
	  lassoPolygon ->hide ();

    rotationItem = NULL;  

    lassoPolygon = addPolygon (QPolygon ());
    lassoPolygon ->hide ();


    //Initializing properties
    m_currentElementSymbol = "C";
    m_editMode = MolScene::DrawMode;
    m_bondLength = 40;
    m_bondOrder = 1;
    m_bondWidth = 0;
    m_bondType = Bond::InPlane;
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
	
	void MolScene::setColor (QColor c) {
		color = c;
		foreach (QGraphicsItem* item, selectedItems()) {
			if (item->type() == Atom::Type) {
				dynamic_cast<Atom*>(item) ->setColor(c);
			}

		}
		foreach (QGraphicsItem* item, items()) {
			if (item->type() == Bond::Type) {
				Bond *b = dynamic_cast<Bond*>(item);
				if (b-> beginAtom() ->isSelected () && b->endAtom() ->isSelected()) b->setColor(c);
			}
		}
			
	}

  void MolScene::setCarbonVisible(bool value)
  {
    m_carbonVisible = value;
  }

  void MolScene::setHydrogenVisible(bool value)
  {
    m_hydrogenVisible = value;
  }

  void MolScene::setBondLength(double length)
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

  void MolScene::setAtomSize( qreal size )
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
      if (item->type() == Molecule::Type || item->type() == Atom::Type) 
        item->setFlag(QGraphicsItem::ItemIsSelectable,mode == MolScene::MoveMode);

    // Delete hint Ring if mode != addMode
    if (mode != MolScene::DrawMode) {
      if (m_hintMoleculeItems) {
        delete m_hintMoleculeItems;
        m_hintMoleculeItems = 0;
      }
      m_hintRingPoints.clear();
    }

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

  void MolScene::convertImage()
  {
    QClipboard* clipboard = qApp->clipboard();
    QImage img = clipboard->image();

    if (!img.isNull()) {
      m_stack->beginMacro(tr("converting image using OSRA"));
      QString tmpimg = QDesktopServices::storageLocation(QDesktopServices::TempLocation) + QDir::separator() + "osra.png";
      img.save(tmpimg, "PNG", 100);
      Molecule* mol = call_osra(tmpimg);
      if (mol) 
        m_stack->push(new AddItem(new Molecule(mol), this));
      QFile::remove(tmpimg);
      m_stack->endMacro();
    }
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
    setEditMode(MolScene::DrawMode);
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

  Molecule * MolScene::merge( const Molecule * molA, const Molecule* molB )
  {
    // pre: molA and molB are different molecules
    Q_CHECK_PTR(molA);
    Q_CHECK_PTR(molB);
    Q_ASSERT(molA != molB);

    // Creating a new molecule for the merge
    Molecule* molC = new Molecule;

    // Adding the bonds and atoms of the first two molecules
    foreach (Atom* a, molA->atoms())
    {
      Atom* a2 = new Atom(a->scenePos(),a->element(),a->hasImplicitHydrogens());
		a2 ->setColor( a->getColor ());
      molC->addAtom(a2);
    }
    foreach (Bond* b, molA->bonds())
    {
      molC->addBond( molC->atomAt(b->beginAtom()->scenePos()), molC->atomAt(b->endAtom()->scenePos()), b->bondOrder(), b->bondType(), b->getColor ());
    }
    foreach (Atom* a, molB->atoms())
    {
      molC->addAtom( a->element(), a->scenePos(), a->hasImplicitHydrogens(), a->getColor ());
    }
    foreach (Bond* b, molB->bonds())
    {
      molC->addBond( molC->atomAt(b->beginAtom()->scenePos()), molC->atomAt(b->endAtom()->scenePos()), b->bondOrder(), b->bondType(), b->getColor ());
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

  void MolScene::insertRing(const QPointF &pos)
  {
    qDebug() << "insertRing @ " << pos;

    QList<Molecule*> molecules;
    // check if there are already atoms at the corners
    foreach (const QPointF &p, m_hintRingPoints) {
      Atom *atom = atomAt(m_hintMoleculeItems->mapToScene(p));
      if (atom) 
        if (atom->molecule() && !molecules.contains(atom->molecule()))
          molecules.append(atom->molecule());
    }
    m_stack->beginMacro("Add Molecule");
    // create a new molecule or merge extisting molecules
    if (molecules.isEmpty()) {
      // Adding a totally new ring
      molecules.append(new Molecule);
      m_stack->push(new AddItem(molecules[0],this));
    } else if (molecules.size() > 1) {
      // the ring merges several molecules (i.e. it does not only extend a single molecule)
      Molecule *mergedMol = molecules[0];
      for (int i = 1; i < molecules.size(); ++i)
        mergedMol = merge(mergedMol, molecules[i]);
      //m_stack->push(new MergeMol(molecules[0], molecules[i], molecules[0]));
      m_stack->push(new AddItem(mergedMol,this));
      molecules[0] = mergedMol;
    }
    // add the atoms and bonds
    QList<Bond*> bonds;
    int indexOfFirstDoubleBond = -1;
    for (int i = 0; i < m_hintRingPoints.size(); ++i) {
      int next = i + 1;
      if (next == m_hintRingPoints.size())
        next = 0;

      QPointF beginPoint = m_hintMoleculeItems->mapToScene(m_hintRingPoints[i]);
      QPointF endPoint = m_hintMoleculeItems->mapToScene(m_hintRingPoints[next]);
      Atom *begin = molecules[0]->atomAt(beginPoint);
      if (!begin) {
        begin = new Atom(beginPoint, "C", m_autoAddHydrogen);
        m_stack->push(new AddAtom(begin, molecules[0]));
      }
      Atom *end = molecules[0]->atomAt(endPoint);
      if (!end) {
        end = new Atom(endPoint, "C", m_autoAddHydrogen);
        m_stack->push(new AddAtom(end, molecules[0]));
      }

      Bond *bond = molecules[0]->bondBetween(begin, end);
      if (!bond) {
        bond = new Bond(begin, end);
        m_stack->push(new AddBond(bond));
      }
      bonds.append(bond);

      if (m_aromaticHintRing && (indexOfFirstDoubleBond < 0)) {

        bool beginHasDoubleBond = false;
        bool endHasDoubleBond = false;
        foreach (Atom *nbr, begin->neighbors()) {
          Bond *possibleDoubleBond = molecules[0]->bondBetween(begin, nbr);
          if (possibleDoubleBond && possibleDoubleBond->bondOrder() == 2)
            beginHasDoubleBond = true;
        }
        foreach (Atom *nbr, end->neighbors()) {
          Bond *possibleDoubleBond = molecules[0]->bondBetween(end, nbr);
          if (possibleDoubleBond && possibleDoubleBond->bondOrder() == 2)
            endHasDoubleBond = true;
        }

        if (beginHasDoubleBond && endHasDoubleBond)
          indexOfFirstDoubleBond = i;
      }
    }
    if (m_aromaticHintRing) {
      int numBonds;
      if (m_hintRingPoints.size() == 5) {
        if (indexOfFirstDoubleBond == -1) {
          indexOfFirstDoubleBond = 1;
          numBonds = 5;
        } else {
          indexOfFirstDoubleBond = indexOfFirstDoubleBond % 2 + 2;
          numBonds = 4;
        }
      } else {
        indexOfFirstDoubleBond = (indexOfFirstDoubleBond == -1) ? 0 : indexOfFirstDoubleBond % 2 + 2;
        numBonds = 6;
      }
      for (int i = indexOfFirstDoubleBond; i < numBonds; i+=2)
        if (bonds.at(i)->bondOrder() == 1)
	  m_stack->push(new IncOrder(bonds.at(i)));
    }

    molecules[0]->setFocus();
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

    // Mark all atoms as selected
    foreach (QGraphicsItem* item, items())
    {
      if (item->type() == Atom::Type)
        item->setSelected(true);
    }
  }

  // Hinting methods

  void MolScene::drawHintPoints(const QPointF &startPos )
  {
    QPointF position = startPos;

    // If clicked on atom, use that as startpoint
    Atom* atom = atomAt(position);
    if (atom) position = atom->scenePos();

    // Draw hintpoints
    m_hintPointsGroup->setPos(position);
    addItem(m_hintPointsGroup);
  }

  void MolScene::initHintItems()
  {
    // hint molecule always starts as none
    m_hintMolecule = 0;
    m_hintMoleculeItems = 0;

    // Initialize hintline
    m_hintLine = new QGraphicsLineItem(QLineF(0,0,0,0));
    m_hintLine->setAcceptedMouseButtons(0);
    m_hintLine->setZValue(-1);

    //   // Initialize hover feedback
    //   m_hoverRect = new QGraphicsPathItem(QPainterPath());
    //   m_hoverRect->setZValue(5);
    //   m_hoverRect->setAcceptedMouseButtons(0);

    // Initialize hint points circle
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

  void MolScene::setHintLine(const QPointF &startPos, const QPointF &curPos)
  {
    // Set the hintline between startPos and curPos
    m_hintLine->setLine(QLineF(startPos, curPos));

    // If not already added, add the hintline to the scene
    if (!items().contains(m_hintLine)) 
      addItem(m_hintLine);
  }

  // create a QGraphicsItem for regular polygon rings
  void MolScene::setHintRing(int ringSize, bool aromatic)
  {
    if (ringSize < 3)
      return;

    if (aromatic && ((ringSize == 5) || (ringSize == 6)))
      m_aromaticHintRing = true;
    else
      m_aromaticHintRing = false;

    // delete previous hint molecule
    if (m_hintMoleculeItems)
      delete m_hintMoleculeItems;

    m_hintMoleculeItems = new QGraphicsItemGroup();
    m_hintRingPoints.clear();

    // circumscribed circle regular polygon
    double radius = m_bondLength / (2 * sin(M_PI / ringSize));

    for (int i = 0; i < ringSize; ++i) {
      // Compute begin & end position
      double x1 = 2 * M_PI * i / ringSize;
      QPointF p1(sin(x1) * radius, cos(x1) * radius);
      double x2 = 2 * M_PI * (i + 1) / ringSize;
      QPointF p2(sin(x2) * radius, cos(x2) * radius);
      // Add the line
      QGraphicsLineItem *line = new QGraphicsLineItem(p1.x(), p1.y(), p2.x(), p2.y());
      line->setPen(QPen(Qt::lightGray));
      m_hintMoleculeItems->addToGroup(line);
      m_hintRingPoints.append(p1);
    }

    addItem(m_hintMoleculeItems);
  }

  void MolScene::setHintMolecule(MolLibItem *item)
  {
    Q_CHECK_PTR(item);

    // delete previous hint molecule
    if (m_hintMolecule)
      delete m_hintMolecule;
    if (m_hintMoleculeItems)
      delete m_hintMoleculeItems;

    m_hintMolecule = item->getMolecule();
    m_hintMoleculeItems = new QGraphicsItemGroup();

    foreach (Bond *bond, m_hintMolecule->bonds()) {
      // Create the line
      QPointF a = bond->beginAtom()->pos();
      QPointF b = bond->endAtom()->pos();
      QGraphicsLineItem *line = new QGraphicsLineItem(a.x(), a.y(), b.x(), b.y());
      line->setPen(QPen(Qt::lightGray));
      // Add the line
      m_hintMoleculeItems->addToGroup(line);
    }

    addItem(m_hintMoleculeItems);
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
    Atom* atom = atomAt(curPos);
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
	bool MolScene::textEditItemAt (const QPointF &pos) {
	    foreach(QGraphicsItem* item,items(pos))
		if (item->type() == TextInputItem::Type) return true;
		return false;
	}

  Atom* MolScene::atomAt(const QPointF &pos)
  {
    // Check if there is a atom at this position
    foreach(QGraphicsItem* item,items(pos))
      if (item->type() == Atom::Type) return dynamic_cast<Atom*>(item);
    //   // Maybe the atom is hidden, try an alternative detection
    //   for (int i = 0; i < items(pos).size(); i++)
    //   {
    //     QGraphicsItem* item = items(pos).at(i);
    //     Atom* atom;
    //     if (item->type() == Molecule::Type) atom = dynamic_cast<Molecule*>(item)->getAtom(pos);
    //     if (atom) return atom;
    //   }

    // Can't find an atom at that location
    return 0;
  }

  Bond* MolScene::bondAt(const QPointF &pos)
  {
    // Check if there is a bond at this position
    foreach( QGraphicsItem* item,items(pos))
      if (item->type() == Bond::Type) return dynamic_cast<Bond*>(item);

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

  void MolScene::alignRingWithAtom(Atom *atom)
  {
    Q_CHECK_PTR(m_hintMoleculeItems);

    if (m_hintRingPoints.isEmpty())
      return;

    if (atom->numBonds() == 0) {
      // just translate ring to make 1 atom align
      QPointF moleculeNormal = QPointF(0.0, -1.0);
      QPointF ringNormal = normalized(m_hintRingPoints[0]);
      QPointF rp = m_hintRingPoints[0];

      qreal ang = angle(moleculeNormal, ringNormal) * 180.0 / M_PI;
      if (angleSign(moleculeNormal, ringNormal) > 0.0)
        ang = -ang;

      m_hintMoleculeItems->setTransform(QTransform().rotate(ang+180.0).translate(-rp.x(), -rp.y()));
    } else if (atom->numBonds() == 1) {
      // rotate/translate ring to align with the bond
      QPointF moleculeNormal = atom->scenePos() - atom->neighbors()[0]->scenePos();
      moleculeNormal = normalized(moleculeNormal);
      QPointF ringNormal = normalized(m_hintRingPoints[0]);
      QPointF rp = m_hintRingPoints[0];

      qreal ang = angle(moleculeNormal, ringNormal) * 180.0 / M_PI;
      if (angleSign(moleculeNormal, ringNormal) > 0.0)
        ang = -ang;

      m_hintMoleculeItems->setTransform(QTransform().rotate(ang+180.0).translate(-rp.x(), -rp.y()));
      m_hintMoleculeItems->setPos(atom->scenePos());

    } else if (atom->numBonds() == 2) {
      // rotate/translate ring to align with the bond
      QPointF moleculeNormal = atom->scenePos() - atom->neighbors()[0]->scenePos();
      moleculeNormal += atom->scenePos() - atom->neighbors()[1]->scenePos();
      moleculeNormal = normalized(moleculeNormal);
      QPointF ringNormal = normalized(m_hintRingPoints[0]);
      QPointF rp = m_hintRingPoints[0];

      qreal ang = angle(moleculeNormal, ringNormal) * 180.0 / M_PI;
      if (angleSign(moleculeNormal, ringNormal) > 0.0)
        ang = -ang;

      m_hintMoleculeItems->setTransform(QTransform().rotate(ang+180.0).translate(-rp.x(), -rp.y()));
      m_hintMoleculeItems->setPos(atom->scenePos());
    }
  }

  void MolScene::alignRingWithBond(Bond *bond, const QPointF &pos)
  {
    Q_CHECK_PTR(m_hintMoleculeItems);
    Q_CHECK_PTR(bond->beginAtom());
    Q_CHECK_PTR(bond->endAtom());

    if (m_hintRingPoints.isEmpty())
      return;

    // just translate ring to make 1 atom align
    QPointF bondNormal = normalized(bond->endAtom()->pos() - bond->beginAtom()->pos());
    QPointF ringNormal = normalized( (m_hintRingPoints[0] + m_hintRingPoints[1]) / 2.0 );
    QPointF rp = m_hintRingPoints[0];

    qreal ang = angle(bondNormal, ringNormal) * 180.0 / M_PI;
    double angSign = angleSign(bondNormal, ringNormal);
    if (angSign > 0.0) 
      ang = -ang;

    double triSign = triangleSign(bond->beginAtom()->scenePos(), bond->endAtom()->scenePos(), pos);

    if (triSign > 0.0) {
      m_hintMoleculeItems->setTransform(QTransform().rotate(ang+270.0).translate(-rp.x(), -rp.y()));
      m_hintMoleculeItems->setPos(bond->endAtom()->scenePos());
    } else {
      m_hintMoleculeItems->setTransform(QTransform().rotate(ang+90.0).translate(-rp.x(), -rp.y()));
      m_hintMoleculeItems->setPos(bond->beginAtom()->scenePos());
    }
  }




  void MolScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
  {
	  
	  
	  // Execute default behavior
	  QGraphicsScene::mousePressEvent(event);
	  
	  
	  
	  
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
          case MolScene::DrawMode:
            addModePress( event );
            break;
          case MolScene::MoveMode:
            moveModePress( event );
            break;
          case MolScene::RotateMode:
            rotateModePress( event );
            break;
          case MolScene::LassoMode:
            lassoModePress (event);
            break;
	  case MolScene::TextMode:
	    textModePress (event);
			case MolScene::MinimiseMode:
				minimiseModePress (event);
	    break;
          default:
            break;
        };
      default:
        break;
    }

  }

  void MolScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
  {
    // Determine the right action
    if (m_hintMoleculeItems) {
      m_hintMoleculeItems->setPos(event->scenePos());
      m_hintMoleculeItems->setTransform(QTransform());

      // Get the position
      QPointF downPos = event->scenePos();

      Atom *atom = atomAt(downPos);
      if (atom)
        alignRingWithAtom(atom);

      Bond *bond = bondAt(downPos);
      if (bond)
        alignRingWithBond(bond, downPos);

    } 

    switch (event->buttons())
    {
      //     case Qt::MidButton:
      //       moveModeMove(event);
      //       break;
      default:
        switch (m_editMode)
        {
          case MolScene::DrawMode:
            addModeMove(event);
            break;
            //         case MolScene::MoveMode:
            //           moveModeMove(event);
            //           break;
          case MolScene::RotateMode:
            rotateModeMove(event);
            break;
          case MolScene::LassoMode:
            lassoModeMove(event);
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
        case MolScene::DrawMode:
            addModeRelease(event);
            clearSelection();
            break;
        case MolScene::MoveMode:
            moveModeRelease(event);
            break;
        case MolScene::LassoMode:
            lassoModeRelease(event);
            break;
        case MolScene::RotateMode:
          rotateModeRelease(event);
          break;
	case MolScene::TextMode:
	  textModeRelease (event);
		  case MolScene::MinimiseMode:
			  minimiseModeRelease (event);
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
        case MolScene::DrawMode:
				addModeDoubleClick (event);

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

	void MolScene::minimiseModePress (QGraphicsSceneMouseEvent* event) {
		Bond *bond = bondAt (event ->scenePos());
		if (bond) {
			Molecule *molecule = bond ->molecule();
			mirrorBondInMolecule(molecule, bond);
			return;
		}
		Molecule * molecule = moleculeAt(event->scenePos());
		if (molecule) minimiseMolecule (molecule);

	}


	void MolScene::textModePress(QGraphicsSceneMouseEvent* event) {
		if (textEditItemAt (event ->scenePos())) {
			inputTextItem ->setFocus();
		}
		else {
		Atom * atom = atomAt(event->scenePos());
		if (atom) {
			inputTextItem ->clickedOn (atom);

				}
/*		
		else {
			QGraphicsTextItem *text = addText("");
			text ->show ();
			text ->setFlag(QGraphicsItem::ItemIsSelectable);
			text->setTextInteractionFlags(Qt::TextEditorInteraction);
			text->setPos (event->buttonDownScenePos(event->button()));
			text ->setFocus ();

		}
*/
		
			}
	}
	
	void MolScene::textModeRelease(QGraphicsSceneMouseEvent* event) {
			
	}
	void MolScene::minimiseModeRelease(QGraphicsSceneMouseEvent* event) {
		
	}

	
	void MolScene::lassoModePress(QGraphicsSceneMouseEvent* event)
	{
		foreach(QGraphicsItem* item, items())
		if (item->type() == Molecule::Type || item->type() == Atom::Type) 
			item->setFlag(QGraphicsItem::ItemIsSelectable,true);
	
		
		
		lassoPolygon ->show ();
		lassoTrail.clear ();
		lassoTrail.push_back (event->scenePos());

	}
	
	void MolScene::lassoModeMove(QGraphicsSceneMouseEvent* event)
	{
		if (!(event->buttons() & Qt::LeftButton)) return;
		
		lassoTrail.push_back (event->scenePos());
		if (lassoTrail.size () > 4) {
			lassoPolygon ->setPolygon (QPolygonF (lassoTrail));
			lassoSelect ();

		}
		event->accept();
	}
	
	void MolScene::lassoModeRelease(QGraphicsSceneMouseEvent* event)
	{
		
		lassoTrail.clear ();
		lassoSelect ();
		lassoPolygon ->setPolygon(QPolygonF ());
		lassoPolygon ->hide ();

	
		
	}
	
	void MolScene::lassoSelect()
	{
		clearSelection();
		QList<QGraphicsItem *> its = items (lassoPolygon ->polygon (), Qt::ContainsItemShape);
		for (unsigned int i = 0; i < its.size (); i++) {
			its[i] ->setSelected (true);
		}

		
	}	
	
	
	
  void MolScene::rotateModeMove(QGraphicsSceneMouseEvent* event)
  {
    // Do nothing if no buttons are pressed or no item has been selected
    if (!rotationItem) return;
    if (!(event->buttons() & Qt::LeftButton)) return;
    /*
    // Find the item to rotate and it's rotation point
    QGraphicsItem * item = itemAt(event->buttonDownScenePos(Qt::LeftButton));
    if (!item) return;
    if (item->type() == Atom::Type) item = item->parentItem();
    if (item->type() == Bond::Type) item = item->parentItem();
    QPointF rotatePoint = item->boundingRect().center();
    QPointF rotatePointAbs = item->mapToScene(rotatePoint);
     */

  //  QPointF rotatePoint = rotationItem->boundingRect().center();
 //   QPointF rotatePointAbs = rotationItem->mapToScene(rotatePoint);


    // Calculate the rotation angle
    QPointF vec1 = lastRotationVect;
    QPointF vec2 = event->scenePos() - rotationCenter;

    qreal crossprod = vec1.x () * vec2.y () - vec1.y () * vec2.x ();
    qreal dotprod =   vec1.x () * vec2.x () + vec1.y () * vec2.y ();
    qreal rotateAngle = std::atan2 (crossprod, dotprod) * 180 / M_PI;


    //	std::cerr << rotateAngle<<"   "<<std::endl;
    //   if (event->modifiers() != Qt::AltModifier) rotateAngle = toRoundedAngle(rotateAngle);
    if (rotateAngle == 0) return;

    // Temporary rotate the item
    QTransform transform;
    transform.translate(rotationCenter.x(), rotationCenter.y());
    switch (event->modifiers()) {
      case Qt::ControlModifier: transform.rotate(rotateAngle, Qt::XAxis); break;
      case Qt::ShiftModifier: transform.rotate(rotateAngle, Qt::YAxis); break;
      default: transform.rotate(rotateAngle, Qt::ZAxis);
    };
    transform.translate(-rotationCenter.x(), -rotationCenter.y());
    rotationItem->setTransform(transform, true);
    lastRotationVect = vec2;
    //   item->rotate(rotateAngle);
  }




  void MolScene::rotateModePress(QGraphicsSceneMouseEvent* event)
  {


    QGraphicsItem * item = itemAt(event->buttonDownScenePos(Qt::LeftButton));
    if (!item) return;
    if (item->type() == Atom::Type) item = item->parentItem();
    if (item->type() == Bond::Type) item = item->parentItem();
    QPointF rotatePoint = item->boundingRect().center();
    QPointF rotatePointAbs = item->mapToScene(rotatePoint);

	rotationCenter = rotatePointAbs;
    rotationItem = item;
    lastRotationVect = event->buttonDownScenePos(Qt::LeftButton) - rotatePointAbs ; //save vector for relative rotation step

    // Execute default behavior
    QGraphicsScene::mousePressEvent(event);
  }

  void MolScene::rotateModeRelease(QGraphicsSceneMouseEvent* event)
  {
    rotationItem = NULL;
    /*

    // Find the item to rotate find the rotate point
    QGraphicsItem * item = itemAt(event->buttonDownScenePos(Qt::LeftButton));
    if (!item) return;
    if (item->type() == Atom::Type) item = item->parentItem();
    if (item->type() == Bond::Type) item = item->parentItem();
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
     */
  }

void MolScene::minimiseMolecule (Molecule *mol) {
	Minimise *minimise = new Minimise (m_bondLength);
	//minimise ->minimiseMolecule (mol);
	minimise ->conformationalSearchMolecule(mol);
	
}
	
	
	void MolScene::mirrorBondInMolecule (Molecule *mol, Bond *bo) {
		Minimise *minimise = new Minimise (m_bondLength);
		//minimise ->minimiseMolecule (mol);
		minimise ->mirrorBondInMolecule(mol, bo);
		
	}	
	

void MolScene::addModeDoubleClick (QGraphicsSceneMouseEvent *event) {
	QPointF downPos = event->buttonDownScenePos(event->button());

        qDebug() << "downPos = " << downPos;

	//clicking on an atom spawns a new bond from that atom
	if (atomAt (downPos)) {
		QPointF new_atom_pos = downPos;
		Atom *at1 = atomAt (downPos);
		switch (at1 ->numBonds()) {
			case 0:
                          {
				qreal x = new_atom_pos.x ();
				new_atom_pos.setX (x + m_bondLength);
				break;
                          }
			case 1:
			{
				Atom *at2 = at1 ->neighbors()[0];
				if (at2 ->neighbors() .size ()< 2) {
				QPointF v = downPos - at1 ->neighbors()[0] ->pos ();

				QPointF rotated_v (v.x()*0.5 - v.y()*sqrt(3)*0.5, v.x()*0.5*sqrt(3) + v.y () *0.5);
				qreal mod = sqrt (rotated_v.x()*rotated_v.x() + rotated_v.y()*rotated_v.y());
				rotated_v *= m_bondLength/mod;
				new_atom_pos = rotated_v + downPos;
				}
				else {
					Atom *at3 = at2 ->neighbors()[0];
					if (at3 == at1) at3 = at2 ->neighbors()[1];
					QPointF rotated_v = at2 ->pos () - at3 ->pos ();
					qreal mod = sqrt (rotated_v.x()*rotated_v.x() + rotated_v.y()*rotated_v.y());
					rotated_v *= m_bondLength/mod;


					new_atom_pos = rotated_v + downPos;

				}
			}
				break;
			case 2:
			{
				Atom *n1 = at1 ->neighbors()[0];
				Atom * n2 = at1 ->neighbors()[1];
				QPointF v1 = n1 ->pos ();
				QPointF v2 = n2 ->pos ();
				QPointF v3 = v1 + v2;
				QPointF v4 (v3.x () / 2, v3.y () / 2);
				QPointF v5 =  at1 ->pos () - v4;
				qreal mod = sqrt (v5.x()*v5.x() + v5.y()*v5.y());
				v5 = QPointF (v5.x()/mod * m_bondLength, v5.y()/mod * m_bondLength);
				new_atom_pos = v5 + at1->pos();
			}
				break;
			default:
				break;
		}
		if (new_atom_pos != downPos) {
			m_stack->beginMacro("Add Bond");
			Atom* atom = new Atom(new_atom_pos,m_currentElementSymbol,m_autoAddHydrogen);
			m_stack->push(new AddAtom(atom,at1 ->molecule()));
			Bond* bond = new Bond(at1,atom);
			m_stack->push(new AddBond(bond));
			for (int i = 0; i < m_bondOrder - 1; i++) 
				m_stack->push(new IncOrder(bond));
			for (int i = 0; i < m_bondType; i++) 
				m_stack->push(new IncType(bond));
			m_stack->endMacro();
		}
	}
		
	
}
	
  void MolScene::minimiseAllMolecules () {

    foreach(QGraphicsItem* item, items())	{
      if (item->type() == Molecule::Type) {
        Molecule* mol = dynamic_cast<Molecule*>(item);
        minimiseMolecule (mol);
      }
    }	
  }



  void MolScene::addModePress(QGraphicsSceneMouseEvent* event)
  {
    // Get the position
    QPointF downPos = event->buttonDownScenePos(event->button());

    // Check for bond click
    if (bondAt(downPos) && !atomAt(downPos)) {
      if (m_hintMoleculeItems)
        return;

      if (event->modifiers() == Qt::SHIFT)
        m_stack->push(new IncType(bondAt(downPos),tr("change of bondtype")));
      else
        m_stack->push(new IncOrder(bondAt(downPos),tr("change of bondorder")));
      return;
    }

    // Show hinting
    if (!m_hintMoleculeItems)
    if (m_currentElementSymbol != "+" && m_currentElementSymbol != "-" && m_currentElementSymbol != "H+" && m_currentElementSymbol != "H-")
    {
      drawHintPoints(downPos);
      setHintLine(downPos,event->scenePos());
      m_hintLine->setVisible(true);
    }

    // Check which molecule has been clicked on
    Atom* atom = atomAt(downPos);

    if (!atom) {
      //
      // mousePress in empty space
      //
      if (m_hintMoleculeItems) {
        // insert the hinted molecule if it exists
        if (!m_hintRingPoints.isEmpty()) {
          //insertRing(downPos);
        } else {
          Molecule* mol = new Molecule;
          mol->setPos(downPos);
          m_stack->beginMacro("Add Molecule");
          m_stack->push(new AddItem(mol,this));

          Q_CHECK_PTR(m_hintMolecule);
          foreach (Atom *hintAtom, m_hintMolecule->atoms())
            m_stack->push(new AddAtom(new Atom( hintAtom->scenePos(), hintAtom->element(), m_autoAddHydrogen ), mol));
          m_stack->endMacro();
        }

      } else {
        if (m_currentElementSymbol!="+" && m_currentElementSymbol!="-" && m_currentElementSymbol != "H+" && m_currentElementSymbol != "H-") {
          // insert a new atom
          //Molecule* mol = new Molecule;
          //mol->setPos(downPos);
          //m_stack->beginMacro("Add Molecule");
          //m_stack->push(new AddItem(mol,this));
          //m_stack->push(new AddAtom(new Atom( downPos, m_currentElementSymbol, m_autoAddHydrogen ), mol));
          //m_stack->endMacro();
          addItem(new Atom( downPos, m_currentElementSymbol, m_autoAddHydrogen ));
        }
      }
    }
  }

  void MolScene::addModeMove(QGraphicsSceneMouseEvent* event)
  {
    //   QPointF startPos = event->buttonDownScenePos(Qt::LeftButton);
    //   QPointF curPos = event->scenePos();
    //   Atom* a1 = getAtom(startPos);
    //   Atom* a2 = getAtom(curPos);
    //   Bond* b = getBond(startPos);

    // Check if hovering over an atom
    //   Atom* atom = atomAt(event->scenePos());

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
    Atom* a1 = atomAt(downPos);
    Atom* a2 = atomAt(upPos);
    Bond* b = bondAt(downPos);
    Molecule* m1 = a1 ? a1->molecule() : 0;
    Molecule* m2 = a2 ? a2->molecule() : 0;

    // Add aligned hint molecule if applicable
    if (m_hintMoleculeItems /*&& (a1 || b)*/) {
      insertRing(upPos);
      return;
    }

    /*
    qDebug() << "a1 =" << a1;
    qDebug() << "a2 =" << a2;
    qDebug() << "m1 =" << m1;
    qDebug() << "m2 =" << m2;
    */
    if (a1 != a2)
      m_stack->beginMacro("Draw");

    // Make sure both molecules are valid
    if (m1 && !m2 && a2) {
      m2 = m1;
      a2->setMolecule(m2);
      m_stack->push(new AddAtom(a2, m2));
    } else if (m2 && !m1 && a1) {
      m1 = m2;
      a1->setMolecule(m1);
      m_stack->push(new AddAtom(a1, m1));
    }

    /*
    qDebug() << "    a1 =" << a1;
    qDebug() << "    a2 =" << a2;
    qDebug() << "    m1 =" << m1;
    qDebug() << "    m2 =" << m2;
    */



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
    if (a2) {

      // Check for atom click
      if (a1 == a2) {
        if (a1->element() != m_currentElementSymbol) {
          // Press + Release with different symbol -> Change Element
          m_stack->push(new ChangeElement(a1, m_currentElementSymbol, tr("Change Element")));
        } else {
          if (!m1) {
            m1 = new Molecule;
            //m_stack->beginMacro("Add Atom");
            m_stack->push(new AddItem(m1, this));
            m_stack->push(new AddAtom(a1,m1));
            m_stack->endMacro();
          }
        }
        return;
      }


      // Check for merge
      if (m1 && m2 && (m1 != m2)) {
        m_stack->push(new MergeMol(m1, m2, m1));
        a1 = m1->atomAt(downPos);
        a2 = m1->atomAt(upPos);
        m1->setFocus();
      } 

      // Adding bond
      Bond* bond = new Bond(a1,a2);
      m_stack->push(new AddBond(bond));
      for (int i = 0; i < m_bondOrder - 1; i++) 
        m_stack->push(new IncOrder(bond));
      for (int i = 0; i < m_bondType; i++) 
        m_stack->push(new IncType(bond));

      // End adding macro
      m_stack->endMacro();
      return;
    }

    // Else scene release
    //m_stack->beginMacro(tr("Add Atoms"));
    if (!m1) {
      m1 = new Molecule;
      m_stack->push(new AddItem(m1,this));
      m_stack->push(new AddAtom(a1,m1));
    }

    Atom* atom = new Atom(upPos,m_currentElementSymbol,m_autoAddHydrogen);
    m_stack->push(new AddAtom(atom,m1));
    Bond* bond = new Bond(a1,atom);
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
    Atom* a = atomAt(event->scenePos());
    Bond* b = bondAt(event->scenePos());
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
	  if ( !inputTextItem ->hasFocus ()) {
    // Declare item
    QGraphicsItem* item;
    Atom* atom;
    //   Bond* bond;
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
        //         if (item->type() == Bond::Type)
        //         {
        //           bond = dynamic_cast<Bond*>(item);
        //           mol = bond->molecule();
        //           m_stack->push(new DelBond(bond));
        //           if (mol->canSplit()) m_stack->push(new SplitMol(mol));
        //         };

        // Then delete all selected atoms
        foreach (item, selectedItems())
          if (item->type() == Atom::Type)
          {
            atom = dynamic_cast<Atom*>(item);
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
		case Qt::Key_Backspace:
			m_stack->beginMacro(tr("removing item(s)"));
			// First delete all selected molecules
			foreach (item, selectedItems())
			if (item->type() == Molecule::Type)
			{
				m_stack->push(new DelItem(item));
			}
			//       // Then delete 
			//       foreach (item, selectedItems())
			//         if (item->type() == Bond::Type)
			//         {
			//           bond = dynamic_cast<Bond*>(item);
			//           mol = bond->molecule();
			//           m_stack->push(new DelBond(bond));
			//           if (mol->canSplit()) m_stack->push(new SplitMol(mol));
			//         };
			
			// Then delete all selected atoms
			foreach (item, selectedItems())
			if (item->type() == Atom::Type)
			{
				atom = dynamic_cast<Atom*>(item);
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
  
    // execute default behaviour (needed for text tool)
    QGraphicsScene::keyPressEvent(keyEvent);
  }
	
	
	Molecule *MolScene::toMol (OpenBabel::OBMol *obmol) {
		qreal k = bondLength() / 1.5;
		Molecule *mol = new Molecule ();
		mol->setPos(QPointF(0,0));
		qreal x = 0;
		qreal y = 0;
		OpenBabel::OBAtom *first_atom = obmol ->GetAtom (1);
		if (first_atom) {
			x = first_atom ->x ();
			y = -first_atom ->y ();
		}
		std::vector <Atom *>ats;
		std::vector <Bond *>bonds;

		//	for (unsigned int i = 0; i <= obmol ->NumAtoms();i++)
		FOR_ATOMS_OF_MOL(obatom,obmol)
		{
			//	OpenBabel::OBAtom *obatom = obmol ->GetAtom(i);
			//  			scene->addRect(QRectF(atom->GetX(),atom->GetY(),5,5));
			//           Atom* atom =
			ats.push_back (new Atom (QPointF((obatom->x() - x)*k,(-obatom->y()-y)*k),number2symbol(obatom->GetAtomicNum()), autoAddHydrogen ()));
			//mol->addAtom();
		}
		
		// Add bonds one-by-one
		/// Mind the numbering!
		//	for (unsigned int i = 0; i < obmol ->NumBonds();i++)
		FOR_BONDS_OF_MOL(obbond,obmol)
		{
			// Loading the OpenBabel objects
			//	OpenBabel::OBBond *obbond = obmol ->GetBond(i);
			OpenBabel::OBAtom *a1 = obbond->GetBeginAtom();
			OpenBabel::OBAtom *a2 = obbond->GetEndAtom();
			if (a1 ->IsHydrogen()) continue;
			if (a2 ->IsHydrogen()) continue;
			
			Atom* atomA = 0;
			Atom* atomB = 0;
			
			if ((a1 ->GetIdx() -1) >=0 && (a1 ->GetIdx() -1) <ats.size ()) 
			atomA = ats [a1 ->GetIdx() -1];
			if ((a2 ->GetIdx() -1) >=0 && (a2 ->GetIdx() -1) <ats.size ()) 
			atomB = ats [a2 ->GetIdx() -1];
			std::cerr<< a2 ->GetIdx() -1<<"  "<<a1 ->GetIdx() -1<<std::endl;

			if (atomA && atomB)	{
				Bond* bond  = new Bond (atomA,atomB,obbond->GetBondOrder());
				// Set special bond types
				if (obbond->IsWedge())
					bond->setType( Bond::Wedge );
				if (obbond->IsHash()) 
					bond->setType( Bond::Hash );
				
				bonds.push_back(bond);

			}
			// Normalizing
			//             factor = scene->getBondLength()/obbond->GetLength();
		}
		
		// // Normalizing molecule
		// mol->scale(factor,factor);
		// mol->setAtomSize(LABEL_SIZE/factor);
		for (unsigned int i=0; i < ats.size (); i++) {
			if (ats[i] ->element () == "H") continue;
			mol ->addAtom (ats[i]);
		}
		for (unsigned int i=0; i < bonds.size (); i++) {

			Atom *a1 = bonds[i] ->beginAtom ();
			Atom *a2 = bonds[i] ->endAtom ();
			if (a1 ->element () == "H") continue;
			if (a2 ->element () == "H") continue;
			mol ->addBond (bonds[i]);
		}		
		int nu = 0;
		QList <Atom *> atts = mol ->atoms ();
		for (unsigned int i = 0; i < atts.size () ; i++) {
			atts[i] ->n = i; 
		}
		return mol;
		
	}
	
	void MolScene::addAndMinimise (OpenBabel::OBMol *obmol) {
		Molecule *mol = toMol (obmol);
		minimiseMolecule (mol);
		
		Molecule *m1 = new Molecule;
		m_stack->beginMacro("Add Molecule");
		m_stack->push(new AddItem(mol, this));
		
		
		
	/*	
		foreach (Atom* a1, m1 ->atoms ())
				 {
				 m_stack->push(new AddAtom(a1,m1));
				 }
		foreach (Bond* b, m1 ->bonds ())
		{
			m_stack->push(new AddBond(b));
		}	
		m_stack->endMacro();
	 */
	//	delete mol;
				 //	std::cerr << mol ->atoms ().size () <<std::endl;
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
    Q_ASSERT( type >= Bond::InPlane and type < Bond::NoType );
    m_bondType = type;

    // Delete the hint molecule if it exists
    if (m_hintMoleculeItems) {
      delete m_hintMoleculeItems;
      m_hintMoleculeItems = 0;
    }
    m_hintRingPoints.clear();

    switch (type) {
      case Bond::Wedge:
      case Bond::Hash:
      case Bond::WedgeOrHash:
        setBondOrder(1);
        break;
      case Bond::CisOrTrans:
        setBondOrder(2);
        break;
      default:
        break;
    }
  }

  void MolScene::setBondOrder(int order)
  {
    Q_ASSERT( order > 0 and order < 4 );
    m_bondOrder = order;
  }

  void MolScene::setElement(const QString & symbol)
  {
    m_currentElementSymbol = symbol;

    // Delete the hint molecule if it exists
    if (m_hintMoleculeItems) {
      delete m_hintMoleculeItems;
      m_hintMoleculeItems = 0;
    }
    m_hintRingPoints.clear();

    setEditMode(MolScene::DrawMode);
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
