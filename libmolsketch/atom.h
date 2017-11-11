/***************************************************************************
 *   Copyright (C) 2007-2008 by Harm van Eersel                            *
 *   Copyright (C) 2009 by Tim Vandermeersch                               *
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

#ifndef MSK_ATOM_H
#define MSK_ATOM_H

#include <QList>

#include "graphicsitem.h"

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
    int type() const { return Type; }

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
    virtual QRectF boundingRect() const;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
    qreal annotationDirection() const ;

    bool isDrawn() const;
    bool isHidden() const;
    void setCoordinates(const QVector<QPointF> &c) ;
    QPolygonF coordinates() const ;
    virtual Molecule* molecule() const;
    void setMolecule(Molecule *molecule);
    QString element() const;
    void setElement(const QString & element);
    void setNewmanDiameter(const qreal& diameter);
    qreal getNewmanDiameter() const;
    void disableNewman();
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
    void setNumUnpairedElectrons(int n)
    {
      m_userElectrons = n;
    }

    void setHidden(bool hidden);
    /// Returns the string for the superscript charge (e.g. "3-", "2-", "-", "", "+", "2+", ...).
    QString chargeString() const;


    bool hasLabel() const;
    QString string ()const;


    int numBonds() const;
    int bondOrderSum() const;
    /// Get the number of non-bonding electrons (e.g. 4 for O=, 2 for NH3, 1 for radicals).
    int numNonBondingElectrons() const;
    bool hasImplicitHydrogens() const;
    int numImplicitHydrogens() const;

    void addBond(Bond *bond);
    void removeBond(Bond *bond);
    QList<Bond*> bonds() const;
    QList<Atom*> neighbours() const;
    /** Sets the number of implicit hydrogens of the current atom to @p number.
      * Changing the number of implicit hydrogens will also effect the number
      * of free valency electrons and hence the charge of the atom.
      */
    void setNumImplicitHydrogens(const int &number);
    void enableImplicitHydrogens(bool enabled);

    void hoverOut () {m_hidden = true;}
    QString xmlName() const;
    static QString xmlClassName();
    Molsketch::Alignment labelAlignment() const;
    Bond *bondTo(Atom *other) const;
    QWidget* getPropertiesWidget();
    void propertiesWidgetDestroyed();
    QPointF bondDrawingStart(const Atom *other) const;
  protected:
    // Event handlers
    /** Event handler to show hidden atoms when the mouse hovers over them. */
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
    /** Event handler to hide hidden atoms again after a mouse hovering event. */
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);
    /** Event handler to handle atom clicks. */
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    /** Event handler to handle element changes. */
    QVariant itemChange(GraphicsItemChange change, const QVariant & value);
    /** Event handler to edit element (double click) */
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

    void readGraphicAttributes(const QXmlStreamAttributes &attributes) ;
    QXmlStreamAttributes graphicAttributes() const ;
    QList<const XmlObjectInterface *> children() const override;
    XmlObjectInterface* produceChild(const QString &name, const QString &type);

  private:
    void drawAtomLabel(QPainter *painter, const QString &lbl, int alignment);
    QRectF computeBoundingRect();

    void initialize(const QPointF & position,
                    const QString & element,
                    bool implicitHydrogens);

    QString m_elementSymbol;
    bool m_hidden;
    int m_userCharge;
    int m_userElectrons;
    qreal m_newmanDiameter;

    QList<Bond*> m_bonds;
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
  };

} // namespace

#endif
