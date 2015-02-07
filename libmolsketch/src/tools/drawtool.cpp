#include "drawtool.h"

#include "../toolgroup.h"
#include "../atom.h"
#include "../bond.h"
#include "../molscene.h"
#include "../commands.h"
#include "../math2d.h"

#include <cmath>
#include <QDebug>

namespace Molsketch {

  using namespace Commands;

  DrawTool::DrawTool(MolScene *scene) : QObject(0), Tool(scene)
  {
    m_currentElementSymbol = "C";
    m_bondLength = 40;
    m_bondOrder = 1;
    //m_bondWidth = 0;
    m_bondType = Bond::InPlane;
    //m_atomSize = 5;
    m_bondAngle = 30;
    //m_carbonVisible = false;
    //m_hydrogenVisible = true;
    //m_chargeVisible = true;
    //m_electronSystemsVisible = false;
    m_autoAddHydrogen = true;
    //m_renderMode = RenderLabels;
    m_hintMoleculeItems = 0;

    initHintItems();
  }

  DrawTool::~DrawTool()
  {
  }

      
  QList<QAction*> DrawTool::actions()
  {
    if (!m_actions.isEmpty())
      return m_actions;

    m_hydrogenAct = new QAction(QObject::tr("H"), 0);
    connect(m_hydrogenAct, SIGNAL(triggered()), this, SLOT(hydrogenClicked()));
    m_actions.append(m_hydrogenAct);
    
    m_carbonAct = new QAction(QObject::tr("C"), 0);
    connect(m_carbonAct, SIGNAL(triggered()), this, SLOT(carbonClicked()));
    m_actions.append(m_carbonAct);
    
    m_nitrogenAct = new QAction(QObject::tr("N"), 0);
    connect(m_nitrogenAct, SIGNAL(triggered()), this, SLOT(nitrogenClicked()));
    m_actions.append(m_nitrogenAct);
    
    m_oxygenAct = new QAction(QObject::tr("O"), 0);
    connect(m_oxygenAct, SIGNAL(triggered()), this, SLOT(oxygenClicked()));
    m_actions.append(m_oxygenAct);

    m_phosphorusAct = new QAction(QObject::tr("P"), 0);
    connect(m_phosphorusAct, SIGNAL(triggered()), this, SLOT(phosphorusClicked()));
    m_actions.append(m_phosphorusAct);
    
    m_sulfurAct = new QAction(QObject::tr("S"), 0);
    connect(m_sulfurAct, SIGNAL(triggered()), this, SLOT(sulfurClicked()));
    m_actions.append(m_sulfurAct);
    
    m_fluorineAct = new QAction(QObject::tr("F"), 0);
    connect(m_fluorineAct, SIGNAL(triggered()), this, SLOT(fluorineClicked()));
    m_actions.append(m_fluorineAct);
    
    m_chlorineAct = new QAction(QObject::tr("Cl"), 0);
    connect(m_chlorineAct, SIGNAL(triggered()), this, SLOT(chlorineClicked()));
    m_actions.append(m_chlorineAct);
    
    m_bromineAct = new QAction(QObject::tr("Br"), 0);
    connect(m_bromineAct, SIGNAL(triggered()), this, SLOT(bromineClicked()));
    m_actions.append(m_bromineAct);
    
    m_iodineAct = new QAction(QObject::tr("I"), 0);
    connect(m_iodineAct, SIGNAL(triggered()), this, SLOT(iodineClicked()));
    m_actions.append(m_iodineAct);
    
    m_xAct = new QAction(QObject::tr("X"), 0);
    connect(m_xAct, SIGNAL(triggered()), this, SLOT(xClicked()));
    m_actions.append(m_xAct);

    m_rAct = new QAction(QObject::tr("R"), 0);
    connect(m_rAct, SIGNAL(triggered()), this, SLOT(rClicked()));
    m_actions.append(m_rAct);
    
    m_anyAct = new QAction(QObject::tr("Any"), 0);
    connect(m_anyAct, SIGNAL(triggered()), this, SLOT(anyClicked()));
    m_actions.append(m_anyAct);
    
    m_singleBondAct = new QAction(QIcon(":images/single.png"), QObject::tr("Single bond"), 0);
    connect(m_singleBondAct, SIGNAL(triggered()), this, SLOT(singleBondClicked()));
    m_actions.append(m_singleBondAct);

    m_hashBondAct = new QAction(QIcon(":images/hash.png"), QObject::tr("Hash bond"), 0);
    connect(m_hashBondAct, SIGNAL(triggered()), this, SLOT(hashBondClicked()));
    m_actions.append(m_hashBondAct);
    
    m_wedgeBondAct = new QAction(QIcon(":images/wedge.png"), QObject::tr("Wedge bond"), 0);
    connect(m_wedgeBondAct, SIGNAL(triggered()), this, SLOT(wedgeBondClicked()));
    m_actions.append(m_wedgeBondAct);
    
    m_wedgeOrHashBondAct = new QAction(QIcon(":images/hashOrWedge.png"), QObject::tr("Hash or Wedge bond"), 0);
    connect(m_wedgeOrHashBondAct, SIGNAL(triggered()), this, SLOT(wedgeOrHashBondClicked()));
    m_actions.append(m_wedgeOrHashBondAct);
    
    m_doubleBondAct = new QAction(QIcon(":images/double.png"), QObject::tr("Double bond"), 0);
    connect(m_doubleBondAct, SIGNAL(triggered()), this, SLOT(doubleBondClicked()));
    m_actions.append(m_doubleBondAct);
    
    m_cisTransBondAct = new QAction(QIcon(":images/cistrans.png"), QObject::tr("Cis or Trans bond"), 0);
    connect(m_cisTransBondAct, SIGNAL(triggered()), this, SLOT(cisTransBondClicked()));
    m_actions.append(m_cisTransBondAct);
    
    m_tripleBondAct = new QAction(QIcon(":images/triple.png"), QObject::tr("Triple bond"), 0);
    connect(m_tripleBondAct, SIGNAL(triggered()), this, SLOT(tripleBondClicked()));
    m_actions.append(m_tripleBondAct);

    m_cyclopropaneAct = new QAction(QIcon(":images/C1CC1.png"), QObject::tr("Cyclopropane"), 0);
    connect(m_cyclopropaneAct, SIGNAL(triggered()), this, SLOT(cyclopropaneClicked()));
    m_actions.append(m_cyclopropaneAct);

    m_cyclobutaneAct = new QAction(QIcon(":images/C1CCC1.png"), QObject::tr("Cyclobutane"), 0);
    connect(m_cyclobutaneAct, SIGNAL(triggered()), this, SLOT(cyclobutaneClicked()));
    m_actions.append(m_cyclobutaneAct);

    m_cyclopentaneAct = new QAction(QIcon(":images/C1CCCC1.png"), QObject::tr("Cyclopentane"), 0);
    connect(m_cyclopentaneAct, SIGNAL(triggered()), this, SLOT(cyclopentaneClicked()));
    m_actions.append(m_cyclopentaneAct);
  
    m_cyclohexaneAct = new QAction(QIcon(":images/C1CCCCC1.png"), QObject::tr("Cyclohexane"), 0);
    connect(m_cyclohexaneAct, SIGNAL(triggered()), this, SLOT(cyclohexaneClicked()));
    m_actions.append(m_cyclohexaneAct);
    
    m_ring7Act = new QAction(QIcon(":images/ring7.png"), QObject::tr("Cycloheptane"), 0);
    connect(m_ring7Act, SIGNAL(triggered()), this, SLOT(ring7Clicked()));
    m_actions.append(m_ring7Act);
    
    m_ring8Act = new QAction(QIcon(":images/ring8.png"), QObject::tr("Cyclooctane"), 0);
    connect(m_ring8Act, SIGNAL(triggered()), this, SLOT(ring8Clicked()));
    m_actions.append(m_ring8Act);

    m_aromatic5ringAct = new QAction(QIcon(":images/a1aaaa1.png"), QObject::tr("Aromatic 5-Ring"), 0);
    connect(m_aromatic5ringAct, SIGNAL(triggered()), this, SLOT(aromatic5ringClicked()));
    m_actions.append(m_aromatic5ringAct);
    
    m_aromatic6ringAct = new QAction(QIcon(":images/a1aaaaa1.png"), QObject::tr("Aromatic 6-Ring"), 0);
    connect(m_aromatic6ringAct, SIGNAL(triggered()), this, SLOT(aromatic6ringClicked()));
    m_actions.append(m_aromatic6ringAct);


    QActionGroup *actionGroup = new QActionGroup(0);
    actionGroup->setExclusive(false);
    foreach (QAction *action, m_actions)
      actionGroup->addAction(action);


    return m_actions;
  }

