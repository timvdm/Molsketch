/***************************************************************************
 *   Copyright (C) 2007-2008 by Harm van Eersel                            *
 *   Copyright (C) 2009 by Tim Vandermeersch                               *
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

#ifndef MSK_ATOM_H
#define MSK_ATOM_H

#include <QList>

#include "graphicsitem.h"
#include "sumformula.h"

namespace Molsketch {

  class Bond;
  class Molecule;

  enum Alignment {
    Left,
    Right,
    Up,
    Down
  };

  /// Atom class
  class Atom : public graphicsItem
  {
    friend class Molecule;

  public:
    enum { Type = AtomType };
    int type() const override { return Type; }

    enum ShapeType { Rectangle = 0, Circle = 1 };

    /**
       * Creates a new atom.
       *
       * @param position the position of the new atom
       * @param element the element symbol of the new atom
       * @param invisible makes the atom invisible if @c true
       */
    Atom(const QPointF & position = QPointF(), const QString & element = QString(),
         bool implicitHydrogens = true, QGraphicsItem* parent = 0 GRAPHICSSCENEHEADER ) ;
    Atom(const Atom& other GRAPHICSSCENEHEADER);
    ~Atom() ;
    virtual QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    qreal annotationDirection() const ;

    bool isDrawn() const;
    void setCoordinates(const QVector<QPointF> &c) override;
    QPolygonF coordinates() const override;
    virtual Molecule* molecule() const;
    void setMolecule(Molecule *molecule);
    QString element() const;
    void setElement(const QString & element);
    ShapeType shapeType() const;
    void setShapeType(const ShapeType& shapeType);
    void setNewmanDiameter(const qreal& diameter);
    qreal getNewmanDiameter() const;
    void disableNewman();
    SumFormula sumFormula() const;
    /** Returns the charge of the atom.
      * FC = # valency electrons - 0.5 * # shared electrons - # unpaired electrons + user specified contribution
      */
    int charge() const;
    /** Set the user specified contribution to the atom charge.
      * FC = # valency electrons - 0.5 * # shared electrons - # unpaired electrons + user specified contribution
      */
    void setCharge(const int &charge);
    /**
       * @return The number of unpaired electrons (radicals).
       */
    int numUnpairedElectrons() const
    {
      return m_userElectrons;
    }
    void setNumUnpairedElectrons(int n) // TODO this seems unused
    {
      m_userElectrons = n;
    }

    /// Returns the string for the superscript charge (e.g. "3-", "2-", "-", "", "+", "2+", ...).
    QString chargeString() const;


    bool hasLabel() const;
    QString string ()const;


    int numBonds() const;
    int bondOrderSum() const;
    /// Get the number of non-bonding electrons (e.g. 4 for O=, 2 for NH3, 1 for radicals).
    int numNonBondingElectrons() const;
    int numImplicitHydrogens() const;

    QList<Bond*> bonds() const;
    QList<Atom*> neighbours() const;
    /** Sets the number of implicit hydrogens of the current atom to @p number.
      * Changing the number of implicit hydrogens will also effect the number
      * of free valency electrons and hence the charge of the atom.
      */
    void setNumImplicitHydrogens(const int &number);

    QString xmlName() const override;
    static QString xmlClassName();
    Molsketch::Alignment labelAlignment() const;
    Bond *bondTo(Atom *other) const;
    QWidget* getPropertiesWidget() override;
    void propertiesWidgetDestroyed();
    QPointF bondDrawingStart(const Atom *other, qreal bondLineWidth) const;
    bool contains(const QPointF &point) const override;
    QPolygonF moveablePoints() const override;

    void updateShape();
    void setIndex(const QString& index);
    QString index() const;
    qreal getBondExtent(const QLineF &outer1, const QLineF &outer2, qreal lineWidth) const;
    virtual void afterMoleculeReadFinalization() {}
  protected:
    // Event handlers
    /** Event handler to handle element changes. */
    QVariant itemChange(GraphicsItemChange change, const QVariant & value) override;
    /** Event handler to edit element (double click) */
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

    void readGraphicAttributes(const QXmlStreamAttributes &attributes) override;
    QXmlStreamAttributes graphicAttributes() const override;
    QList<const XmlObjectInterface *> children() const override;
    XmlObjectInterface* produceChild(const QString &name, const QXmlStreamAttributes &attributes) override;

  private:
    void drawAtomLabel(QPainter *painter, const QString &lbl, int alignment);
    QRectF computeBoundingRect();

    void initialize(const QPointF & position,
                    const QString & element,
                    bool implicitHydrogens);

    QString m_elementSymbol;
    int m_userCharge;
    int m_userElectrons;
    qreal m_newmanDiameter;
    QString m_index;
    ShapeType m_shapeType;

    int m_userImplicitHydrogens;
    bool m_implicitHydrogens;
    QRectF m_shape;
    qreal computeTotalWdith(const int &alignment, const QString &lbl, const QFontMetrics &fmSymbol, const QFontMetrics &fmScript);
    QFont getSymbolFont() const;
    QFont getSubscriptFont(const QFont &symbolFont) const;
    QPair<QFont, QFont> getFonts() const;
    QString composeLabel(bool leftAligned) const;
    qreal computeXOffset(int alignment, const QFontMetrics &fmSymbol, const QString &lbl, const qreal &totalWidth);
    void drawElectrons(QPainter* painter);
    void drawCharge(QPainter* painter);
    void renderColoredSquare(QPainter* painter);
    void renderColoredCircle(QPainter* painter);
    void renderColoredShape(QPainter *painter, void (QPainter::*drawMethod)(int, int, int, int));
    void drawSelectionHighlight(QPainter* painter);
    QString getLabelWithHydrogens();
    void drawNewman(QPainter *painter);
    QPointF getBondDrawingStartFromBoundingBox(const QLineF &connection, qreal bondLineWidth) const;
    bool showHoverPoint() const override { return false; }
    class IntersectionData;
    IntersectionData intersectedEdge(const QLineF &line, qreal lineWidth) const;
    qreal getExtentForIntersectionOfOuterLineAndEdge(const Atom::IntersectionData &edgeIntersection, const QLineF &outer) const;
    qreal getExtentForEndOnCorner(const QPolygonF &fullBondPolygon, const QLineF &middleLine, const QPointF &corner) const;
    friend class LegacyAtom;
  };

  class LegacyAtom : public Atom {
  private:
    int m_legacyHydrogenCount;
  protected:
    void readGraphicAttributes(const QXmlStreamAttributes &attributes) override;
  public:
    void afterMoleculeReadFinalization() override;
  };

} // namespace

#endif
