/***************************************************************************
 *   Copyright (C) 2015 Hendrik Vennekate                                  *
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
#ifndef GRAPHICSITEM_H
#define GRAPHICSITEM_H

#include <QGraphicsItem>
#include "abstractxmlobject.h"

#include "qtversionmacros.h"

class QUndoCommand ;

namespace Molsketch {

  class MolScene ;

  class graphicsItem : public QGraphicsItem, public abstractXmlObject
  {
  public:

    enum Types {
      MoleculeType = QGraphicsItem::UserType + 1,
      AtomType = QGraphicsItem::UserType + 2,
      BondType = QGraphicsItem::UserType + 3,
      ResidueType = QGraphicsItem::UserType + 4, // TODO deprecated
      TextInputType = QGraphicsItem::UserType + 5,
      ReactionArrowType = QGraphicsItem::UserType + 6, // TODO deprecated
      MechanismArrowType = QGraphicsItem::UserType + 7, // TODO deprecated
      GeneralArrowType = QGraphicsItem::UserType + 8,
      FrameType = QGraphicsItem::UserType + 9,
      LonePairType = QGraphicsItem::UserType + 10,
      RadicalType = QGraphicsItem::UserType + 11
    };

    graphicsItem(QGraphicsItem* parent = 0 GRAPHICSSCENEHEADER);
    graphicsItem(const graphicsItem& other GRAPHICSSCENEHEADER);
    ~graphicsItem();
    // TODO XML i/o
    void setColor(const QColor &color) ;
    QColor getColor() const ;
    void setRelativeWidth(const double& w) ;
    qreal lineWidth() const ;
    qreal relativeWidth() const ;

    /** Event handlers */
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    /** coordinate functions */
    virtual QPolygonF coordinates() const = 0;
    virtual void setCoordinates(const QVector<QPointF>& c) = 0;
    void setCoordinates(const QPolygonF& c);
    virtual void setCoordinate(const int& index, const QPointF& p);
    virtual QPointF getPoint(const int& index) const;
    virtual QPointF lastPoint() const;
    virtual QPointF firstPoint() const;
    virtual int coordinateCount() const;
    virtual void swapPoint(const int& index, QPointF& p);
    virtual QPolygonF moveablePoints() const;
    virtual void movePointBy(const QPointF& offset, int pointIndex = -1);
    virtual void moveItemBy(const QPointF& offset);

    virtual void prepareContextMenu(QMenu* contextMenu);
    virtual QWidget *getPropertiesWidget();

    static QXmlStreamAttributes& addColor(QXmlStreamAttributes& attributes, const QColor& color);
    static QColor extractColor(const QXmlStreamAttributes& attributes);
    static QByteArray serialize(const QList<const graphicsItem*> items);
    static QList<graphicsItem*> deserialize(const QByteArray& input);

  protected:
    qreal pointSelectionDistance() const;
    void readAttributes(const QXmlStreamAttributes &attributes) ;
    QXmlStreamAttributes xmlAttributes() const ;
    virtual void readGraphicAttributes(const QXmlStreamAttributes& attributes) { Q_UNUSED(attributes)}
    virtual QXmlStreamAttributes graphicAttributes() const { return QXmlStreamAttributes() ; }
    /**
     * Attempt to push a command onto the undo stack. If none is available, execute the command and dispose of it.
     */
    void attemptUndoPush(QUndoCommand* command) ;
    void attemptBeginMacro(const QString& text);
    void attemptEndEndMacro();
    int selectedPoint() const;
    virtual void prepareItemContextMenu(QMenu* contextMenu);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

  private:
    QColor m_color ;
    qreal lineWidthScaling ;
    class privateData;
    privateData *d;
    virtual qreal sceneLineWidth(MolScene* scene) const ;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) ;
    // TODO automatically return item children from graphicsitem as xmlitem
  };

} // namespace

#endif // GRAPHICSITEM_H