  QString DrawTool::toolbarName(QAction *action)
  {
    if (action == m_cyclopropaneAct ||
        action == m_cyclobutaneAct ||
        action == m_cyclopentaneAct ||
        action == m_cyclohexaneAct ||
        action == m_ring7Act ||
        action == m_ring8Act ||
        action == m_aromatic5ringAct ||
        action == m_aromatic6ringAct)
      return tr("Rings");
    else
      return tr("Draw");
  }

  void DrawTool::deactivated()
  {
    if (m_hintMoleculeItems)
      delete m_hintMoleculeItems;
    m_hintMoleculeItems = 0;
  }

  void DrawTool::mousePressEvent(QGraphicsSceneMouseEvent *event)
  {
    if (event->button() == Qt::RightButton)
      return ;

    if (event->button() == Qt::MiddleButton) {
      delModePress(event);
      return;
    }

    // Get the position
    QPointF downPos = event->buttonDownScenePos(event->button());
    // Get pointer the undo stack
    QUndoStack *undostack = scene()->stack();

    Bond *clickedBond = scene()->bondAt(downPos);
    Atom *clickedAtom = scene()->atomAt(downPos);

    // Check for bond click
    if (clickedBond && !clickedAtom) {
      if (m_hintMoleculeItems)
        return;

      switch (m_bondType) {
        case Bond::InPlane:
          if (clickedBond->bondType() == Bond::InPlane)
            undostack->push(new IncOrder(clickedBond, QObject::tr("Change Bond Order")));
          else
            undostack->push(new SetBondType(clickedBond, Bond::InPlane));
          break;
        case Bond::Wedge:
          if (clickedBond->bondType() == Bond::Wedge)
            undostack->push(new SetBondType(clickedBond, Bond::InvertedWedge, QObject::tr("Change Wedge Bond")));
          else
            undostack->push(new SetBondType(clickedBond, Bond::Wedge, QObject::tr("Change Wedge Bond")));
          break;
        case Bond::Hash:
          if (clickedBond->bondType() == Bond::Hash)
            undostack->push(new SetBondType(clickedBond, Bond::InvertedHash, QObject::tr("Change Hash Bond")));
          else
            undostack->push(new SetBondType(clickedBond, Bond::Hash, QObject::tr("Change Hash Bond")));
          break;
        case Bond::CisOrTrans:
          undostack->push(new SetBondType(clickedBond, Bond::CisOrTrans, QObject::tr("Change Hash Bond")));
          break;
        case Bond::WedgeOrHash:
          undostack->push(new SetBondType(clickedBond, Bond::WedgeOrHash, QObject::tr("Change Hash Bond")));
          break;
        default:
          break;
      };
      
      return;
    }

    // Show hinting
    if (!m_hintMoleculeItems) {
      drawHintPoints(downPos);
      setHintLine(downPos,event->scenePos());
      m_hintLine->setVisible(true);
    }

    if (!clickedAtom) {
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
          undostack->beginMacro("Add Molecule");
          undostack->push(new AddItem(mol, scene()));

          Q_CHECK_PTR(m_hintMolecule);
          foreach (Atom *hintAtom, m_hintMolecule->atoms())
            undostack->push(new AddAtom(new Atom( hintAtom->scenePos(), hintAtom->element(), m_autoAddHydrogen), mol));
          undostack->endMacro();
        }

      } else {
        // insert a new atom
        scene()->addItem(new Atom(downPos, m_currentElementSymbol, m_autoAddHydrogen));
      }
    }
  }

  void DrawTool::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
  {
    // Determine the right action
    if (m_hintMoleculeItems) {
      m_hintMoleculeItems->show();
      m_hintMoleculeItems->setPos(event->scenePos());
      m_hintMoleculeItems->setTransform(QTransform());

      // Get the position
      QPointF downPos = event->scenePos();

      Atom *atom = scene()->atomAt(downPos);
      if (atom)
        alignRingWithAtom(atom);

      Bond *bond = scene()->bondAt(downPos);
      if (bond)
        alignRingWithBond(bond, downPos);
    }

    // Check hinting conditions
    if (!(event->buttons() & Qt::LeftButton))
      return;
    if (!scene()->atomAt(event->buttonDownScenePos(Qt::LeftButton)))
      return;

    // Set hinting
    setHintLine(nearestPoint(event->buttonDownScenePos(Qt::LeftButton)), nearestPoint(event->scenePos()));
  }
  
  void DrawTool::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
  {
    // Remove the hinting
    removeHintLine();
    removeHintPoints();

    // Get position
    QPointF downPos = event->buttonDownScenePos(event->button());
    QPointF upPos = nearestPoint(event->scenePos());
    // Get pointer the undo stack
    QUndoStack *undostack = scene()->stack();

    // Check possible targets
    Atom* a1 = scene()->atomAt(downPos);
    Atom* a2 = scene()->atomAt(upPos);
    Bond* b = scene()->bondAt(downPos);
    Molecule* m1 = a1 ? a1->molecule() : 0;
    Molecule* m2 = a2 ? a2->molecule() : 0;

    // Add aligned hint molecule if applicable
    if (m_hintMoleculeItems) {
      insertRing(upPos);
      return;
    }

    if (a1 != a2)
      undostack->beginMacro("Draw");

    // Make sure both molecules are valid
    if (m1 && !m2 && a2) {
      m2 = m1;
      a2->setMolecule(m2);
      undostack->push(new AddAtom(a2, m2));
    } else if (m2 && !m1 && a1) {
      m1 = m2;
      a1->setMolecule(m1);
      undostack->push(new AddAtom(a1, m1));
    }

    if (!a1 && !b) return;
    if (b && !a1) return;

    // Check for atom release
    if (a2) {

      // Check for atom click
      if (a1 == a2) {
        if (a1->element() != m_currentElementSymbol) {
          // Press + Release with different symbol -> Change Element
          undostack->push(new ChangeElement(a1, m_currentElementSymbol, QObject::tr("Change Element")));
        } else {
          if (!m1) {
            m1 = new Molecule;
            //m_stack->beginMacro("Add Atom");
            undostack->push(new AddItem(m1, scene()));
            undostack->push(new AddAtom(a1,m1));
            undostack->endMacro();
          }
        }
        return;
      }


      // Check for merge
      if (m1 && m2 && (m1 != m2)) {
        undostack->push(new MergeMol(m1, m2, m1));
        a1 = m1->atomAt(a1->scenePos());
        a2 = m1->atomAt(a2->scenePos());
        m1->setFocus();
      } 

      // Adding bond
      Bond* bond = new Bond(a1,a2);
      undostack->push(new AddBond(bond));
      for (int i = 0; i < m_bondOrder - 1; i++) 
        undostack->push(new IncOrder(bond));
      undostack->push(new SetBondType(bond, m_bondType));

      // End adding macro
      undostack->endMacro();
      return;
    }

    // Else scene release
    //m_stack->beginMacro(tr("Add Atoms"));
    if (!m1) {
      m1 = new Molecule;
      undostack->push(new AddItem(m1,scene()));
      undostack->push(new AddAtom(a1,m1));
    }

    Atom* atom = new Atom(upPos,m_currentElementSymbol,m_autoAddHydrogen);
    undostack->push(new AddAtom(atom,m1));
    Bond* bond = new Bond(a1,atom);
    undostack->push(new AddBond(bond));
    for (int i = 0; i < m_bondOrder - 1; i++) 
      undostack->push(new IncOrder(bond));
    undostack->push(new SetBondType(bond, m_bondType));

    undostack->endMacro();

    scene()->update();
  }

  void DrawTool::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
  {
    switch (event->button()) {
      case Qt::LeftButton:
        this->addModeDoubleClick(event);
        break;
      case Qt::RightButton:
//        this->delModeDoubleClick(event); // this doesn't seem to work as Qt appears to  allow double clicks only from the left button.
        break;
      default:
        break;
    }
  }

  void DrawTool::leaveSceneEvent(QEvent *event)
  {
    Q_UNUSED(event)
    if (m_hintMoleculeItems)
      m_hintMoleculeItems->hide();
  }






  void DrawTool::addModeDoubleClick (QGraphicsSceneMouseEvent *event)
  {
    QUndoStack *stack = scene()->stack();
    QPointF downPos = event->buttonDownScenePos(event->button());

    qDebug() << "downPos = " << downPos;

    //clicking on an atom spawns a new bond from that atom
    if (scene()->atomAt (downPos)) {
      QPointF new_atom_pos = downPos;
      Atom *at1 = scene()->atomAt (downPos);
      switch (at1 ->numBonds()) {
        case 0:
        {
          qreal x = new_atom_pos.x ();
          new_atom_pos.setX (x + m_bondLength);
          break;
        }
        case 1:
        {
          Atom *at2 = at1 ->neighbours()[0];
          if (at2 ->neighbours() .size ()< 2) {
            QPointF v = downPos - at1 ->neighbours()[0] ->pos ();

            QPointF rotated_v (v.x()*0.5 - v.y()*sqrt(3)*0.5, v.x()*0.5*sqrt(3) + v.y () *0.5);
            qreal mod = sqrt (rotated_v.x()*rotated_v.x() + rotated_v.y()*rotated_v.y());
            rotated_v *= m_bondLength/mod;
            new_atom_pos = rotated_v + downPos;
          } else {
            Atom *at3 = at2 ->neighbours()[0];
            if (at3 == at1) at3 = at2 ->neighbours()[1];
            QPointF rotated_v = at2 ->pos () - at3 ->pos ();
            qreal mod = sqrt (rotated_v.x()*rotated_v.x() + rotated_v.y()*rotated_v.y());
            rotated_v *= m_bondLength/mod;

            new_atom_pos = rotated_v + downPos;
          }
        }
        break;
        case 2:
        {
          Atom *n1 = at1 ->neighbours()[0];
          Atom * n2 = at1 ->neighbours()[1];
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
        stack->beginMacro("Add Bond");
        Atom* atom = new Atom(new_atom_pos,m_currentElementSymbol,m_autoAddHydrogen);
        stack->push(new AddAtom(atom,at1 ->molecule()));
        Bond* bond = new Bond(at1,atom);
        stack->push(new AddBond(bond));
        for (int i = 0; i < m_bondOrder - 1; i++)
          stack->push(new IncOrder(bond));

        stack->push(new SetBondType(bond, m_bondType));
        stack->endMacro();
      }
    }


  }


  void DrawTool::delModePress(QGraphicsSceneMouseEvent* event)
  {
    QUndoStack *stack = scene()->stack();
    Atom* a = scene()->atomAt(event->scenePos());
    Bond* b = scene()->bondAt(event->scenePos());
    Molecule* mol;

    // Look for atomclick
    if (a)
    {
      mol = a->molecule();
      stack->beginMacro(tr("removing atom"));
      stack->push(new DelAtom(a));
      if (mol->canSplit())
        stack->push(new SplitMol(mol));
      if (mol->atoms().isEmpty())
        stack->push(new DelItem(mol));
      stack->endMacro();
      return;
    }

    // Look for bondclick
    if (b)
    {
      mol = b->molecule();
      stack->beginMacro(tr("removing bond"));
      stack->push(new DelBond(b));
      if (mol->canSplit())
        stack->push(new SplitMol(mol));
      stack->endMacro();
      return;
    }
  }

  void DrawTool::delModeDoubleClick( QGraphicsSceneMouseEvent * event ) // This doesn't seem to work
  {
    QUndoStack *stack = scene()->stack();
    Molecule* item = scene()->moleculeAt(event->scenePos());

    if (item)
      stack->push(new DelItem(item,tr("removing molecule")));
  }



  void DrawTool::initHintItems()
  {
    // hint molecule always starts as none
    m_hintMolecule = 0;
    m_hintMoleculeItems = 0;
    //m_mechanismArrowHint = 0;

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




  void DrawTool::setHintLine(const QPointF &startPos, const QPointF &curPos)
  {
    // Set the hintline between startPos and curPos
    m_hintLine->setLine(QLineF(startPos, curPos));

    // If not already added, add the hintline to the scene
    if (!scene()->items().contains(m_hintLine)) 
      scene()->addItem(m_hintLine);
  }

  void DrawTool::drawHintPoints(const QPointF &startPos)
  {
    QPointF position = startPos;

    // If clicked on atom, use that as startpoint
    Atom* atom = scene()->atomAt(position);
    if (atom) 
      position = atom->scenePos();

    // Draw hintpoints
    m_hintPointsGroup->setPos(position);
    scene()->addItem(m_hintPointsGroup);
  }

  void DrawTool::removeHintLine()
  {
    if (scene()->items().contains(m_hintLine)) 
      scene()->removeItem(m_hintLine);
  }

  void DrawTool::removeHintPoints()
  {
    if (scene()->items().contains(m_hintPointsGroup)) 
      scene()->removeItem(m_hintPointsGroup);
  }

   // create a QGraphicsItem for regular polygon rings
  void DrawTool::setHintRing(int ringSize, bool aromatic)
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

    m_hintMoleculeItems->hide();
    scene()->addItem(m_hintMoleculeItems);
  }

  void DrawTool::insertRing(const QPointF &pos)
  {
    Q_UNUSED(pos); // Why would you write a non-virtual, private function with an unused argument?
    // Get pointer the undo stack
    QUndoStack *undostack = scene()->stack();

    QList<Molecule*> molecules;
    // check if there are already atoms at the corners
    foreach (const QPointF &p, m_hintRingPoints) {
      Atom *atom = scene()->atomAt(m_hintMoleculeItems->mapToScene(p));
      if (atom) 
        if (atom->molecule() && !molecules.contains(atom->molecule()))
          molecules.append(atom->molecule());
    }
    undostack->beginMacro("Add Molecule");
    // create a new molecule or merge extisting molecules
    if (molecules.isEmpty()) {
      // Adding a totally new ring
      molecules.append(new Molecule);
      undostack->push(new AddItem(molecules[0], scene()));
    } else if (molecules.size() > 1) {
      // the ring merges several molecules (i.e. it does not only extend a single molecule)
      Molecule *mergedMol = molecules[0];
      for (int i = 1; i < molecules.size(); ++i)
        mergedMol = merge(mergedMol, molecules[i]);
      //undostack->push(new MergeMol(molecules[0], molecules[i], molecules[0]));
      undostack->push(new AddItem(mergedMol, scene()));
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
        undostack->push(new AddAtom(begin, molecules[0]));
      }
      Atom *end = molecules[0]->atomAt(endPoint);
      if (!end) {
        end = new Atom(endPoint, "C", m_autoAddHydrogen);
        undostack->push(new AddAtom(end, molecules[0]));
      }

      Bond *bond = molecules[0]->bondBetween(begin, end);
      if (!bond) {
        bond = new Bond(begin, end);
        undostack->push(new AddBond(bond));
      }
      bonds.append(bond);

      if (m_aromaticHintRing && (indexOfFirstDoubleBond < 0)) {

        bool beginHasDoubleBond = false;
        bool endHasDoubleBond = false;
        foreach (Atom *nbr, begin->neighbours()) {
          Bond *possibleDoubleBond = molecules[0]->bondBetween(begin, nbr);
          if (possibleDoubleBond && possibleDoubleBond->bondOrder() == 2)
            beginHasDoubleBond = true;
        }
        foreach (Atom *nbr, end->neighbours()) {
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
	  undostack->push(new IncOrder(bonds.at(i)));
    }

    molecules[0]->setFocus();
    undostack->endMacro();
  }

  void DrawTool::alignRingWithAtom(Atom *atom)
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
      QPointF moleculeNormal = atom->scenePos() - atom->neighbours()[0]->scenePos();
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
      QPointF moleculeNormal = atom->scenePos() - atom->neighbours()[0]->scenePos();
      moleculeNormal += atom->scenePos() - atom->neighbours()[1]->scenePos();
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

  void DrawTool::alignRingWithBond(Bond *bond, const QPointF &pos)
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

  Molecule * DrawTool::merge( const Molecule * molA, const Molecule* molB )
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



  QPointF DrawTool::nearestPoint(const QPointF &curPos)
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
    Atom* atom = scene()->atomAt(curPos);
    if (atom) nPoint = atom->scenePos();

    return nPoint;
  }




















  void DrawTool::hydrogenClicked()
  {
    m_currentElementSymbol = "H";
    m_bondOrder = 1;

    QAction *action = qobject_cast<QAction*>(sender());
    if (!action)
      return;

    ToolGroup *toolGroup = static_cast<ToolGroup*>(action->property("toolGroup").value<void*>());
    if (!toolGroup)
      return;

    toolGroup->setActiveTool(this);
  }

  void DrawTool::carbonClicked()
  {
    m_currentElementSymbol = "C";
    m_bondOrder = 1;

    QAction *action = qobject_cast<QAction*>(sender());
    if (!action)
      return;

    ToolGroup *toolGroup = static_cast<ToolGroup*>(action->property("toolGroup").value<void*>());
    if (!toolGroup)
      return;

    toolGroup->setActiveTool(this);
  }

  void DrawTool::nitrogenClicked()
  {
    m_currentElementSymbol = "N";
    m_bondOrder = 1;

    QAction *action = qobject_cast<QAction*>(sender());
    if (!action)
      return;

    ToolGroup *toolGroup = static_cast<ToolGroup*>(action->property("toolGroup").value<void*>());
    if (!toolGroup)
      return;

    toolGroup->setActiveTool(this);
  }

  void DrawTool::oxygenClicked()
  {
    m_currentElementSymbol = "O";
    m_bondOrder = 1;

    QAction *action = qobject_cast<QAction*>(sender());
    if (!action)
      return;

    ToolGroup *toolGroup = static_cast<ToolGroup*>(action->property("toolGroup").value<void*>());
    if (!toolGroup)
      return;

    toolGroup->setActiveTool(this);
  }

  void DrawTool::phosphorusClicked()
  {
    m_currentElementSymbol = "P";
    m_bondOrder = 1;

    QAction *action = qobject_cast<QAction*>(sender());
    if (!action)
      return;

    ToolGroup *toolGroup = static_cast<ToolGroup*>(action->property("toolGroup").value<void*>());
    if (!toolGroup)
      return;

    toolGroup->setActiveTool(this);
  }

  void DrawTool::sulfurClicked()
  {
    m_currentElementSymbol = "S";
    m_bondOrder = 1;

    QAction *action = qobject_cast<QAction*>(sender());
    if (!action)
      return;

    ToolGroup *toolGroup = static_cast<ToolGroup*>(action->property("toolGroup").value<void*>());
    if (!toolGroup)
      return;

    toolGroup->setActiveTool(this);
  }

  void DrawTool::fluorineClicked()
  {
    m_currentElementSymbol = "F";
    m_bondOrder = 1;

    QAction *action = qobject_cast<QAction*>(sender());
    if (!action)
      return;

    ToolGroup *toolGroup = static_cast<ToolGroup*>(action->property("toolGroup").value<void*>());
    if (!toolGroup)
      return;

    toolGroup->setActiveTool(this);
  }

  void DrawTool::chlorineClicked()
  {
    m_currentElementSymbol = "Cl";
    m_bondOrder = 1;

    QAction *action = qobject_cast<QAction*>(sender());
    if (!action)
      return;

    ToolGroup *toolGroup = static_cast<ToolGroup*>(action->property("toolGroup").value<void*>());
    if (!toolGroup)
      return;

    toolGroup->setActiveTool(this);
  }

  void DrawTool::bromineClicked()
  {
    m_currentElementSymbol = "Br";
    m_bondOrder = 1;

    QAction *action = qobject_cast<QAction*>(sender());
    if (!action)
      return;

    ToolGroup *toolGroup = static_cast<ToolGroup*>(action->property("toolGroup").value<void*>());
    if (!toolGroup)
      return;

    toolGroup->setActiveTool(this);
  }

  void DrawTool::iodineClicked()
  {
    m_currentElementSymbol = "I";
    m_bondOrder = 1;

    QAction *action = qobject_cast<QAction*>(sender());
    if (!action)
      return;

    ToolGroup *toolGroup = static_cast<ToolGroup*>(action->property("toolGroup").value<void*>());
    if (!toolGroup)
      return;

    toolGroup->setActiveTool(this);
  }

  void DrawTool::rClicked()
  {
    m_currentElementSymbol = "R";
    m_bondOrder = 1;

    QAction *action = qobject_cast<QAction*>(sender());
    if (!action)
      return;

    ToolGroup *toolGroup = static_cast<ToolGroup*>(action->property("toolGroup").value<void*>());
    if (!toolGroup)
      return;

    toolGroup->setActiveTool(this);
  }

  void DrawTool::xClicked()
  {
    m_currentElementSymbol = "X";
    m_bondOrder = 1;

    QAction *action = qobject_cast<QAction*>(sender());
    if (!action)
      return;

    ToolGroup *toolGroup = static_cast<ToolGroup*>(action->property("toolGroup").value<void*>());
    if (!toolGroup)
      return;

    toolGroup->setActiveTool(this);
  }

  void DrawTool::anyClicked()
  {
    m_currentElementSymbol = "Any"; //FIXME
    m_bondOrder = 1;

    QAction *action = qobject_cast<QAction*>(sender());
    if (!action)
      return;

    ToolGroup *toolGroup = static_cast<ToolGroup*>(action->property("toolGroup").value<void*>());
    if (!toolGroup)
      return;

    toolGroup->setActiveTool(this);
  }

  void DrawTool::singleBondClicked()
  {
    m_bondType = Bond::InPlane;
    m_bondOrder = 1;

    QAction *action = qobject_cast<QAction*>(sender());
    if (!action)
      return;

    ToolGroup *toolGroup = static_cast<ToolGroup*>(action->property("toolGroup").value<void*>());
    if (!toolGroup)
      return;

    toolGroup->setActiveTool(this);
  }

  void DrawTool::doubleBondClicked()
  {
    m_bondType = Bond::InPlane;
    m_bondOrder = 2;

    QAction *action = qobject_cast<QAction*>(sender());
    if (!action)
      return;

    ToolGroup *toolGroup = static_cast<ToolGroup*>(action->property("toolGroup").value<void*>());
    if (!toolGroup)
      return;

    toolGroup->setActiveTool(this);
  }

  void DrawTool::tripleBondClicked()
  {
    m_bondType = Bond::InPlane;
    m_bondOrder = 3;

    QAction *action = qobject_cast<QAction*>(sender());
    if (!action)
      return;

    ToolGroup *toolGroup = static_cast<ToolGroup*>(action->property("toolGroup").value<void*>());
    if (!toolGroup)
      return;

    toolGroup->setActiveTool(this);
  }

  void DrawTool::hashBondClicked()
  {
    m_bondType = Bond::Hash;
    m_bondOrder = 1;

    QAction *action = qobject_cast<QAction*>(sender());
    if (!action)
      return;

    ToolGroup *toolGroup = static_cast<ToolGroup*>(action->property("toolGroup").value<void*>());
    if (!toolGroup)
      return;

    toolGroup->setActiveTool(this);
  }

  void DrawTool::wedgeBondClicked()
  {
    m_bondType = Bond::Wedge;
    m_bondOrder = 1;

    QAction *action = qobject_cast<QAction*>(sender());
    if (!action)
      return;

    ToolGroup *toolGroup = static_cast<ToolGroup*>(action->property("toolGroup").value<void*>());
    if (!toolGroup)
      return;

    toolGroup->setActiveTool(this);
  }

  void DrawTool::wedgeOrHashBondClicked()
  {
    m_bondType = Bond::WedgeOrHash;
    m_bondOrder = 1;

    QAction *action = qobject_cast<QAction*>(sender());
    if (!action)
      return;

    ToolGroup *toolGroup = static_cast<ToolGroup*>(action->property("toolGroup").value<void*>());
    if (!toolGroup)
      return;

    toolGroup->setActiveTool(this);
  }

  void DrawTool::cisTransBondClicked()
  {
    m_bondType = Bond::CisOrTrans;
    m_bondOrder = 1;

    QAction *action = qobject_cast<QAction*>(sender());
    if (!action)
      return;

    ToolGroup *toolGroup = static_cast<ToolGroup*>(action->property("toolGroup").value<void*>());
    if (!toolGroup)
      return;

    toolGroup->setActiveTool(this);
  }

  void DrawTool::cyclopropaneClicked()
  {
    QAction *action = qobject_cast<QAction*>(sender());
    if (!action)
      return;

    ToolGroup *toolGroup = static_cast<ToolGroup*>(action->property("toolGroup").value<void*>());
    if (!toolGroup)
      return;

    toolGroup->setActiveTool(this);
    setHintRing(3);
  }

  void DrawTool::cyclobutaneClicked()
  {
    QAction *action = qobject_cast<QAction*>(sender());
    if (!action)
      return;

    ToolGroup *toolGroup = static_cast<ToolGroup*>(action->property("toolGroup").value<void*>());
    if (!toolGroup)
      return;

    toolGroup->setActiveTool(this);
    setHintRing(4);
  }

  void DrawTool::cyclopentaneClicked()
  {
    QAction *action = qobject_cast<QAction*>(sender());
    if (!action)
      return;

    ToolGroup *toolGroup = static_cast<ToolGroup*>(action->property("toolGroup").value<void*>());
    if (!toolGroup)
      return;

    toolGroup->setActiveTool(this);
    setHintRing(5);
  }

  void DrawTool::cyclohexaneClicked()
  {
    QAction *action = qobject_cast<QAction*>(sender());
    if (!action)
      return;

    ToolGroup *toolGroup = static_cast<ToolGroup*>(action->property("toolGroup").value<void*>());
    if (!toolGroup)
      return;

    toolGroup->setActiveTool(this);
    setHintRing(6);
  }

  void DrawTool::ring7Clicked()
  {
    QAction *action = qobject_cast<QAction*>(sender());
    if (!action)
      return;

    ToolGroup *toolGroup = static_cast<ToolGroup*>(action->property("toolGroup").value<void*>());
    if (!toolGroup)
      return;

    toolGroup->setActiveTool(this);
    setHintRing(7);
  }

  void DrawTool::ring8Clicked()
  {
    QAction *action = qobject_cast<QAction*>(sender());
    if (!action)
      return;

    ToolGroup *toolGroup = static_cast<ToolGroup*>(action->property("toolGroup").value<void*>());
    if (!toolGroup)
      return;

    toolGroup->setActiveTool(this);
    setHintRing(8);
  }

  void DrawTool::aromatic5ringClicked()
  {
    QAction *action = qobject_cast<QAction*>(sender());
    if (!action)
      return;

    ToolGroup *toolGroup = static_cast<ToolGroup*>(action->property("toolGroup").value<void*>());
    if (!toolGroup)
      return;

    toolGroup->setActiveTool(this);
    setHintRing(5, true);
  }

  void DrawTool::aromatic6ringClicked()
  {
    QAction *action = qobject_cast<QAction*>(sender());
    if (!action)
      return;

    ToolGroup *toolGroup = static_cast<ToolGroup*>(action->property("toolGroup").value<void*>());
    if (!toolGroup)
      return;

    toolGroup->setActiveTool(this);
    setHintRing(6, true);
  }


}

