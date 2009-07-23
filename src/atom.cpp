/***************************************************************************
 *   Copyright (C) 2007-2008 by Harm van Eersel                            *
 *   devsciurus@xs4all.nl                                            *
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

#define ATOM_SIZE 30

#include <QPainter>
#include <QDebug>

#include "atom.h"
#include "bond.h"
#include "molecule.h"

#include "element.h"
#include "molscene.h"

namespace Molsketch {

  //
  //     /    \      \   /      H
  //   HN      NH      N        N
  //     \    /        H      /   \
  //
  //   Left   Right   Down     Up
  //
  enum {
    Left,
    Right,
    Up,
    Down
  };

  Atom::Atom(const QPointF &position, const QString &element, bool implicitHydrogens, 
      QGraphicsItem* parent, QGraphicsScene* scene) : QGraphicsItem (parent,scene)
  {
    //pre: position is a valid position in scene coordinates

    // Setting initial parameters
    setPos(position);
    setZValue(3);
    //setFlag(QGraphicsItem::ItemIsMovable);
    //setFlag(QGraphicsItem::ItemIgnoresTransformations);

    MolScene *molScene = dynamic_cast<MolScene*>(scene); // @todo qobject_cast is faster
    if (molScene)
      setFlag(QGraphicsItem::ItemIsSelectable, molScene->editMode() == MolScene::MoveMode);

    // Enabling hovereffects
    setAcceptsHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton | Qt::MidButton);

    // Setting private fields
    m_elementSymbol = element;
    m_hidden = true;
    m_drawn = false;

    m_ring = 0;

    m_userCharge = 0; // The initial additional charge is zero
    m_userImplicitHydrogens =  0;
    enableImplicitHydrogens(implicitHydrogens);
  }


  //////////////////////////////////////////////////////////////////////////////
  // Inherited painting methods
  //////////////////////////////////////////////////////////////////////////////

  QRectF Atom::boundingRect() const
  {
    /* FIXME The implicit hydrogens don't seem te be included */
    if (m_shape.width() && m_shape.height())
      return m_shape;
    else
      return QRectF(-ATOM_SIZE/2, -ATOM_SIZE/2, ATOM_SIZE, ATOM_SIZE);

  }

  bool Atom::hasLabel() const
  {
    MolScene* molScene = dynamic_cast<MolScene*>(scene());
    Q_CHECK_PTR(molScene);

    if ((m_elementSymbol == "C") && !molScene->carbonVisible() && (numBonds() > 1) && ((charge() == 0) || !molScene->chargeVisible()))
      return false;

    return true;
  }

  int labelAlignment(Atom *atom)
  {
    // compute the sum of the bond vectors, this gives
    QPointF direction(0.0, 0.0);
    foreach (Atom *nbr, atom->neighbors())
      direction += atom->pos() - nbr->pos();

    int alignment = 0;
    if ((atom->numBonds() == 2) && (abs(direction.y()) > abs(direction.x()))) {
      if (direction.y() <= 0.0)
        alignment = Up;
      else
        alignment = Down;
    } else {
      if (direction.x() < 0.0)
        alignment = Left;
      else
        alignment = Right;
    }

    return alignment;
  }

  void Atom::drawAtomLabel(QPainter *painter, const QString &lbl, int alignment)
  {
    MolScene* molScene = dynamic_cast<MolScene*>(scene());

    painter->save();
    QFont symbolFont = molScene->atomSymbolFont();
    QFont subscriptFont = symbolFont;
    subscriptFont.setPointSize(0.75 * symbolFont.pointSize());
    QFontMetrics fmSymbol(symbolFont);
    QFontMetrics fmScript(subscriptFont);


    // compute the total width
    qreal totalWidth = 0.0;
    if ((alignment == Right) || (alignment == Left) || !lbl.contains("H")) {
      for (int i = 0; i < lbl.size(); ++i) {
        if (lbl[i].isDigit())
          totalWidth += fmScript.width(lbl[i]);
        else
          totalWidth += fmSymbol.width(lbl[i]);
      }
    } else {
      totalWidth = fmSymbol.width(lbl.left(lbl.indexOf("H")));
      qreal width = 0.0; 
      for (int i = lbl.indexOf("H"); i < lbl.size(); ++i) {
        if (lbl[i].isDigit())
          width += fmScript.width(lbl[i]);
        else
          width += fmSymbol.width(lbl[i]);
      }

      if (width > totalWidth)
        totalWidth = width; 
    }

    // debug
    /*
    painter->save();
    painter->setPen(Qt::red);
    painter->drawPoint(QPointF(0.0, 0.0));
    painter->restore();
    */

    QString str, subscript;
    // compute the horizontal starting position
    qreal xOffset, yOffset, yOffsetSubscript;
    switch (alignment) {
      case Right:
        xOffset = - 0.5 * fmSymbol.width(lbl.left(1));
        break;
      case Left:
        xOffset = 0.5 * fmSymbol.width(lbl.right(1)) - totalWidth;
        break;
      case Up:
      case Down:
        if (lbl.contains("H"))
          xOffset = - 0.5 * fmSymbol.width(lbl.left(lbl.indexOf("H")));
        else
          xOffset = - 0.5 * totalWidth;
        break;
      default:
        xOffset = - 0.5 * totalWidth;
        break;
    }
    // compute the vertical starting position
    yOffset = 0.5 * (fmSymbol.ascent() - fmSymbol.descent());
    yOffsetSubscript = yOffset + fmSymbol.descent();
    qreal xInitial = xOffset;

    // compute the shape
    if ((alignment == Right) || (alignment == Left) || !lbl.contains("H"))
      m_shape = QRectF(xOffset, yOffsetSubscript - fmSymbol.height(), totalWidth, fmSymbol.height());
    else {
      if (alignment == Down)
        m_shape = QRectF(xOffset, yOffsetSubscript - fmSymbol.height(), totalWidth, fmSymbol.ascent() + fmSymbol.height());
      else
        m_shape = QRectF(xOffset, yOffsetSubscript - fmSymbol.ascent() - fmSymbol.height(), totalWidth, fmSymbol.ascent() + fmSymbol.height());
    }

    for (int i = 0; i < lbl.size(); ++i) {
      if (lbl[i] == 'H') {
        if ((alignment == Up) || (alignment == Down))
          if (!str.isEmpty()) {
            // write the current string
            painter->setFont(symbolFont);
            painter->drawText(xOffset, yOffset, str);
            if (alignment == Down) {
              yOffset += fmSymbol.ascent()/* - fmSymbol.descent()*/;
              yOffsetSubscript += fmSymbol.ascent()/* - fmSymbol.descent()*/;
            } else {
              yOffset -= fmSymbol.ascent()/* + fmSymbol.descent()*/;
              yOffsetSubscript -= fmSymbol.ascent()/* + fmSymbol.descent()*/;
            }
            xOffset = xInitial;
            str.clear();
          }
      }

      if (lbl[i].isDigit()) {
        if (!str.isEmpty()) {
          // write the current string
          painter->setFont(symbolFont);
          painter->drawText(xOffset, yOffset, str);
          xOffset += fmSymbol.width(str);
          str.clear();
        }

        subscript += lbl.mid(i, 1);
      } else {
        if (!subscript.isEmpty()) {
          // write the current subscript
          painter->setFont(subscriptFont);
          painter->drawText(xOffset, yOffsetSubscript, subscript);
          xOffset += fmScript.width(subscript);
          subscript.clear();
        }

        str += lbl.mid(i, 1);
      }
    }
    if (!str.isEmpty()) {
      painter->setFont(symbolFont);
      painter->drawText(xOffset, yOffset, str);
    }
    if (!subscript.isEmpty()) {
      painter->setFont(subscriptFont);
      painter->drawText(xOffset, yOffsetSubscript, subscript);
    }

    painter->restore();
  }


  void Atom::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
  {
    Q_UNUSED(option)
    Q_UNUSED(widget)
    // Save the original painter state
    //painter->save();
    
/*    
    qDebug() << "Atom::paint()";
    qDebug() << "m_hidden" << m_hidden;
    qDebug() << "isSelected" << isSelected();
    qDebug() << "numBonds" << numBonds();
*/
    // Check the scene
    MolScene* molScene = dynamic_cast<MolScene*>(scene());
    Q_CHECK_PTR(molScene);

    // If element is m_hidden, don't draw the atoms
    // Always draw the atom when there are no bonds
    if (m_hidden && !isSelected() && numBonds()) {
        if (m_elementSymbol == "H" && !molScene->autoAddHydrogen() && (!molScene->chargeVisible())) {
            m_drawn = false;
            return;
        }
  //      qDebug() << "bondOrderSum = " << bondOrderSum();
    //    qDebug() << "# implicit H = " << numberOfImplicitHydrogens();
        //if (m_elementSymbol == "C" && !molScene->carbonVisible() && ((bondOrderSum() - numberOfImplicitHydrogens()) > 2 && charge() == 0 || !molScene->chargeVisible())) {
        if ((m_elementSymbol == "C") && !molScene->carbonVisible() && (numBonds() > 1) && ((charge() == 0) || !molScene->chargeVisible())) {
            m_drawn = false;
            return;
        }
    }

    m_drawn = true;

    int alignment = labelAlignment(this);
    bool leftAligned = false;
    switch (alignment) {
      case Left:
        leftAligned = true;
      default:
        break;
    }

    int hCount = numberOfImplicitHydrogens();

    QString lbl;
    if (hCount && leftAligned)
      lbl += "H";
    if ((hCount > 1) && leftAligned)
      lbl += QString::number(hCount);

    lbl += m_elementSymbol;
    
    if (hCount && !leftAligned)
      lbl += "H";
    if ((hCount > 1) && !leftAligned)
      lbl += QString::number(hCount);

    drawAtomLabel(painter, lbl, alignment);

    // Drawing background
    painter->save();
    painter->setPen(Qt::NoPen);
    if (this->isSelected()) 
      painter->setPen(Qt::blue);
    painter->drawRect(m_shape);
    painter->restore();

    /*
    // Draw charge
    if (molScene->chargeVisible()) {
        QString chargeId = chargeString();
        painter->setFont( QFont("",molScene->atomSymbolFont().pointSizeF()/1.5) );
        int superscriptOffset = fmSymbol.width("H");
        painter->drawText(m_shape.translated(superscriptOffset/1.5, -superscriptOffset/1.5), Qt::AlignTop|Qt::AlignRight, chargeId);
    }
    */

    // Draw unbound electrons
    if (0) /*molScene->chargeVisible()*/ {
        int unboundElectrons = 8;
        QList<QRectF> layoutList;

        // Loading different layouts
        layoutList << QRectF(-3,-10,2,2);
        layoutList << QRectF(3,-10,2,2);
        layoutList << QRectF(-3,10,2,2);
        layoutList << QRectF(3,10,2,2);
        layoutList << QRectF(10,-3,2,2);
        layoutList << QRectF(10,3,2,2);
        layoutList << QRectF(-10,-3,2,2);
        layoutList << QRectF(-10,3,2,2);

        painter->save();
        painter->setBrush(Qt::black);

        for (int i = 0; i < unboundElectrons; i++)
            painter->drawEllipse(layoutList[i]);

        painter->restore();
    }

  }

  QVariant Atom::itemChange(GraphicsItemChange change, const QVariant &value)
  {
    if (change == ItemPositionChange && parentItem()) {
//         setTransform(parentItem()->transform().transposed());
      parentItem()->update();
      dynamic_cast<Molecule*>(parentItem())->rebuild();

// 	    setGroup(dynamic_cast<Molecule*>(parentItem()));
    };
    if (change == ItemSelectedChange && molecule()) {
//       molecule()->setSm_elementSymbolected(isSm_elementSymbolected());
      molecule()->setFlag(ItemIsSelectable, isSelected());
    }
    
    return QGraphicsItem::itemChange(change, value);
  }


  //////////////////////////////////////////////////////////////////////////////
  // Event handlers
  //////////////////////////////////////////////////////////////////////////////

  void Atom::mousePressEvent( QGraphicsSceneMouseEvent* event )
  {
    // Execute default behavior
    QGraphicsItem::mousePressEvent( event );
  }

  void Atom::hoverEnterEvent( QGraphicsSceneHoverEvent * event )
  {
    m_hidden = false;
    // Execute default behavior
    QGraphicsItem::hoverEnterEvent( event );
  }

  void Atom::hoverLeaveEvent( QGraphicsSceneHoverEvent * event )
  {
    m_hidden = true;
    // Execute default behavior
    QGraphicsItem::hoverLeaveEvent( event );
  }







  // Commands

  void Atom::setElement(const QString &element)
  {
    m_elementSymbol = element;
    update();
  }

  void Atom::setNumberOfImplicitHydrogens(int number)
  {
    Q_ASSERT (number >= 0);

    m_implicitHydrogens = true;

//  int deltaNoIH = number - m_numberOfImplicitHydrogens;
//  int newNoB = m_numberOfBonds - deltaNoIH;
  //m_numberOfBonds = (newNoB < 0) ? 0 : newNoB;

  m_userImplicitHydrogens = number;
}


