#include "drawaction.h"
#include <QPainter>
#include <QVBoxLayout>
#include <QDockWidget>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItemGroup>
#include <cmath>
#include <QMainWindow>
#include "periodictablewidget.h"
#include "bondtypewidget.h"
#include "molecule.h"
#include "molscene.h"
#include "commands.h"
#include "math2d.h"
#include <QDebug>

// TODO snap to help points
namespace Molsketch {

  class drawAction::privateData
  {
  public:
    QDockWidget *dock ;
    periodicTableWidget *periodicTable;
    bondTypeWidget *bondType;
    QGraphicsPolygonItem *hintMoleculeItems;
    // TODO initialization
    QPolygonF hintRingPoints;
    QGraphicsLineItem hintLine;
    Molecule* hintMolecule;
    bool autoAddHydrogen;
    QGraphicsItemGroup hintPointsGroup;
    qreal bondAngle;
    qreal bondLength;
    drawAction *parent;

    privateData(drawAction* p) :
      hintMoleculeItems (0),
      hintLine(QLineF(0,0,0,0)),
      hintMolecule(0),
      bondAngle(30), // TODO modifiable
      bondLength(40), // TODO modifiable
      parent(p)
    {
      hintLine.setAcceptedMouseButtons(Qt::NoButton);
      hintLine.setZValue(-1);

      // Initialize hint point circle
      for (int i = 0; i < 360./bondAngle ; ++i)
      {
        QGraphicsEllipseItem* dot = new QGraphicsEllipseItem(-2.5,-2.5,5,5);
        dot->setBrush(Qt::lightGray);
        dot->setPen(Qt::NoPen);
        dot->setPos(QLineF::fromPolar(bondLength, bondAngle * i).p2());
        hintPointsGroup.addToGroup(dot);
      }
      hintPointsGroup.setAcceptedMouseButtons(Qt::NoButton);
    }

    void createHintRing(int size, bool& aromatic) // TODO use real molecule
    {
      delete hintMoleculeItems;
      hintMoleculeItems = 0;
      if (size < 3) return;
      if (5 > size || 6 < size) aromatic = false ; // only do Cp or Ph

      hintRingPoints.clear();
      qreal radius = bondLength / (2*sin(M_PI/size));
      for (int i = 0; i < size ; ++i)
        hintRingPoints.append(QLineF::fromPolar(radius, 360.*i/size).p2());

      hintMoleculeItems = new QGraphicsPolygonItem(hintRingPoints);
      hintMoleculeItems->setPen(QPen(Qt::lightGray));
      hintMoleculeItems->hide();
      parent->scene()->addItem(hintMoleculeItems);
    }

    void insertRing(QUndoStack* undostack, bool aromatic)
    {
      undostack->beginMacro(tr("Add molecule"));
      Molecule *newMolecule = new Molecule();
      undostack->push(new Commands::AddItem(newMolecule, parent->scene()));

      // create/find atoms
      QList<Atom*> ringAtoms;
      foreach(const QPointF& vertex, hintRingPoints)
      {
        Atom *atom = parent->scene()->atomAt(vertex); // TODO check (no mapping)
        if (!atom)
        {
          atom = new Atom(vertex, "C", autoAddHydrogen);
          undostack->push(new Commands::AddAtom(atom, newMolecule));
        }
        else
          *newMolecule += *(atom->molecule()); // TODO remove old molecule?
        ringAtoms << atom;
      }

      // create bonds
      QList<Bond*> ringBonds;
      for (int i = 0 ; i < ringAtoms.size() ; ++i)
      {
        int next = (i == ringAtoms.size()-1 ? 0 : i+1);
        Bond* bond = newMolecule->bondBetween(ringAtoms[i], ringAtoms[next]);
        if (!bond)
        {
          bond = new Bond(ringAtoms[i], ringAtoms[next]);
          undostack->push(new Commands::AddBond(bond));
        }
        ringBonds << bond;
      }

      if (!aromatic) return;
      // create aromatic bonds
      // TODO check!
      foreach(Bond* bond, ringBonds)
      {
        bool canBeDouble = true;
        foreach(Bond* otherBond, bond->beginAtom()->bonds() + bond->endAtom()->bonds())
          canBeDouble = canBeDouble && otherBond->bondOrder() < 2 ;
        if (!canBeDouble) continue;
        undostack->push(new Commands::IncOrder(bond));
      }
    }

