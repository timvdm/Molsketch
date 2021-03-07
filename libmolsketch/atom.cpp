/***************************************************************************
 *   Copyright (C) 2007-2008 by Harm van Eersel                            *
 *   Copyright (C) 2009 Tim Vandermeersch                                  *
 *   Copyright (C) 2018 Hendrik Vennekate                                  *
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

#define ATOM_SIZE 5

#include "atom.h"

#include <QPainter>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>

#include "qtdeprecations.h"

#include "atompopup.h"
#include "bond.h"
#include "lonepair.h"
#include "molecule.h"
#include "radicalelectron.h"

#include "element.h"
#include "molscene.h"
#include "TextInputItem.h"
#include <iostream>
#include <QtMath>
#include "scenesettings.h"
#include "settingsitem.h"
#include <QDebug>

#define REQ_MOLECULE auto m_molecule = molecule(); if (!m_molecule) return

namespace Molsketch {
  //                                        //
  //     /    \      \   /      H           //
  //   HN      NH      N        N           //
  //     \    /        H      /   \         //
  //                                        //
  //   Left   Right   Down     Up           //
  //                                        //
  Alignment Atom::labelAlignment() const {
    switch (hydrogenAlignment) {
      case north: return Up;
      case south: return Down;
      case east: return Right;
      case west: return Left;
      case automatic: return autoLabelAlignment();
      default: return autoLabelAlignment();
    }
  }

  Alignment Atom::autoLabelAlignment() const {
    // compute the sum of the bond vectors, this gives
    QPointF direction(0.0, 0.0);
    foreach (Atom *nbr, this->neighbours())
      direction += this->pos() - nbr->pos();

    if ((this->numBonds() == 2) && (qAbs(direction.y()) > qAbs(direction.x())))
      return direction.y() <= 0.0 ? Up : Down;
    return direction.x() < -0.1 // hack to make almost vertical lines align Right
        ? Left : Right;
  }

  Atom::Atom(const QPointF &position, const QString &element, bool implicitHydrogens,
             QGraphicsItem* parent)
    : graphicsItem (parent)
  {
    initialize(position, element, implicitHydrogens);
  }

  Atom::Atom(const Atom &other)
    : graphicsItem (other)
  { // TODO unit test copy constructor
    initialize(other.scenePos(), other.element(), other.m_implicitHydrogens);
    m_newmanDiameter = other.m_newmanDiameter;
    m_userCharge = other.m_userCharge;
    m_userElectrons = other.m_userElectrons;
    m_userImplicitHydrogens = other.m_userImplicitHydrogens;
  }

  Atom::~Atom() {}

  qreal Atom::computeTotalWdith(const int& alignment,
                                const QString& lbl,
                                const QFontMetrics &fmSymbol,
                                const QFontMetrics &fmScript)
  {
    qreal totalWidth = 0;
    if ((alignment == Right) || (alignment == Left) || !lbl.contains("H")) {
      for (int i = 0; i < lbl.size(); ++i) {
        if (lbl[i].isDigit())
          totalWidth += fmScript.horizontalAdvance(lbl[i]);
        else
          totalWidth += fmSymbol.horizontalAdvance(lbl[i]);
      }
    } else {
      totalWidth = fmSymbol.horizontalAdvance(lbl.left(lbl.indexOf("H")));
      qreal width = 0.0;
      for (int i = lbl.indexOf("H"); i < lbl.size(); ++i) {
        if (lbl[i].isDigit())
          width += fmScript.horizontalAdvance(lbl[i]);
        else
          width += fmSymbol.horizontalAdvance(lbl[i]);
      }

      if (width > totalWidth)
        totalWidth = width;
    }
    return totalWidth;
  }

  qreal Atom::computeXOffset(int alignment, const QFontMetrics& fmSymbol, const QString& lbl, const qreal& totalWidth)
  {
    qreal xOffset;
    switch (alignment) {
      case Right:
        xOffset = - 0.5 * fmSymbol.horizontalAdvance(lbl.left(1));
        break;
      case Left:
        xOffset = 0.5 * fmSymbol.horizontalAdvance(lbl.right(1)) - totalWidth;
        break;
      case Up:
      case Down:
        if (lbl.contains("H") && !QRegExp("H[0-9]*").exactMatch(lbl))
          xOffset = - 0.5 * fmSymbol.horizontalAdvance(lbl.left(lbl.indexOf("H")));
        else
          xOffset = - 0.5 * totalWidth;
        break;
      default:
        xOffset = - 0.5 * totalWidth;
        break;
    }
    return xOffset;
  }

  QPair<QFont, QFont> Atom::getFonts() const
  {
    QFont symbolFont = getSymbolFont();
    return qMakePair(symbolFont, getSubscriptFont(symbolFont));
  }

  QFont Atom::getSymbolFont() const
  {
    QFont symbolFont;
    MolScene *sc = qobject_cast<MolScene*>(scene());
    if (sc) symbolFont = sc->settings()->atomFont()->get();
    if (symbolFont.pointSizeF() > 0)
      symbolFont.setPointSizeF(symbolFont.pointSizeF() * relativeWidth());
    return symbolFont;
  }

  QFont Atom::getSubscriptFont(const QFont& symbolFont) const
  {
    QFont subscriptFont = symbolFont;
    if (symbolFont.pointSizeF() > 0)
      subscriptFont.setPointSizeF(0.75 * symbolFont.pointSizeF());
    return subscriptFont;
  }

  QString Atom::composeLabel(bool leftAligned) const
  {
    int hCount = numImplicitHydrogens();
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
    return lbl;
  }

  QRectF Atom::computeBoundingRect()
  {
    if (m_newmanDiameter > 0.) return QRectF(-m_newmanDiameter/2., -m_newmanDiameter/2., m_newmanDiameter, m_newmanDiameter);

    if (Circle == m_shapeType) {
      auto radius = diameterForCircularShape()/2.;
      return QRectF(-radius, -radius, radius, radius);
    }
    // TODO do proper prepareGeometryChange() call
    // TODO call whenever boundingRect() is called
    Alignment alignment = labelAlignment();

    QString lbl = composeLabel(Left == alignment);

    QPair<QFont, QFont> fonts = getFonts();
    if (fonts.first.pointSizeF() < 0) return QRectF();
    QFontMetrics fmSymbol(fonts.first), fmScript(fonts.second);

    qreal totalWidth = computeTotalWdith(alignment, lbl, fmSymbol, fmScript);
    qreal xOffset = computeXOffset(alignment, fmSymbol, lbl, totalWidth);
    qreal yOffset = 0.5 * (fmSymbol.ascent() - fmSymbol.descent());
    qreal yOffsetSubscript = yOffset + fmSymbol.descent();

    // compute the shape
    if ((alignment == Right) || (alignment == Left) || !lbl.contains("H") || QRegExp("H[0-9]*").exactMatch(lbl))
      return QRectF(xOffset, yOffsetSubscript - fmSymbol.height(), totalWidth, fmSymbol.height());
    if (alignment == Down)
      return QRectF(xOffset, yOffsetSubscript - fmSymbol.height(), totalWidth, fmSymbol.ascent() + fmSymbol.height());
    return QRectF(xOffset, yOffsetSubscript - fmSymbol.ascent() - fmSymbol.height(), totalWidth, fmSymbol.ascent() + fmSymbol.height());
  }

  void Atom::initialize(const QPointF &position,
                        const QString &element,
                        bool implicitHydrogens)
  {
    hydrogenAlignment = automatic;
    setPos(position);
    setZValue(3);
    setShapeType(Rectangle);

    MolScene *molScene = qobject_cast<MolScene*>(scene());

    if (molScene) {
      setColor (molScene->settings()->defaultColor()->get());
    }
    else setColor (QColor (0, 0, 0));
    // Enabling hovereffects
    setAcceptHoverEvents(true) ;

    // Setting private fields
    m_elementSymbol = element;

    m_userCharge = 0; // The initial additional charge is zero
    m_userElectrons = 0;
    m_userImplicitHydrogens =  0;
    m_newmanDiameter = 0;
    m_implicitHydrogens = implicitHydrogens;
    updateShape();
  }

  QRectF Atom::boundingRect() const
  {
    if (!isDrawn() || m_elementSymbol.isEmpty()) {
      auto centeringDistance = QPointF(pointSelectionDistance(), pointSelectionDistance());
      return QRectF(-centeringDistance, centeringDistance);
    }
    return m_shape;
  }

  void Atom::drawAtomLabel(QPainter *painter, const QString &lbl, int alignment)
  {
    painter->save(); // TODO unite with computeBoundingRect
    QFont symbolFont = getSymbolFont();
    QFont subscriptFont = symbolFont;
    subscriptFont.setPointSize(0.75 * symbolFont.pointSize());
    QFontMetrics fmSymbol(symbolFont);
    QFontMetrics fmScript(subscriptFont);


    // compute the total width
    qreal totalWidth = 0.0;
    if ((alignment == Right) || (alignment == Left) || !lbl.contains("H")) {
      for (int i = 0; i < lbl.size(); ++i) {
        if (lbl[i].isDigit())
          totalWidth += fmScript.horizontalAdvance(lbl[i]);
        else
          totalWidth += fmSymbol.horizontalAdvance(lbl[i]);
      }
    } else {
      totalWidth = fmSymbol.horizontalAdvance(lbl.left(lbl.indexOf("H")));
      qreal width = 0.0;
      for (int i = lbl.indexOf("H"); i < lbl.size(); ++i) {
        if (lbl[i].isDigit())
          width += fmScript.horizontalAdvance(lbl[i]);
        else
          width += fmSymbol.horizontalAdvance(lbl[i]);
      }

      if (width > totalWidth)
        totalWidth = width;
    }

    QString str, subscript;
    // compute the horizontal starting position
    qreal xOffset, yOffset, yOffsetSubscript;
    switch (alignment) {
      case Right:
        xOffset = - 0.5 * fmSymbol.horizontalAdvance(lbl.left(1));
        break;
      case Left:
        xOffset = 0.5 * fmSymbol.horizontalAdvance(lbl.right(1)) - totalWidth;
        break;
      case Up:
      case Down:
        if (lbl.contains("H") && !QRegExp("H[0-9]*").exactMatch(lbl))
          xOffset = - 0.5 * fmSymbol.horizontalAdvance(lbl.left(lbl.indexOf("H")));
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
          xOffset += fmSymbol.horizontalAdvance(str);
          str.clear();
        }

        subscript += lbl.mid(i, 1);
      } else {
        if (!subscript.isEmpty()) {
          // write the current subscript
          painter->setFont(subscriptFont);
          painter->drawText(xOffset, yOffsetSubscript, subscript);
          xOffset += fmScript.horizontalAdvance(subscript);
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


  void Atom::drawElectrons(QPainter* painter)
  {
    int unboundElectrons = numNonBondingElectrons();
    QList<QRectF> layoutList;

    auto m_bonds = bonds();
    if (m_bonds.empty()) {
      //  ..   ..   ..     ..
      //  NH2  OH2  CH3    N .
      //       ''          ''
      layoutList << QRectF(-3,-10,2,2); // top1
      layoutList << QRectF(3,-10,2,2); // top2
      layoutList << QRectF(-3,10,2,2); // bottom1
      layoutList << QRectF(3,10,2,2); // bottom2
      layoutList << QRectF(-10,-3,2,2); // left1
      layoutList << QRectF(-10,3,2,2); // left2
      layoutList << QRectF(10,-3,2,2); // right1
      layoutList << QRectF(10,3,2,2); // right2
    } else if (m_bonds.size() == 1) {
      //   ..   ..     ..    |
      // --OH   HO--  :OH   :OH
      //   ''   ''     |     ''
      QPointF direction(0.0, 0.0);
      foreach (Atom *nbr, neighbours())
        direction += pos() - nbr->pos();

      if (qAbs(direction.y()) > qAbs(direction.x())) {
        if (direction.y() <= 0.0) {
          //   ..
          //   NH2
          //   |
          layoutList << QRectF(-3,-10,2,2); // top1
          layoutList << QRectF(3,-10,2,2); // top2
          if (direction.x() < -0.1) {
            layoutList << QRectF(10,-3,2,2); // right1
            layoutList << QRectF(10,3,2,2); // right2
            layoutList << QRectF(-10,-3,2,2); // left1
            layoutList << QRectF(-10,3,2,2); // left2
          } else {
            layoutList << QRectF(-10,-3,2,2); // left1
            layoutList << QRectF(-10,3,2,2); // left2
            layoutList << QRectF(10,-3,2,2); // right1
            layoutList << QRectF(10,3,2,2); // right2
          }
          layoutList << QRectF(-3,10,2,2); // bottom1
          layoutList << QRectF(3,10,2,2); // bottom2
        } else {
          layoutList << QRectF(-3,10,2,2); // bottom1
          layoutList << QRectF(3,10,2,2); // bottom2
          if (direction.x() < -0.1) {
            layoutList << QRectF(10,-3,2,2); // right1
            layoutList << QRectF(10,3,2,2); // right2
            layoutList << QRectF(-10,-3,2,2); // left1
            layoutList << QRectF(-10,3,2,2); // left2
          } else {
            layoutList << QRectF(-10,-3,2,2); // left1
            layoutList << QRectF(-10,3,2,2); // left2
            layoutList << QRectF(10,-3,2,2); // right1
            layoutList << QRectF(10,3,2,2); // right2
          }
          layoutList << QRectF(-3,-10,2,2); // top1
          layoutList << QRectF(3,-10,2,2); // top2
        }
      } else {
        if (direction.x() < 0.0) {
          layoutList << QRectF(-3,-10,2,2); // top1
          layoutList << QRectF(3,-10,2,2); // top2
          layoutList << QRectF(-3,10,2,2); // bottom1
          layoutList << QRectF(3,10,2,2); // bottom2
          layoutList << QRectF(-10,-3,2,2); // left1
          layoutList << QRectF(-10,3,2,2); // left2
          layoutList << QRectF(10,-3,2,2); // right1
          layoutList << QRectF(10,3,2,2); // right2

        } else {
          layoutList << QRectF(-3,-10,2,2); // top1
          layoutList << QRectF(3,-10,2,2); // top2
          layoutList << QRectF(-3,10,2,2); // bottom1
          layoutList << QRectF(3,10,2,2); // bottom2
          layoutList << QRectF(10,-3,2,2); // right1
          layoutList << QRectF(10,3,2,2); // right2
          layoutList << QRectF(-10,-3,2,2); // left1
          layoutList << QRectF(-10,3,2,2); // left2
        }
      }

    }

    if (layoutList.isEmpty()) {
      // Loading different layouts
      layoutList << QRectF(-3,-10,2,2); // bottom1
      layoutList << QRectF(3,-10,2,2); // bottom2
      layoutList << QRectF(-3,10,2,2); // top1
      layoutList << QRectF(3,10,2,2); // top2
      layoutList << QRectF(10,-3,2,2); // right1
      layoutList << QRectF(10,3,2,2); // right2
      layoutList << QRectF(-10,-3,2,2); // left1
      layoutList << QRectF(-10,3,2,2); // left2
    }


    painter->save();

    for (int i = 0; i < unboundElectrons; i++)
      painter->drawEllipse(layoutList[i]);

    painter->restore();
  }

  void Atom::drawCharge(QPainter* painter)
  {
    QString chargeId = chargeString();
    QFont superscriptFont = getSymbolFont();
    superscriptFont.setPointSize(0.75 * superscriptFont.pointSize());
    QFontMetrics fmSymbol(superscriptFont);
    int offset = 0.5 * fmSymbol.horizontalAdvance("+");
    painter->save();
    painter->setFont(superscriptFont);
    painter->drawText(m_shape.right() - offset, m_shape.top() + offset, chargeId);
    painter->restore();
  }

  void Atom::renderColoredSquare(QPainter* painter) {
    renderColoredShape(painter, &QPainter::drawRect);
  }

  void Atom::renderColoredCircle(QPainter* painter) {
    renderColoredShape(painter, &QPainter::drawEllipse);
  }

  void Atom::renderColoredShape(QPainter* painter, void (QPainter::*drawMethod)(int,int,int,int)) {
    int element = Element::strings.indexOf(m_elementSymbol);
    if (element != Element::C) {
      QColor color = elementColor(element);
      painter->save();
      painter->setPen(color);
      painter->setBrush(color);
      qreal half = 10.0;
      (painter->*drawMethod)(-half, -half, 2.0 * half, 2.0 * half);
      painter->restore();
    }
  }

  qreal Atom::diameterForCircularShape() const {
    auto bounds = boundingRect();
    return QLineF(bounds.center(), bounds.topRight()).length() * 2;
  }

  void Atom::drawSelectionHighlight(QPainter* painter)
  {
    if (this->isSelected()) {
      painter->save();
      painter->setPen(Qt::blue);
      painter->drawRect(m_shape); // TODO draw rectangle around this and children rectangle to include electrons (possibly move this method to graphicsItem)
      painter->restore();
    }
  }

  QString Atom::getLabelWithHydrogens()
  {
    bool leftAligned = false;
    switch (labelAlignment()) {
      case Left:
        leftAligned = true;
      default:
        break;
    }

    int hCount = numImplicitHydrogens();

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

    return lbl;
  }

  void Atom::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
  {
    Q_UNUSED(option)
    Q_UNUSED(widget)


    MolScene* molScene = dynamic_cast<MolScene*>(scene());
    if (!molScene) return ;
    Q_CHECK_PTR(molScene);

    switch (molScene->renderMode()) { // TODO this could be better handled by using an atom renderer from the scene
      case MolScene::RenderColoredSquares:
        renderColoredSquare(painter);
        return;
      case MolScene::RenderColoredCircles:
        renderColoredCircle(painter);
        return;
      case MolScene::RenderColoredWireframe:
        return;
      default:
      case MolScene::RenderLabels:
        break;
    }

    if (m_newmanDiameter > 0) {
      drawNewman(painter);
      graphicsItem::paint(painter, option, widget);
      return;
    }
    if (!isDrawn()) return;
    painter->save();
    painter->setPen(getColor());

    drawAtomLabel(painter, getLabelWithHydrogens(), labelAlignment());
    drawSelectionHighlight(painter);
    if (molScene->settings()->chargeVisible()->get() && !m_elementSymbol.isEmpty())
      drawCharge(painter); // TODO unite with subscript drawing and align appropriately
    if (molScene->settings()->lonePairsVisible()->get()) drawElectrons(painter);
    painter->restore();

    graphicsItem::paint(painter, option, widget);
  }

  void Atom::drawNewman(QPainter* painter) {
    painter->save();
    QPen pen = painter->pen();
    pen.setColor(getColor());
    pen.setWidthF(lineWidth());
    painter->setPen(pen);
    qreal half = m_newmanDiameter/2.;
    painter->drawEllipse(mapFromScene(pos()), half, half);
    painter->restore();
  }

  qreal Atom::annotationDirection() const
  {
    // Determine optimum direction if angleDirection negative
    auto m_bonds = bonds();
    //   No preference & no bonds => downward
    if (m_bonds.isEmpty())
      return 270 ;
    if (m_bonds.size() == 1)
      return Molecule::toDegrees(m_bonds.first()->bondAngle(this)+180.) ;
    //   Have bonds? determine largest free angle
    QVector<qreal> angles ;
    foreach (Bond *bond, m_bonds)
      angles << bond->bondAngle(this) ;
    std::sort(angles.begin(), angles.end()) ;
    angles << angles.first() + 360. ;
    qreal maxAngleGap = -1, result = 270 ;
    for (int i = 0 ; i < angles.size()-1 ; ++i)
    {
      qreal gap = angles[i+1] - angles[i] ;
      if (gap > maxAngleGap)
      {
        maxAngleGap = gap ;
        result = (angles[i+1]+angles[i]) / 2. ;
      }
    }
    return Molecule::toDegrees(result) ;
  }

  QVariant Atom::itemChange(GraphicsItemChange change, const QVariant &value)
  {
    if (change == ItemPositionChange && parentItem()) {
      parentItem()->update();
      dynamic_cast<Molecule*>(parentItem())->rebuild();
    };
    updateShape();
    return graphicsItem::itemChange(change, value);
  }

  void Atom::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
  {
    if (event->button() != Qt::LeftButton) return;
    MolScene* sc = dynamic_cast<MolScene*>(scene());
    if(!sc || !sc->inputItem()) return;
    event->accept();
    TextInputItem *inputItem = sc->inputItem();
    sc->addItem(inputItem);
    inputItem->clickedOn(this);
  }

  const char *ID_ATTRIBUTE = "id";
  const char *ELEMENT_ATTRIBUTE = "elementType";
  const char *CHARGE_ATTRIBUTE = "userCharge";
  const char *DISABLE_HYDROGENS_ATTRIBUTE = "disableHydrogens";
  const char *HYDROGEN_COUNT_ATTRIBUTE = "hydrogens";
  const char *NEWMAN_DIAMETER_ATTRIBUTE = "newmanDiameter";
  const char *SHAPE_TYPE_ATTRIBUTE = "shapeType";
  const char *HYDROGEN_ALIGNMENT = "hydrogenAlignment";

  void Atom::readGraphicAttributes(const QXmlStreamAttributes &attributes)
  {
    m_elementSymbol = attributes.value(ELEMENT_ATTRIBUTE).toString();
    m_index = attributes.value(ID_ATTRIBUTE).toString();
    m_newmanDiameter = qAbs(attributes.value(NEWMAN_DIAMETER_ATTRIBUTE).toDouble());
    m_userImplicitHydrogens = attributes.value(HYDROGEN_COUNT_ATTRIBUTE).toInt();
    m_implicitHydrogens = !attributes.value(DISABLE_HYDROGENS_ATTRIBUTE).toInt();
    m_userCharge = attributes.value(CHARGE_ATTRIBUTE).toInt();
    m_shapeType = (ShapeType) attributes.value(SHAPE_TYPE_ATTRIBUTE).toInt();
    auto hAlignment = attributes.value(HYDROGEN_ALIGNMENT).toInt();
    hydrogenAlignment = (NeighborAlignment) hAlignment;
    updateShape();
  }

  QXmlStreamAttributes Atom::graphicAttributes() const
  {
    QXmlStreamAttributes attributes ;
    attributes.append(ID_ATTRIBUTE, m_index) ;
    attributes.append(ELEMENT_ATTRIBUTE, m_elementSymbol) ;
    attributes.append(CHARGE_ATTRIBUTE, QString::number(m_userCharge));
    attributes.append(DISABLE_HYDROGENS_ATTRIBUTE, QString::number(!m_implicitHydrogens));
    attributes.append(HYDROGEN_COUNT_ATTRIBUTE, QString::number(m_userImplicitHydrogens));
    attributes.append(SHAPE_TYPE_ATTRIBUTE, QString::number(m_shapeType));
    if (m_newmanDiameter > 0) attributes.append(NEWMAN_DIAMETER_ATTRIBUTE, QString::number(m_newmanDiameter));
    attributes.append(HYDROGEN_ALIGNMENT, QString::number(hydrogenAlignment));
    return attributes ;
  }

  void Atom::setElement(const QString &element)
  {
    m_elementSymbol = element;
    updateShape();
    if (Molecule *m = molecule()) {
      m->invalidateElectronSystems();
      m->updateTooltip();
    }
  }

  Atom::ShapeType Atom::shapeType() const {
    return m_shapeType;
  }

  void Atom::setShapeType(const Atom::ShapeType &shapeType) {
    m_shapeType = shapeType;
    updateShape();
  }

  void Atom::setNewmanDiameter(const qreal &diameter) {
    m_newmanDiameter = diameter;
    updateShape();
  }

  qreal Atom::getNewmanDiameter() const {
    return m_newmanDiameter;
  }

  void Atom::setHAlignment(const NeighborAlignment &hAlignment) {
    hydrogenAlignment = hAlignment;
    updateShape();
  }

  NeighborAlignment Atom::hAlignment() const {
    return hydrogenAlignment;
  }

  void Atom::disableNewman() {
    m_newmanDiameter = 0;
  }

  SumFormula Atom::sumFormula() const {
    int hydrogenCount = numImplicitHydrogens();
    SumFormula result = SumFormula::fromString(m_elementSymbol);
    if (hydrogenCount) result += SumFormula{"H", hydrogenCount, charge()};
    return  result;
  }

  void Atom::setNumImplicitHydrogens(const int& number)
  {
    m_implicitHydrogens = true;

    m_userImplicitHydrogens = 0;
    int deltaH = number - numImplicitHydrogens();

    m_userImplicitHydrogens = deltaH;
    if (auto m = molecule()) m->updateTooltip();
  }

  int Atom::numBonds() const {
    return bonds().size();
  }

  int Atom::bondOrderSum() const
  {
    int sum = numImplicitHydrogens();
    foreach (Bond *bond, bonds())
      sum += bond->bondOrder();
    return sum;
  }

  int Atom::numNonBondingElectrons() const
  {
    int boSum = bondOrderSum();
    int group =  elementGroup(Element::strings.indexOf(m_elementSymbol));

    // TODO check again and write test
    if (group >= 3 && group <= 11)
      return group - boSum + m_userElectrons;

    switch (group) {
      case 1:
      case 2:
      case 13:
      case 14:
        return 0 + m_userElectrons;
      case 15:
        if (boSum > 3) return 0 + m_userElectrons;
        else return 5 - boSum + m_userElectrons;
      case 16:
        switch (boSum) {
          case 0: return 6 + m_userElectrons;
          case 1: return 5 + m_userElectrons;
          case 2: return 4 + m_userElectrons;
          case 3: return 2 + m_userElectrons; // hm?
          default: return 0 + m_userElectrons;
        }
      case 17:
        if (boSum == 1) return 6 + m_userElectrons;
        else return 8 + m_userElectrons;
      case 18:
        return 8 + m_userElectrons;
      default:
        return 0 + m_userElectrons;
    }
  }

  int Atom::numImplicitHydrogens() const {
    if (!m_implicitHydrogens) return 0;
    int bosum = 0;
    foreach (Bond *bond, bonds())
      bosum += bond->bondOrder();
    int n = expectedValence(Element::strings.indexOf(m_elementSymbol)) - bosum + m_userImplicitHydrogens;
    return qMax(n, 0);
  }

  QString Atom::element() const
  {
    return m_elementSymbol;
  }

  int Atom::charge()  const
  {
    // non element atoms have no charge unless explicitly set (m_userCharge)
    int atomicNumber = Element::strings.indexOf(m_elementSymbol);
    if (!atomicNumber)
      return m_userCharge;

    // special case: He uses duet rule (<-> octet rule)
    if (atomicNumber == Element::He)
      return m_userCharge;

    return Molsketch::numValenceElectrons(atomicNumber) - bondOrderSum() - numNonBondingElectrons() + m_userCharge;
  }

  void Atom::setCharge(const int& requiredCharge)
  {
    int computedCharge = charge() - m_userCharge;
    m_userCharge = requiredCharge - computedCharge;
    if (auto m = molecule()) m->updateTooltip();
  }

  QString Atom::chargeString() const
  {
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

    return string;
  }

  Molecule * Atom::molecule() const
  {
    return dynamic_cast<Molecule*>(this->parentItem());
  }

  void Atom::setMolecule(Molecule *molecule)
  {
    setParentItem(static_cast<QGraphicsItem*>(molecule));
  }

  bool Atom::isDrawn() const
  {
    if (isHovering() || isSelected() || !numBonds()) return true;
    bool carbonVisible = false;
    bool chargeVisible = true;
    bool terminalMethylShown = true;
    MolScene* molScene = dynamic_cast<MolScene*>(scene());
    if (molScene)
    {
      carbonVisible = molScene->settings()->carbonVisible()->get();
      chargeVisible = molScene->settings()->chargeVisible()->get();
      terminalMethylShown = molScene->settings()->showTerminalMethyls()->get();
    }

    if ((m_elementSymbol == "C")
        && !carbonVisible
        && (numBonds() > 1 || (numBonds() == 1 && !terminalMethylShown))
        && ((charge() == 0) || !chargeVisible)
        && childItems().empty()
        && 0 == m_newmanDiameter)
      return false;
    return true;
  }

  void Atom::setCoordinates(const QVector<QPointF> &c)
  {
    if (c.size() != 1) return ;
    setPos(c.first());
  }

  QPolygonF Atom::coordinates() const
  {
    return QVector<QPointF>() << pos() ; // TODO change to coordinates relative to parent
  }

  QString Atom::xmlName() const { return Atom::xmlClassName() ; }

  QString Atom::xmlClassName() { return "atom" ; }

  QList<Bond*> Atom::bonds() const {
    REQ_MOLECULE {};
    return m_molecule->bonds(this);
  }

  Bond* Atom::bondTo(Atom* other) const {
    REQ_MOLECULE nullptr;
    return m_molecule->bondBetween(this, other);
  }

  QWidget *Atom::getPropertiesWidget()
  {
    AtomPopup* widget = new AtomPopup;
    widget->connectAtom(this);
    return widget;
  }

  QPointF Atom::bondDrawingStart(const Atom *other, qreal bondLineWidth) const
  {
    if (!isDrawn()) return pos();

    QLineF connection(pos(), other->pos());

    if (m_newmanDiameter > 0) {
      connection.setLength((m_newmanDiameter + qMax(lineWidth(), bondLineWidth))/2. );
      return connection.p2();
    }

    if (Circle == m_shapeType) {
      auto bounds = boundingRect();
      auto circleDiameter = QLineF(bounds.center(), bounds.topRight()).length() * 2;
      connection.setLength((circleDiameter + qMax(lineWidth(), bondLineWidth))/2. );
      return connection.p2();
    }


    return getBondDrawingStartFromBoundingBox(connection, bondLineWidth/1.5); // TODO: why 1.5?
  }

  class Atom::IntersectionData {
    const QPointF intersection;
    const QLineF edge;
  public:
    IntersectionData(const QPointF &intersection, const QLineF &edge)
      : intersection(intersection), edge(edge) {}
    QPointF getIntersectionPoint() const {
      return intersection;
    }
    QLineF getEdge() const {
      return edge;
    }
  };

  Atom::IntersectionData Atom::intersectedEdge(const QLineF& line, qreal lineWidth) const { // Cave: atom has to be bond start
    QRectF bounds = boundingRect().adjusted(-.5*lineWidth, -.5*lineWidth, .5*lineWidth, .5*lineWidth);
    QPointF intersection;

    QLineF topEdge{bounds.topLeft(), bounds.topRight()};
    if (intersectionType(topEdge, line, &intersection) == QLineF::BoundedIntersection)
      return IntersectionData(intersection, topEdge);

    QLineF bottomEdge{bounds.bottomLeft(), bounds.bottomRight()};
    if (intersectionType(bottomEdge, line, &intersection) == QLineF::BoundedIntersection)
      return IntersectionData(intersection, bottomEdge);

    QLineF leftEdge{bounds.topLeft(), bounds.bottomLeft()};
    if (intersectionType(leftEdge, line, &intersection) == QLineF::BoundedIntersection)
      return IntersectionData(intersection, leftEdge);

    QLineF rightEdge{bounds.topRight(), bounds.bottomRight()};
    if (intersectionType(rightEdge, line, &intersection) == QLineF::BoundedIntersection)
      return IntersectionData(intersection, rightEdge);
    // TODO pick the edge it intersects with first (i.e. closest to the middle)
    return IntersectionData(QPointF(), QLineF());
  }

  QPointF closestPointTo(const QPointF& position, const QList<QPointF>& points) {
    qreal shortestDistance = INFINITY;
    QPointF closestPoint;
    for (auto point : points) {
      qreal distance = QLineF(position, point).length();
      if (distance < shortestDistance) {
        shortestDistance = distance;
        closestPoint = point;
      }
    }
    return closestPoint;
  }


  qreal getBondExtentForNewmanAtom(const QLineF &middleLine, qreal lineWidth, qreal newmanDiameter) {
    qreal f = pow(middleLine.dx(), 2) + pow(middleLine.dy(), 2);
    qreal p = 2*(middleLine.p1().x() * middleLine.dx() + middleLine.p1().y() * middleLine.dy()) / f / 2.;
    qreal q = (pow(middleLine.p1().x(), 2) + pow(middleLine.p1().y(), 2) - pow((newmanDiameter + lineWidth)/2., 2))/f;
    return qMax(-p + sqrt(p*p - q), -p - sqrt(p*p - q));
  }

  qreal Atom::getExtentForEndOnCorner(const QPolygonF &fullBondPolygon, const QLineF& middleLine, const QPointF& corner) const {
    if (!fullBondPolygon.containsPoint(corner, Qt::OddEvenFill)) return 0;
    QLineF unitVector{middleLine.unitVector()};
    QLineF toCorner{middleLine.p1(), corner};
    qreal extent = toCorner.dx() * unitVector.dx() + toCorner.dy() * unitVector.dy();
    return extent / middleLine.length();
  }

  qreal Atom::getExtentForIntersectionOfOuterLineAndEdge(const IntersectionData &edgeIntersection, const QLineF &outer) const {
    QPointF intersectionOfOuterAndEdge;
    QLineF::IntersectType intersectType = intersectionType( edgeIntersection.getEdge(), outer, &intersectionOfOuterAndEdge);
    return QLineF::BoundedIntersection == intersectType
        ? QLineF(intersectionOfOuterAndEdge, outer.p1()).length() / outer.length()
        : 0;
  }

  qreal Atom::getBondExtent(const QLineF& outer1, const QLineF& outer2, qreal lineWidth) const { // Cave: atom has to be bond start
    if (!isDrawn()) return 0;

    // Assumption: p1 is the point closest to the atom; returned value is measured starting from atom accordingly
    QLineF middleLine{0.5 * (outer1.p1() + outer2.p1()), 0.5 * (outer1.p2() + outer2.p2())};

    if (m_newmanDiameter > 0) return getBondExtentForNewmanAtom(middleLine, lineWidth, m_newmanDiameter);

    if (Circle == m_shapeType) return getBondExtentForNewmanAtom(middleLine, lineWidth, diameterForCircularShape());

    IntersectionData edgeIntersection{intersectedEdge(middleLine, lineWidth)};

    QPolygonF fullBondPolygon({outer1.p1(), outer1.p2(), outer2.p2(), outer2.p1(), outer1.p1()});
    QList<qreal> possibleExtents{
          getExtentForEndOnCorner(fullBondPolygon, middleLine, edgeIntersection.getEdge().p1()),
          getExtentForEndOnCorner(fullBondPolygon, middleLine, edgeIntersection.getEdge().p2()),
          getExtentForIntersectionOfOuterLineAndEdge(edgeIntersection, outer1),
          getExtentForIntersectionOfOuterLineAndEdge(edgeIntersection, outer2)
    };

    return *(std::max_element(possibleExtents.begin(), possibleExtents.end()));
  }

  bool Atom::contains(const QPointF &point) const {
    if (m_newmanDiameter > 0)
      return QLineF(mapFromScene(pos()), point).length() <= (m_newmanDiameter + lineWidth())/2.;
    return QGraphicsItem::contains(point);
  }

  QPolygonF Atom::moveablePoints() const {
    return coordinates();
  }

  void Atom::updateShape() {
    prepareGeometryChange();
    m_shape = computeBoundingRect();
  }

  void Atom::setIndex(const QString &index) {
    m_index = index;
  }

  QString Atom::index() const {
    return m_index;
  }

  QPointF Atom::getBondDrawingStartFromBoundingBox(const QLineF& connection, qreal bondLineWidth) const {
    QRectF bounds = mapRectToScene(boundingRect());
    QVector<QPointF> corners;
    corners << bounds.bottomLeft() + QPointF(-bondLineWidth, bondLineWidth)
            << bounds.bottomRight() + QPointF(bondLineWidth, bondLineWidth)
            << bounds.topRight() + QPointF(bondLineWidth, -bondLineWidth)
            << bounds.topLeft() + QPointF(-bondLineWidth, -bondLineWidth)
            << bounds.bottomLeft() + QPointF(-bondLineWidth, bondLineWidth);
    for (int i = 0 ; i < 4 ; ++i)
    {
      QLineF edge(corners[i], corners[i+1]);
      QPointF result;
      if (intersectionType(connection, edge, &result) == QLineF::BoundedIntersection)
        return result;
    }
    return connection.p1();
  }

  QList<Atom*> Atom::neighbours() const
  {
    QList<Atom*> nbrs;
    foreach (Bond *bond, bonds())
      if (auto otherAtom = bond->otherAtom(this))
        nbrs << otherAtom;
    return nbrs;
  }

  QList<const XmlObjectInterface *> Atom::children() const { // TODO write test
    QList<QGraphicsItem*> allChildren = childItems();
    QList<const XmlObjectInterface*> serializableChildren;
    std::transform(allChildren.begin(), allChildren.end(), std::back_inserter(serializableChildren),
                   [](QGraphicsItem* item) { return dynamic_cast<const XmlObjectInterface*>(item); });
    serializableChildren.removeAll(nullptr);
    return serializableChildren;
  }

  XmlObjectInterface *Atom::produceChild(const QString &name, const QXmlStreamAttributes &attributes) {
    Q_UNUSED(attributes)
    if ("radicalElectron" == name) {
      RadicalElectron *radicalElectron = new RadicalElectron; // TODO default diameter?
      radicalElectron->setParentItem(this);
      return radicalElectron;
    }
    if ("lonePair" == name) {
      LonePair *lonePair = new LonePair;
      lonePair->setParentItem(this);
      return lonePair;
    }
    return graphicsItem::produceChild(name, attributes);
  }

  void LegacyAtom::readGraphicAttributes(const QXmlStreamAttributes &attributes) {
    m_legacyHydrogenCount = attributes.value("hydrogenCount").toInt();
    m_elementSymbol = attributes.value(ELEMENT_ATTRIBUTE).toString();
    m_index = attributes.value(ID_ATTRIBUTE).toString();
    m_newmanDiameter = qAbs(attributes.value(NEWMAN_DIAMETER_ATTRIBUTE).toDouble());
    updateShape();
  }

  void LegacyAtom::afterMoleculeReadFinalization() {
    setNumImplicitHydrogens(m_legacyHydrogenCount);
  }

} // namespace


