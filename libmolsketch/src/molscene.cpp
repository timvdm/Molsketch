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
#include <QMenu>
#include <QProcess>
#include <QDir>
#include <QLibrary>
#include <QMessageBox>
#include <QSvgGenerator>
#include <QBuffer>
#if QT_VERSION < 0x050000
#include <QDesktopServices>
#else
#include <QStandardPaths>
#endif
#include <QDebug>

#include "molscene.h"

#include "element.h"
#include "atom.h"
#include "bond.h"
#include "residue.h"

#include "molecule.h"
#include "commands.h"
#include "smilesitem.h"
#include "mimemolecule.h"
#include "TextInputItem.h"
#include "math2d.h"
#include "obabeliface.h"
#include "grid.h"

#include "reactionarrow.h"
#include "mechanismarrow.h"
#include "arrow.h"
#include "actions/abstractitemaction.h"

#include <actions/genericaction.h>




namespace Molsketch {

  struct MolScene::privateData
  {
    QGraphicsRectItem *selectionRectangle;
    TextInputItem *inputItem;
    grid *Grid;
    privateData()
      : selectionRectangle(new QGraphicsRectItem),
        inputItem(new TextInputItem),
        Grid(new grid)
    {
      selectionRectangle->setPen(QPen(Qt::blue,0,Qt::DashLine));
    }

    ~privateData()
    {
      delete selectionRectangle;
      if (!Grid->scene()) delete Grid;
    }

    bool gridOn()const { return Grid->scene(); }
  };

  using namespace Commands;


  //////////////////////////////////////////////////////////////////////////////
  //
  // Constructor & destructor
  //
  //////////////////////////////////////////////////////////////////////////////

  MolScene::MolScene(QObject* parent)
        : QGraphicsScene(parent),
          m_bondWidth(2),
          m_arrowLineWidth(1.5),
          d(new privateData)
  {
        // Set the default color to black
        m_color = QColor(0, 0, 0);


	// Create the TextInputItem that will be shown to edit text in the scene
//	m_inputTextItem = new TextInputItem();
//	addItem(m_inputTextItem);
	// hide it for now...
//	m_inputTextItem->hide();


	//Initializing properties
	m_editMode = MolScene::DrawMode;
	m_atomSize = 5;
	m_carbonVisible = false;
	m_hydrogenVisible = true;
	m_chargeVisible = true;
	m_electronSystemsVisible = false;
	m_autoAddHydrogen = true;
	m_renderMode = RenderLabels;

	// Prepare undo m_stack
	m_stack = new QUndoStack(this);
	connect(m_stack, SIGNAL(indexChanged(int)), this, SIGNAL(documentChange()));
	connect(m_stack, SIGNAL(indexChanged(int)), this, SIGNAL(selectionChange()));
	connect(m_stack, SIGNAL(indexChanged(int)), this, SLOT(update()));
	connect(m_stack, SIGNAL(indexChanged(int)), this, SLOT(updateAll())) ;
	connect(this, SIGNAL(selectionChanged()), this, SLOT(selectionSlot()));

	// Set initial size
	QRectF sizerect(-5000,-5000,10000,10000);
	setSceneRect(sizerect);
  }

  MolScene::~MolScene()
  {
    delete d;
        // Clear the scene
        clear();
  }

  void MolScene::addResidue (QPointF pos, QString name)
  {
#if QT_VERSION < 0x050000
        m_stack ->push (new AddResidue (new Residue (pos, name, 0, this)));
#else
	Residue* newResidue = new Residue(pos, name, 0) ;
	addItem(newResidue) ;
	m_stack->push(new AddResidue(newResidue)) ;
#endif
                }
  // Commands

  QColor MolScene::color() const
  {
        return m_color;
  }