// Query methods

  int Atom::numBonds() const
  {
    return m_neighbors.size();
  }

  int Atom::bondOrderSum() const
  {
    Molecule *mol = molecule();
    if (!mol) 
      return 0;

    // count explicit bonds
    int sum = 0;
    foreach (Bond *bond, mol->bonds(this))
      sum += bond->bondOrder();

    // take implicit hydrogens into account 
    sum += numberOfImplicitHydrogens();

    return sum;
  }

	QString Atom::string () const {
		QString el = element ();
		int n = numberOfImplicitHydrogens();
		QString hs;
		QString num = "";
		if (n) {
			if (n > 1) num.setNum (n);
			hs = QString ("H") + num;
		}
		else hs = QString ("");
		QString q = chargeString();
		return el+hs+q;
	}
	
  int Atom::numberOfImplicitHydrogens() const
  {
    Molecule *mol = molecule();
    if (!mol) 
      return 0;

    // count explicit bonds
    int bosum = 0;
    foreach (Bond *bond, mol->bonds(this))
      bosum += bond->bondOrder();

    int n = Molsketch::expectedValence(Molsketch::symbol2number(m_elementSymbol)) - bosum + m_userImplicitHydrogens;
    
    return (n > 0) ? n : 0;
  }

  QString Atom::element() const
  {
    return m_elementSymbol;
  }

  int Atom::charge()  const
  {
    // non element atoms have no charge unless explicitly set (m_userCharge)
    int atomicNumber = symbol2number(m_elementSymbol);
    if (!atomicNumber)
      return m_userCharge;

    int bosum = bondOrderSum();

    if (m_elementSymbol == "H") {
      if (bosum == 0) {
        if (m_implicitHydrogens)
          return 0 + m_userCharge;
        else
          return 1 + m_userCharge;
      } else
        return 1 - bosum + m_userCharge;
    }
    /*
    qDebug() << "Atom::charge()";
    qDebug() << "    element = " << m_elementSymbol;
    qDebug() << "    numValenceElectrons = " << Molsketch::numValenceElectrons(Molsketch::symbol2number(m_elementSymbol));
    qDebug() << "    bondOrderSum() = " << bondOrderSum();
    qDebug() << "    charge = " << Molsketch::numValenceElectrons(Molsketch::symbol2number(m_elementSymbol)) 
      - bondOrderSum() - (8 - 2 * bondOrderSum()) + m_userCharge;
    */
    return Molsketch::numValenceElectrons(Molsketch::symbol2number(m_elementSymbol)) 
      - bondOrderSum() - (8 - 2 * bondOrderSum()) + m_userCharge;
  }

  void Atom::setCharge(int charge)
  {
  
  }

  QString Atom::chargeString() const
  {
    // Get the charge
    int c = charge();

    // Drawing text
    QString string;
    string.setNum(c);
    if (c < -1) // ..., "3-", "2-"
      string =  string.remove(0,1) + "-";
    if (c == -1) // "-"
      string = "-";
    if (c == 0) // ""
      string = "";
    if (c == 1) // "+"
      string = "+";
    if (c > 1) // "2+", "3+", ...
      string = string + "+";

    // Return the charge string
    return string;
  }

  qreal Atom::weight( ) const
  {
    return Molsketch::weightOfElement(Molsketch::symbol2number(m_elementSymbol)) + numberOfImplicitHydrogens() * Molsketch::weightOfElement(1);
  }

  Molecule * Atom::molecule() const
  {
    return dynamic_cast<Molecule*>(this->parentItem());
  }

  void Atom::setMolecule(Molecule *molecule)
  {
    setParentItem(static_cast<QGraphicsItem*>(molecule));
  }

  bool Atom::hasImplicitHydrogens() const
  {
    return m_implicitHydrogens;
  }

  bool Atom::isDrawn() const
  {
    return m_drawn;
  }

  bool Atom::isHidden() const
  {
    return m_hidden;
  }

  void Atom::enableImplicitHydrogens(bool enabled)
  {
    m_implicitHydrogens = enabled && (m_elementSymbol == "C" || m_elementSymbol == "N" || m_elementSymbol == "O");
  }

  void Atom::addNeighbor(Atom *atom)
  {
    Q_CHECK_PTR(atom);
    if (!m_neighbors.contains(atom))
      m_neighbors.append(atom);
  }

  void Atom::removeNeighbor(Atom *atom)
  {
    Q_CHECK_PTR(atom);
    m_neighbors.removeAll(atom);
  }

  const QList<Atom*>& Atom::neighbors() const
  {
    return m_neighbors;
  }

} // namespace
