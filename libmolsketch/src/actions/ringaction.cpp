#include "ringaction.h"

#include <QPen>
#include <bond.h>
#include <cmath>
#include <commands.h>
#include "molscene.h"
#include "math2d.h"

namespace Molsketch {

  class ringAction::privateData
  {
  public:
    QGraphicsPolygonItem hintMoleculeItems;
    // TODO initialization
    QPolygonF hintRingPoints;
    Molecule* hintMolecule;
    qreal bondLength;
    ringAction *parent;
    bool autoAddHydrogen;

    privateData(ringAction* p) :
      hintMoleculeItems(0),
      hintMolecule(0),
      bondLength(Bond::defaultLength), // TODO here and in draw action: replace with direct access to defaultLength (possibly move to settings object)
      parent(p),
      autoAddHydrogen(false)
    {
      hintMoleculeItems.hide();
      hintMoleculeItems.setPen(QPen(Qt::lightGray));
    }

    void createHintRing(int size, bool aromatic) // TODO use real molecule
    {
      Q_UNUSED(aromatic) // TODO
      if (size < 3) return;

      hintRingPoints.clear();
      qreal radius = bondLength / (2*sin(M_PI/size));
      for (int i = 0; i < size ; ++i)
        hintRingPoints.append(QLineF::fromPolar(radius, 360.*i/size).p2());

      hintMoleculeItems.setPolygon(hintRingPoints);
      parent->scene()->addItem(&hintMoleculeItems); // TODO scene takes ownership
    }

    void insertRing(QUndoStack* undostack, bool aromatic)
    {
      undostack->beginMacro(tr("Add molecule"));
      Molecule *newMolecule = new Molecule();
      undostack->push(new Commands::AddItem(newMolecule, parent->scene()));

      // create/find atoms
      QList<Atom*> ringAtoms;
      foreach(const QPointF& hintPoint, hintRingPoints)
      {
        QPointF vertex(hintMoleculeItems.mapToScene(hintPoint));
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

    void alignRingWithAtom(Atom *atom) // TODO partial merge with alignRingWithBond()
    {

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
      hintMoleculeItems.setTransform(
            QTransform().rotate(ang+180.0).translate(-rp.x(), -rp.y()));
      if (atom->numBonds()) hintMoleculeItems.setPos(atom->scenePos());
    }

    void alignRingWithBond(Bond *bond, const QPointF &pos)
    {
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
        hintMoleculeItems.setTransform(QTransform().rotate(ang+270.0).translate(-rp.x(), -rp.y()));
        hintMoleculeItems.setPos(bond->endAtom()->scenePos());
      } else {
        hintMoleculeItems.setTransform(QTransform().rotate(ang+90.0).translate(-rp.x(), -rp.y()));
        hintMoleculeItems.setPos(bond->beginAtom()->scenePos());
      }
    }
  };

#define ADDRINGSUBACTION(NAME, SIZE) \
  c = (SIZE > 0 ? "C" : "a"); \
  newAction = new QAction(QIcon(":images/" + c + "1" + c.repeated(qAbs(SIZE) -1) + "1.png"), tr(NAME), this); \
  newAction->setData(SIZE); \
  addSubAction(newAction);

  ringAction::ringAction(MolScene *scene)
    : multiAction(scene),
      d(new privateData(this))
  {
    QAction *newAction = 0 ;
    QString c;
    ADDRINGSUBACTION("Cyclopropyl", 3);
    ADDRINGSUBACTION("Cyclobutyl", 4);
    ADDRINGSUBACTION("Cyclopentyl", 5);
    ADDRINGSUBACTION("Cyclohexyl", 6);
    ADDRINGSUBACTION("Cycloheptyl", 7);
    ADDRINGSUBACTION("Cyclooctyl", 8);
    ADDRINGSUBACTION("Cyclopentadienyl", -5);
    ADDRINGSUBACTION("Aryl group", -6);
    connect(this, SIGNAL(triggered()), this, SLOT(changeRing()));
    changeRing();
  }

  ringAction::~ringAction()
  {
    delete d->hintMolecule;
    delete d;
  }

  void ringAction::mousePressEvent(QGraphicsSceneMouseEvent *event)
  {
    if (Qt::LeftButton != event->button()) return ;
    if (event->modifiers()) return ; // TODO change for "snap to grid"
  }

  void ringAction::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
  {
    d->hintMoleculeItems.show();
    d->hintMoleculeItems.setPos(event->scenePos());
    d->hintMoleculeItems.setTransform(QTransform()); // TODO get rid of transforms

    // Get the position
    QPointF downPos = event->scenePos();

    Atom *atom = scene()->atomAt(downPos);
    if (atom)
      d->alignRingWithAtom(atom);

    Bond *bond = scene()->bondAt(downPos);
    if (bond)
      d->alignRingWithBond(bond, downPos);
  }

  void ringAction::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
  {
    Q_UNUSED(event)
    d->insertRing(undoStack(), activeSubAction()->data().toInt() < 0); // TODO merge???
  }

  void ringAction::leaveSceneEvent(QEvent *event)
  {
    Q_UNUSED(event)
    d->hintMoleculeItems.hide();
  }

  void ringAction::changeRing() // TODO virtual function in multiaction
  {
      d->createHintRing(qAbs(activeSubAction()->data().toInt()), false);
  }

} // namespace Molsketch