  void MolScene::setColor (QColor c)
  {
                m_color = c;
                foreach (QGraphicsItem* item, selectedItems()) {
                        if (item->type() == Atom::Type) {
                                dynamic_cast<Atom*>(item) ->setColor(c);
                        }
                        else if (item->type() == Residue::Type) {
                                dynamic_cast<Residue*>(item) ->setColor(c);
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

  void MolScene::setAtomSize( qreal size )
  {
        m_atomSize = size;
  }



  void MolScene::alignToGrid()
  {
    setGrid(true);
        m_stack->beginMacro(tr("aligning to grid"));
        foreach(QGraphicsItem* item,items())
          if (item->type() == Molecule::Type)
                m_stack->push(new MoveItem(item,toGrid(item->scenePos()) - item->scenePos()));
        m_stack->endMacro();
        update();
  }

  void MolScene::setEditMode(int mode)
  {
    // TODO
//        // Reset moveflag (movebug)
//        foreach(QGraphicsItem* item, items())
//          item->setFlag(QGraphicsItem::ItemIsMovable, false);

//	// enable moving for all Molecule and atom items
//	foreach(QGraphicsItem* item, items())
//	  if (item->type() == Molecule::Type || item->type() == Atom::Type)
//		item->setFlag(QGraphicsItem::ItemIsSelectable,mode == MolScene::MoveMode);



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
        if (img.isNull()) return ;

        QLibrary obabeliface("obabeliface" QTVERSIONSUFFIX);
	obabeliface.load() ;
	callOsraFunctionPointer callOsraPtr = (callOsraFunctionPointer) obabeliface.resolve("call_osra") ;
	if (!callOsraPtr)
	{
	  QMessageBox::critical(0, tr("Error importing image"), tr("OpenBabel support unavailable.")) ;
	  return ;
	}
#if QT_VERSION < 0x050000
	QString tmpimg = QDesktopServices::storageLocation(QDesktopServices::TempLocation) + QDir::separator() + "osra.png";
#else
	QString tmpimg = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + QDir::separator() + "osra.png";
#endif
	img.save(tmpimg, "PNG", 100);
	Molecule* mol = callOsraPtr(tmpimg);
	if (mol)
	{
	  m_stack->beginMacro(tr("converting image using OSRA"));
	  m_stack->push(new AddItem(new Molecule(mol), this));
	  m_stack->endMacro();
	}
	else
	  QMessageBox::critical(0, tr("Error"), tr("OSRA conversion failed. Is OSRA installed?")) ;
        QFile::remove(tmpimg);
  }

#ifdef QT_DEBUG
  void MolScene::debugScene()
  {
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
  }
#endif

  void MolScene::clear()
  {
        // Purge the undom_stack
        m_stack->clear();

        QGraphicsScene::clear();

	// Reinitialize the scene
	//m_hintPoints.clear();
	//initHintItems();
	setEditMode(MolScene::DrawMode);

//	m_inputTextItem = new TextInputItem();
//	addItem(m_inputTextItem);
	// hide it for now...
//	m_inputTextItem->hide();

  }

  QImage MolScene::renderMolToImage (Molecule *mol)
  {
                QRectF rect = mol ->boundingRect();
                QImage image(int(rect.width()),int(rect.height()),QImage::Format_RGB32);
                image.fill(QColor("white").rgb());

		// Creating and setting the painter
		QPainter painter(&image);
		painter.setRenderHint(QPainter::Antialiasing);

		// Rendering in the image and saving to file
		render(&painter,QRectF(0,0,rect.width(),rect.height()),QRectF (mol ->mapToScene (rect.topLeft ()), mol ->mapToScene (rect.bottomRight ())));
                return image;
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

  void MolScene::addMolecule(Molecule* mol)
  {
        Q_CHECK_PTR(mol);
        if (!mol) return;
        m_stack->beginMacro(tr("add molecule"));
        m_stack->push(new AddItem(mol,this));
        if (mol->canSplit()) m_stack->push(new SplitMol(mol));
        m_stack->endMacro();
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


  // Queries

  bool MolScene::carbonVisible( ) const
  {
        return m_carbonVisible;
  }

  bool MolScene::hydrogenVisible( ) const
  {
        return m_hydrogenVisible;
  }



  qreal MolScene::atomSize( ) const
  {
        return m_atomSize;
  }

  int MolScene::editMode() const
  {
        return m_editMode;
  }

  MolScene::RenderMode MolScene::renderMode() const
  {
        return m_renderMode;
  }

  void MolScene::setRenderMode(MolScene::RenderMode mode)
  {
        m_renderMode = mode;
  }

  QPointF MolScene::toGrid(const QPointF &position)
  {
        QPointF p = position;
        int factor = 40;
        p.rx() = floor(p.x() / factor) * factor;
        p.ry() = floor(p.y() / factor) * factor;

        return p;
  }
  qreal MolScene::arrowLineWidth() const
  {
        return m_arrowLineWidth;
  }

  void MolScene::setArrowLineWidth(const qreal &arrowLineWidth)
  {
    m_arrowLineWidth = arrowLineWidth;
  }

  QPointF MolScene::snapToGrid(const QPointF &point, bool force)
  {
    if (!d->gridOn() && !force) return point;
    return d->Grid->alignPoint(point);
  }

  bool MolScene::snappingToGrid() const
  {
    return d->gridOn();
  }

  void MolScene::setGrid(bool on)
  {
    if (on) addItem(d->Grid);
    else removeItem(d->Grid);
  }

  abstractXmlObject *MolScene::produceChild(const QString &childName, const QString &type)
  {
    graphicsItem *object = 0 ;
    if (childName == "molecule") // TODO move those names to their classes.
      object = new Molecule;
    if (childName == "arrow")
      object = new Arrow;
    if (childName == "object")
    {
      if (type == "ReactionArrow") object = new ReactionArrow ;
      if (type == "MechanismArrow") object = new MechanismArrow ;
    }
    if (childName == "plugin")
      object = ItemPluginFactory::createInstance(type);
    if (object) addItem(object) ;
    return object ;
  }

  QList<const abstractXmlObject *> MolScene::children() const
  {
        QList<const abstractXmlObject*> childrenList ;
        foreach(QGraphicsItem* item, items())
        {
          graphicsItem *gItem = dynamic_cast<graphicsItem*>(item);
          if (gItem && gItem->parentItem()) continue;
          childrenList << dynamic_cast<const abstractXmlObject*>(item) ;
        }
        return childrenList ;
  }

  void MolScene::readAttributes(const QXmlStreamAttributes &attributes)
  {
    foreach(const QXmlStreamAttribute& attribute, attributes)
      setProperty(attribute.name().toLatin1(), attribute.value().toString());
    clear();
  }

  QXmlStreamAttributes MolScene::xmlAttributes() const
  {
    QXmlStreamAttributes attributes;
    foreach (const QByteArray& name, dynamicPropertyNames()) // TODO handle int, double, bool appropriately
      attributes.append(name, property(name).toString());
    return attributes;
  }

  qreal MolScene::bondWidth() const
  {
        return m_bondWidth;
  }

  void MolScene::setBondWidth(const qreal &bondWidth)
  {
        m_bondWidth = bondWidth;
  }



  Molecule* MolScene::moleculeAt(const QPointF &pos)
  {
        // Check if there is a molecule at this position
        foreach(QGraphicsItem* item,items(pos))
          if (item->type() == Molecule::Type) return dynamic_cast<Molecule*>(item);

	// Else return NULL
	return 0;

  }

  bool MolScene::textEditItemAt (const QPointF &pos)
  {
//                foreach(QGraphicsItem* item,items(pos))
//                if (item->type() == TextInputItem::Type) return true;
                  return false;
  }

  TextInputItem *MolScene::inputItem()
  {
    return d->inputItem;
  }

  void MolScene::selectionSlot()
  {
        foreach(abstractItemAction* itemAction, findChildren<abstractItemAction*>())
          itemAction->setItems(selectedItems());
        return;
  }

  Atom* MolScene::atomAt(const QPointF &pos)
  {
        // Check if there is a atom at this position
        foreach(QGraphicsItem* item,items(pos))
          if (item->type() == Atom::Type) return dynamic_cast<Atom*>(item);

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


  //////////////////////////////////////////////////////////////////////////////
  //
  // Text Mode
  //
  //////////////////////////////////////////////////////////////////////////////

	void MolScene::textModePress(QGraphicsSceneMouseEvent* event) {
		if (textEditItemAt (event ->scenePos())) {
//			m_inputTextItem ->setFocus();
		}
		else {
		Atom * atom = atomAt(event->scenePos());
		if (atom) {
//			m_inputTextItem ->clickedOn (atom);

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

        void MolScene::textModeRelease(QGraphicsSceneMouseEvent* event)
  {
                  Q_UNUSED(event)
                }

		void MolScene::updateAll()
		{
		  invalidate() ;
		  update() ;
		}

































  void MolScene::keyPressEvent(QKeyEvent* keyEvent)
  {
//          if ( !m_inputTextItem ->hasFocus ()) {
//        // Declare item
//        QGraphicsItem* item;
//        Atom* atom;
//        //   Bond* bond;
//        //   Molecule* mol;
//        QSet<Molecule*> molSet;

//	switch (keyEvent->key())
//	{
//	  case Qt::Key_Delete:
//		m_stack->beginMacro(tr("removing item(s)"));
//		// First delete all selected molecules
//		foreach (item, selectedItems())
//		  if (item->type() == Molecule::Type)
//		  {
//			m_stack->push(new DelItem(item));
//		  }
//		//       // Then delete
//		//       foreach (item, selectedItems())
//		//         if (item->type() == Bond::Type)
//		//         {
//		//           bond = dynamic_cast<Bond*>(item);
//		//           mol = bond->molecule();
//		//           m_stack->push(new DelBond(bond));
//		//           if (mol->canSplit()) m_stack->push(new SplitMol(mol));
//		//         };

//		// Then delete all selected atoms
//		foreach (item, selectedItems())
//		  if (item->type() == Atom::Type)
//		  {
//			atom = dynamic_cast<Atom*>(item);
//			molSet << atom->molecule();
//			m_stack->push(new DelAtom(atom));
//		  }

//		// Cleanup the affected molecules
//		foreach (Molecule* mol, molSet)
//		{
//		  if (mol->canSplit()) m_stack->push(new SplitMol(mol));
//		  if (mol->atoms().isEmpty()) m_stack->push(new DelItem(mol));
//		}

//		// Finally delete all the residues
//		foreach (item, selectedItems()) m_stack->push(new DelItem(item));

//		m_stack->endMacro();
//		keyEvent->accept();
//		break;
//		case Qt::Key_Backspace:
//			m_stack->beginMacro(tr("removing item(s)"));
//			// First delete all selected molecules
//			foreach (item, selectedItems())
//			if (item->type() == Molecule::Type)
//			{
//				m_stack->push(new DelItem(item));
//			}
//			//       // Then delete
//			//       foreach (item, selectedItems())
//			//         if (item->type() == Bond::Type)
//			//         {
//			//           bond = dynamic_cast<Bond*>(item);
//			//           mol = bond->molecule();
//			//           m_stack->push(new DelBond(bond));
//			//           if (mol->canSplit()) m_stack->push(new SplitMol(mol));
//			//         };

//			// Then delete all selected atoms
//			foreach (item, selectedItems())
//			if (item->type() == Atom::Type)
//			{
//				atom = dynamic_cast<Atom*>(item);
//				molSet << atom->molecule();
//				m_stack->push(new DelAtom(atom));
//			}

//			// Cleanup the affected molecules
//			foreach (Molecule* mol, molSet)
//		{
//			if (mol->canSplit()) m_stack->push(new SplitMol(mol));
//			if (mol->atoms().isEmpty()) m_stack->push(new DelItem(mol));
//		}

//			// Finally delete all the residues
//			foreach (item, selectedItems()) m_stack->push(new DelItem(item));

//			m_stack->endMacro();
//			keyEvent->accept();
//			break;


//	  case Qt::Key_Up:
//		m_stack->beginMacro("moving item(s)");
//		foreach (item, selectedItems())
//		  m_stack->push(new MoveItem(item,QPointF(0,-10)));
//		m_stack->endMacro();
//		keyEvent->accept();
//		break;
//	  case Qt::Key_Down:
//		m_stack->beginMacro("moving item(s)");
//		foreach (item, selectedItems())
//		  m_stack->push(new MoveItem(item,QPointF(0,10)));
//		m_stack->endMacro();
//		keyEvent->accept();
//		break;
//	  case Qt::Key_Left:
//		m_stack->beginMacro("moving item(s)");
//		foreach (item, selectedItems())
//		  m_stack->push(new MoveItem(item,QPointF(-10,0)));
//		m_stack->endMacro();
//		keyEvent->accept();
//		break;
//	  case Qt::Key_Right:
//		m_stack->beginMacro("moving item(s)");
//		foreach (item, selectedItems())
//		  m_stack->push(new MoveItem(item,QPointF(10,0)));
//		m_stack->endMacro();
//		keyEvent->accept();
//		break;
//	  case Qt::Key_Escape:
//		clearSelection();
//		break;
//	  default:
//		keyEvent->ignore();
//	}
//	  }

	// execute default behaviour (needed for text tool)
    keyEvent->ignore();
    QGraphicsScene::keyPressEvent(keyEvent);
    update();
    if (keyEvent->isAccepted()) return;
    switch (keyEvent->key())
    {
      case Qt::Key_Escape:
        keyEvent->accept();
        clearSelection();
        foreach(QAction* action, findChildren<QAction*>())
          if (action->isChecked())
            action->setChecked(false);
        break;
      default:;
    }
  }

  void MolScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
  {
        if (selectedItems().isEmpty())
        {
          QGraphicsScene::contextMenuEvent(event); // let the item handle the event
          return;
        }

	// have a selection, so scene is handling the context menu
	QMenu contextMenu;
	foreach(QGraphicsItem* qgItem, selectedItems())
	{
	  graphicsItem *item = dynamic_cast<graphicsItem*>(qgItem);
	  if (!item) continue;
	  item->prepareContextMenu(&contextMenu);
	}

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


  QUndoStack * MolScene::stack()
  {
        return m_stack;
  }

  QList<genericAction *> MolScene::sceneActions() const
  {
        return findChildren<genericAction*>();
  }

  QFont MolScene::atomSymbolFont() const
  {
        return m_atomSymbolFont;
  }

  void MolScene::setAtomSymbolFont(const QFont & font)
  {
        m_atomSymbolFont = font;
  }

} // namespace