    QPointF nearestPoint(const QPointF& currentPosition)
    {
      QPointF nPoint = currentPosition ;

      // Check the hinting points
      qreal minDistance = bondLength / 4.;
      foreach(const QGraphicsItem* hintPoint, hintPointsGroup.childItems())
      {
        qreal distance = QLineF(hintPoint->scenePos(), currentPosition).length();
        if (distance > minDistance) continue;
        minDistance = distance;
        nPoint = hintPoint->scenePos();
      }

      // Look whether a atom is nearby
      Atom* atom = parent->scene()->atomAt(currentPosition);
      if (atom) nPoint = atom->scenePos();

      return nPoint;
    }

    void alignRingWithAtom(Atom *atom) // TODO partial merge with alignRingWithBond()
    {
      Q_CHECK_PTR(hintMoleculeItems);

      if (hintRingPoints.size() < 2) return;


      QPointF moleculeNormal = QPointF(0.0, -1.0);
      QPointF ringNormal = normalized(hintRingPoints.first());
      QPointF rp = hintRingPoints.first();
      if (atom->numBonds())
      {
        // rotate/translate ring to align with the bond
        moleculeNormal = atom->scenePos() - atom->neighbours().first()->scenePos();
        if (atom->numBonds() > 1)
          moleculeNormal += atom->scenePos() - atom->neighbours()[1]->scenePos();
        moleculeNormal = normalized(moleculeNormal);
      }

      qreal ang = angle(moleculeNormal, ringNormal) * 180.0 / M_PI;
      if (angleSign(moleculeNormal, ringNormal) > 0.0)
        ang = -ang;
      hintMoleculeItems->setTransform(
            QTransform().rotate(ang+180.0).translate(-rp.x(), -rp.y()));
      if (atom->numBonds()) hintMoleculeItems->setPos(atom->scenePos());
    }

    void alignRingWithBond(Bond *bond, const QPointF &pos)
    {
      Q_CHECK_PTR(hintMoleculeItems);
      Q_CHECK_PTR(bond->beginAtom());
      Q_CHECK_PTR(bond->endAtom());

      if (hintRingPoints.size() < 2) return;

      // just translate ring to make 1 atom align
      QPointF bondNormal = normalized(bond->endAtom()->pos() - bond->beginAtom()->pos());
      QPointF ringNormal = normalized( (hintRingPoints[0] + hintRingPoints[1]) / 2.0 );
      QPointF rp = hintRingPoints.first();

      qreal ang = angle(bondNormal, ringNormal) * 180.0 / M_PI;
      double angSign = angleSign(bondNormal, ringNormal);
      if (angSign > 0.0)
        ang = -ang;

      double triSign = triangleSign(bond->beginAtom()->scenePos(), bond->endAtom()->scenePos(), pos);

      if (triSign > 0.0) {
        hintMoleculeItems->setTransform(QTransform().rotate(ang+270.0).translate(-rp.x(), -rp.y()));
        hintMoleculeItems->setPos(bond->endAtom()->scenePos());
      } else {
        hintMoleculeItems->setTransform(QTransform().rotate(ang+90.0).translate(-rp.x(), -rp.y()));
        hintMoleculeItems->setPos(bond->beginAtom()->scenePos());
      }
    }
  };

  drawAction::drawAction(MolScene *scene)
    : genericAction(scene),
      d(new privateData(this))
  {
    d->dock = new QDockWidget(tr("Draw mode"), parentWidget()) ;
    d->dock->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum); // TODO minimize size
    d->dock->setWidget(new QWidget(d->dock));
    d->dock->widget()->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    QVBoxLayout *layout = new QVBoxLayout(d->dock->widget()) ;
    layout->setAlignment(Qt::AlignLeft | Qt::AlignTop) ;
    d->periodicTable = new periodicTableWidget(d->dock) ;
    d->bondType = new bondTypeWidget(d->dock) ;
    d->dock->widget()->layout()->addWidget(d->periodicTable);
    d->dock->widget()->layout()->addWidget(d->bondType);
    connect(d->periodicTable, SIGNAL(elementChanged(QString)),
            this, SLOT(refreshIcon()));
    connect(d->bondType, SIGNAL(bondTypeChanged(const bondTypeWidget::BondType&)),
            this, SLOT(refreshIcon()));
    connect(this, SIGNAL(toggled(bool)),
            this, SLOT(toggleVisibility(bool)));
    refreshIcon(); // TODO
    if (parentWidget())
      qobject_cast<QMainWindow*>(parentWidget())->addDockWidget(Qt::LeftDockWidgetArea, d->dock);
    d->dock->show();
  }

  drawAction::~drawAction()
  {
    delete d->dock ;
    delete d ;
  }

  void drawAction::mousePressEvent(QGraphicsSceneMouseEvent *event)
  {
    if (Qt::LeftButton != event->button()) return ;
    if (event->modifiers()) return ; // TODO change for "snap to grid"
    // TODO delete action

    // Get the position
    QPointF downPos = event->buttonDownScenePos(event->button());

    // TODO handle group abbreviations (Ts, Bn, Bz, tBu, iBu, nBu...) -> settings dialog, settings per file (to allow for different conventions)

    // TODO move to bond!!
//    Bond *clickedBond = scene()->bondAt(downPos);
    Atom *clickedAtom = scene()->atomAt(downPos);

    // Check for bond click
//    if (clickedBond && !clickedAtom) {
//      if (m_hintMoleculeItems)
//        return;

//      switch (m_bondType) {
//        case Bond::InPlane:
//          if (clickedBond->bondType() == Bond::InPlane)
//            undostack->push(new IncOrder(clickedBond, QObject::tr("Change Bond Order")));
//          else
//            undostack->push(new SetBondType(clickedBond, Bond::InPlane));
//          break;
//        case Bond::Wedge:
//          if (clickedBond->bondType() == Bond::Wedge)
//            undostack->push(new SetBondType(clickedBond, Bond::InvertedWedge, QObject::tr("Change Wedge Bond")));
//          else
//            undostack->push(new SetBondType(clickedBond, Bond::Wedge, QObject::tr("Change Wedge Bond")));
//          break;
//        case Bond::Hash:
//          if (clickedBond->bondType() == Bond::Hash)
//            undostack->push(new SetBondType(clickedBond, Bond::InvertedHash, QObject::tr("Change Hash Bond")));
//          else
//            undostack->push(new SetBondType(clickedBond, Bond::Hash, QObject::tr("Change Hash Bond")));
//          break;
//        case Bond::CisOrTrans:
//          undostack->push(new SetBondType(clickedBond, Bond::CisOrTrans, QObject::tr("Change Hash Bond")));
//          break;
//        case Bond::WedgeOrHash:
//          undostack->push(new SetBondType(clickedBond, Bond::WedgeOrHash, QObject::tr("Change Hash Bond")));
//          break;
//        default:
//          break;
//      };

//    return;

    // Show hinting
    if (!d->hintMoleculeItems) {
      // hint points
      Atom* hintAtom = scene()->atomAt(downPos);
      d->hintPointsGroup.setPos(hintAtom
                                ? hintAtom->scenePos()
                                : downPos);
      scene()->addItem(&(d->hintPointsGroup)); // TODO attn: scene takes ownership...

      // hint line
      d->hintLine.setLine(QLineF(downPos, event->scenePos()));
      scene()->addItem(&(d->hintLine)); // TODO attn: scene takes ownership...
      d->hintLine.setVisible(true);
    }

    if (!clickedAtom)
    {
      //
      // mousePress in empty space
      //
      if (d->hintMoleculeItems) {
        // insert the hinted molecule if it exists
        if (d->hintRingPoints.isEmpty())
        {
          Molecule* mol = new Molecule;
          mol->setPos(downPos);
          undoStack()->beginMacro(tr("Add Molecule"));
          undoStack()->push(new Commands::AddItem(mol, scene()));

          Q_CHECK_PTR(d->hintMolecule);
          foreach (Atom *hintAtom, d->hintMolecule->atoms())
            undoStack()->push(
                  new Commands::AddAtom(
                    new Atom(hintAtom->scenePos(),
                             hintAtom->element(),
                             d->autoAddHydrogen),
                    mol));
          undoStack()->endMacro();
        }
      }
      else
        scene()->addItem(new Atom(downPos,
                                  d->periodicTable->currentElement(),
                                  d->autoAddHydrogen)); // TODO how does this end up on the undoStack?
    }

  }

  void drawAction::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
  {
    // Determine the right action
    if (d->hintMoleculeItems) {
      d->hintMoleculeItems->show();
      d->hintMoleculeItems->setPos(event->scenePos());
      d->hintMoleculeItems->setTransform(QTransform()); // TODO get rid of transforms

      // Get the position
      QPointF downPos = event->scenePos();

      Atom *atom = scene()->atomAt(downPos);
      if (atom)
        d->alignRingWithAtom(atom);

      Bond *bond = scene()->bondAt(downPos);
      if (bond)
        d->alignRingWithBond(bond, downPos);
    }

    // Check hinting conditions
    if (!(event->buttons() & Qt::LeftButton)) return;
    if (!scene()->atomAt(event->buttonDownScenePos(Qt::LeftButton))) return;

    // Set hinting
    d->hintLine.setLine(QLineF(
                          d->nearestPoint(event->buttonDownScenePos(Qt::LeftButton)),
                          d->nearestPoint(event->scenePos())));
  }

  void drawAction::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
  { // TODO rewrite
    // Remove the hinting
    if (d->hintLine.scene())
      d->hintLine.scene()->removeItem(&(d->hintLine));
    if (d->hintPointsGroup.scene())
      d->hintPointsGroup.scene()->removeItem(&(d->hintPointsGroup));

    // Get position
    QPointF downPos = event->buttonDownScenePos(event->button());
    QPointF upPos = d->nearestPoint(event->scenePos());
    // Get pointer the undo stack
    QUndoStack *undostack = undoStack();

    // Check possible targets
    Atom* a1 = scene()->atomAt(downPos);
    Atom* a2 = scene()->atomAt(upPos);
    Bond* b = scene()->bondAt(downPos);
    Molecule* m1 = a1 ? a1->molecule() : 0;
    Molecule* m2 = a2 ? a2->molecule() : 0;

    // Add aligned hint molecule if applicable
    if (d->hintMoleculeItems) {
      d->insertRing(undostack, false); // TODO aromatic!!
      return;
    }

    if (a1 != a2)
      undostack->beginMacro("Draw");

    // Make sure both molecules are valid
    if (m1 && !m2 && a2) {
      m2 = m1;
      a2->setMolecule(m2);
      undostack->push(new Commands::AddAtom(a2, m2));
    } else if (m2 && !m1 && a1) {
      m1 = m2;
      a1->setMolecule(m1);
      undostack->push(new Commands::AddAtom(a1, m1));
    }

    if (!a1 && !b) return;
    if (b && !a1) return;

    // Check for atom release
    if (a2) {

      // Check for atom click
      if (a1 == a2) {
        if (a1->element() != d->periodicTable->currentElement()) {
          // Press + Release with different symbol -> Change Element
          undostack->push(new Commands::ChangeElement(a1, d->periodicTable->currentElement(), QObject::tr("Change Element")));
        } else {
          if (!m1) {
            m1 = new Molecule;
            //m_stack->beginMacro("Add Atom");
            undostack->push(new Commands::AddItem(m1, scene()));
            undostack->push(new Commands::AddAtom(a1,m1));
            undostack->endMacro();
          }
        }
        return;
      }


      // Check for merge
      if (m1 && m2 && (m1 != m2)) {
        undostack->push(new Commands::MergeMol(m1, m2, m1));
        a1 = m1->atomAt(a1->scenePos());
        a2 = m1->atomAt(a2->scenePos());
        m1->setFocus();
      }

      // Adding bond
      Bond* bond = new Bond(a1,a2);
      undostack->push(new Commands::AddBond(bond));
      for (int i = 0; i < d->bondType->bondOrder() - 1; i++)
        undostack->push(new Commands::IncOrder(bond));
      undostack->push(new Commands::SetBondType(bond, d->bondType->legacyType())); // TODO bond anpassen.

      // End adding macro
      undostack->endMacro();
      return;
    }

    // Else scene release
    //m_stack->beginMacro(tr("Add Atoms"));
    if (!m1) {
      m1 = new Molecule;
      undostack->push(new Commands::AddItem(m1,scene()));
      undostack->push(new Commands::AddAtom(a1,m1));
    }

    Atom* atom = new Atom(upPos,d->periodicTable->currentElement(),d->autoAddHydrogen);
    undostack->push(new Commands::AddAtom(atom,m1));
    Bond* bond = new Bond(a1,atom);
    undostack->push(new Commands::AddBond(bond));
    for (int i = 0; i < d->bondType->bondOrder() - 1; i++)
      undostack->push(new Commands::IncOrder(bond));
    undostack->push(new Commands::SetBondType(bond, d->bondType->legacyType()));

    undostack->endMacro();

    scene()->update();

  }

  void drawAction::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
  {
    QUndoStack *stack = undoStack();
    QPointF downPos = event->buttonDownScenePos(event->button());

    //clicking on an atom spawns a new bond from that atom
    if (scene()->atomAt (downPos)) {
      QPointF new_atom_pos = downPos;
      Atom *at1 = scene()->atomAt (downPos);
      switch (at1 ->numBonds()) {
        case 0:
        {
          qreal x = new_atom_pos.x ();
          new_atom_pos.setX (x + d->bondLength);
          break;
        }
        case 1:
        {
          Atom *at2 = at1 ->neighbours()[0];
          if (at2 ->neighbours() .size ()< 2) {
            QPointF v = downPos - at1 ->neighbours()[0] ->pos ();

            QPointF rotated_v (v.x()*0.5 - v.y()*sqrt(3)*0.5, v.x()*0.5*sqrt(3) + v.y () *0.5);
            qreal mod = sqrt (rotated_v.x()*rotated_v.x() + rotated_v.y()*rotated_v.y());
            rotated_v *= d->bondLength/mod;
            new_atom_pos = rotated_v + downPos;
          } else {
            Atom *at3 = at2 ->neighbours()[0];
            if (at3 == at1) at3 = at2 ->neighbours()[1];
            QPointF rotated_v = at2 ->pos () - at3 ->pos ();
            qreal mod = sqrt (rotated_v.x()*rotated_v.x() + rotated_v.y()*rotated_v.y());
            rotated_v *= d->bondLength/mod;

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
          v5 = QPointF (v5.x()/mod * d->bondLength, v5.y()/mod * d->bondLength);
          new_atom_pos = v5 + at1->pos();
        }
        break;
        default:
        break;
      }
      if (new_atom_pos != downPos) {
        stack->beginMacro("Add Bond");
        Atom* atom = new Atom(new_atom_pos,d->periodicTable->currentElement(),d->autoAddHydrogen);
        stack->push(new Commands::AddAtom(atom,at1 ->molecule()));
        Bond* bond = new Bond(at1,atom);
        stack->push(new Commands::AddBond(bond));
        for (int i = 0; i < d->bondType->bondOrder() - 1; i++) // TODO this is repetitive (see mouseReleaseEvent()) -> make function
          stack->push(new Commands::IncOrder(bond));

        stack->push(new Commands::SetBondType(bond, d->bondType->legacyType()));
        stack->endMacro();
      }
    }
  }

  void drawAction::leaveSceneEvent(QEvent *event)
  {
    Q_UNUSED(event)
    if (d->hintMoleculeItems)
      d->hintMoleculeItems->hide();
  }

  void drawAction::refreshIcon()
  {
    QIcon bondIcon(d->bondType->bondIcon()) ;
    QString element = d->periodicTable->currentElement();
    int height = 0, width = 0;
    foreach (const QSize& size, bondIcon.availableSizes())
    {
      if (size.height() > height)
      {
        height = size.height();
        width  = size.width();
      }
    }
    QFont atomFont(font()); // TODO measuring is not quite right
    atomFont.setPixelSize(height);
    QFontMetrics metrics(atomFont);
    QRectF atomRect(QFontMetrics(atomFont).boundingRect(element));
    QImage compoundIcon(width + atomRect.width(), qMax((qreal) height, atomRect.height()), QImage::Format_RGB888);
    compoundIcon.fill(parentWidget() ? parentWidget()->palette().window().color() : Qt::white);
    QPainter painter(&compoundIcon);
    bondIcon.paint(&painter,0,.5*(compoundIcon.height()-height),width,height);
    painter.setFont(atomFont);
//    painter.drawText(width,atomRect.height(),element);
    atomRect.moveTopLeft(QPointF(width,0));
    qDebug() << "AtomRect" << atomRect << atomRect.height()<< width << compoundIcon.size();
    painter.drawText(atomRect, element);
    setIcon(QPixmap::fromImage(compoundIcon));
  }

  void drawAction::toggleVisibility(bool visible)
  {
    if (visible) d->dock->show();
    else d->dock->hide();
  }

} // namespace Molsketch

