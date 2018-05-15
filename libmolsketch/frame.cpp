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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/
#include "frame.h"

#include "arrow.h"
#include "molscene.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QDebug>
#include <molecule.h>
#include "scenesettings.h"
#include "settingsitem.h"

//// code for parsing frame instructions TODO externalize
///
/* Syntax for coordinates:
 * + - add to current coordinate
 * r - relative value (relative to bounding box)
 * f - relative value (relative to font size)
 *
 * Postfix: (use coord stack)
 * - - line to
 * . - quadratic to
 * <nothing> - move to
 * $ - do not advance path, just parse coordinate
 */

class CoordinateParser
{
private:
  QPointF currentCoordinate;
  qreal relativeX, relativeY, fontX, fontY;
  void applyScaling(const QString& rfStringX, const QString& rfStringY, QPointF& coordinate) const
  {
    if ("r" == rfStringX) coordinate.rx() *= relativeX;
    if ("f" == rfStringX) coordinate.rx() *= fontX;
    if ("r" == rfStringY) coordinate.ry() *= relativeY;
    if ("f" == rfStringY) coordinate.ry() *= fontY;
  }
public:
  CoordinateParser(qreal relativeX = 0,
                   qreal relativeY = 0,
                   qreal fontX = 0,
                   qreal fontY = 0)
    : relativeX(relativeX),
      relativeY(relativeY),
      fontX(fontX),
      fontY(fontY)
  {}
  void reset() { setCurrentCoordinate(QPointF()); }
  QPointF getCurrentCoordinate() const { return currentCoordinate; }
  void setCurrentCoordinate(const QPointF &value) { currentCoordinate = value; }
  void parse(const QStringList& l)
  {
    if (l.size() != 5)
    {
      qDebug() << "coordinateParser: invalid number of strings to parse: " + QString::number(l.size());
      return;
    }
    QPointF coord(l[2].toDouble(), l[4].toDouble());
    applyScaling(l[1], l[3], coord);
    if ("+" == l[0]) currentCoordinate += coord;
    else currentCoordinate = coord;
  }
};

class PathSegmentParser
{
private:
  QRegExp re;
protected:
  virtual void process(QPainterPath& path, CoordinateParser& parser) = 0;
  const QRegExp& regExp() const { return re; }
public:
  PathSegmentParser(const QString& re)
    : re(re)
  {}
  virtual ~PathSegmentParser() {}
  bool processed(QPainterPath& path, const QString& segment, int& index, CoordinateParser& parser)
  {
    if (!match(segment, index)) return false;
    index += re.matchedLength();
    process(path, parser);
    return true;
  }
  bool match(const QString& segment, const int& index) const
  {
    return re.indexIn(segment, index) == index;
  }
  static QString coordinateRegExp()
  {
    QString numberRegExp("([rf]?)([+-]?[0-9]*.?[0-9]+(?:[eE][+-]?[0-9])?)");
    return "(\\+?)\\(" + numberRegExp + "," + numberRegExp + "\\)";
  }
};

template <void (QPainterPath::*fp)(const QPointF&)>
class SinglePointSegment : public PathSegmentParser
{
  void process(QPainterPath &path, CoordinateParser &parser)
  {
    parser.parse(regExp().capturedTexts().mid(1));
    (path.*fp)(parser.getCurrentCoordinate());
  }
public:
  SinglePointSegment<fp>(const QString& prefix) : PathSegmentParser(prefix + coordinateRegExp()) {}
};

class SilentMoveSegment : public PathSegmentParser
{
  void process(QPainterPath &path, CoordinateParser &parser)
  {
    Q_UNUSED(path)
    parser.parse(regExp().capturedTexts().mid(1));
  }
public:
  SilentMoveSegment() : PathSegmentParser("$") {}
};

class QuadToSegment : public PathSegmentParser
{
  void process(QPainterPath &path, CoordinateParser &parser)
  {
    parser.parse(regExp().capturedTexts().mid(1,5));
    QPointF point1(parser.getCurrentCoordinate());
    parser.parse(regExp().capturedTexts().mid(6));
    path.quadTo(point1, parser.getCurrentCoordinate());
  }
public:
  QuadToSegment() : PathSegmentParser("." + coordinateRegExp() + "." + coordinateRegExp()) {}
};
//// end code for parsing


namespace Molsketch {

  class Frame::privateData
  {
  private:
    QList<PathSegmentParser*> segmentParsers;
    Frame *frame;
  public:
    privateData(Frame *frame)
      : frame(frame),
        isHovering(false)
    {
      segmentParsers << new SinglePointSegment<&QPainterPath::moveTo>("")
                     << new SinglePointSegment<&QPainterPath::lineTo>("-")
                     << new QuadToSegment
                     << new SilentMoveSegment
                        ;
    }

    QRectF baseRect;
    QString framePathCode;
    bool isHovering;

    void highlightPoints(QPainter* painter)
    {// TODO move this to graphicsItem; highlight selected point
      if (!isHovering || !frame->childItems().isEmpty()) return;
      painter->save();
      painter->setPen(Qt::red);
      painter->setBrush(Qt::NoBrush);
      foreach(QPointF point, frame->moveablePoints())
        painter->drawEllipse(point, 5, 5);
      painter->restore();
    }

    void refreshBaseRect()
    {
      if (frame->childItems().isEmpty()) return;
      baseRect = frame->childrenBoundingRect();
    }

    QPainterPath parseFramePath()
    {
      refreshBaseRect();

      CoordinateParser parser(
            baseRect.width(), // relX
            baseRect.height(), // rely
            10, // fontX TODO
            10); // fontY

      auto purePath = framePathCode;
      purePath.remove(QRegExp("\\s+"));

      QPainterPath painterPath;
      int currentIndex = 0;
      while (currentIndex < purePath.size())
      {
        bool found = false;
        for (auto segmentParser : segmentParsers)
          if ((found = segmentParser->processed(painterPath, purePath, currentIndex, parser)))
            break;
        if (!found) break;
      }

      painterPath.translate(baseRect.center());
      return painterPath;
    }
  };

  Frame::Frame(QGraphicsItem *parent) // TODO rotation around center of parentItem (new variable d->rotationAngle)
    : graphicsItem(parent),
      d(new privateData(this))
  {
#if QT_VERSION < 0x050000
    setAcceptsHoverEvents(true);
#else
    setAcceptHoverEvents(true) ;
#endif
    setZValue(10);
  }

  Frame::~Frame()
  {
    delete d;
  }

  void Frame::setCoordinates(const QVector<QPointF> &c)
  {
    d->baseRect.setTopLeft(c[0]);
    d->baseRect.setBottomRight(c[1]);
  }

  QPolygonF Frame::coordinates() const
  {
    return QPolygonF()
        << d->baseRect.topLeft()
        << d->baseRect.bottomRight();
  }

  QPolygonF Frame::moveablePoints() const
  {
    QPolygonF mps;
    mps
        << d->baseRect.topLeft()
        << d->baseRect.topRight()
        << d->baseRect.bottomLeft()
        << d->baseRect.bottomRight()
        << (d->baseRect.topLeft() + d->baseRect.topRight())/2
        << (d->baseRect.bottomLeft() + d->baseRect.bottomRight())/2
        << (d->baseRect.topLeft() + d->baseRect.bottomLeft())/2
        << (d->baseRect.topRight() + d->baseRect.bottomRight())/2;
    return mps;
  }

  QPointF Frame::getPoint(const int &index) const
  {
    if (index < 0 || index >= moveablePoints().size()) return QPointF();
    return moveablePoints().at(index);
  }

  void Frame::movePointBy(const QPointF &offset, int pointIndex)
  {
    switch(pointIndex)
    {
      case 0: d->baseRect.setTopLeft(d->baseRect.topLeft() + offset); break;
      case 1: d->baseRect.setTopRight(d->baseRect.topRight() + offset); break;
      case 2: d->baseRect.setBottomLeft(d->baseRect.bottomLeft() + offset); break;
      case 3: d->baseRect.setBottomRight(d->baseRect.bottomRight() + offset); break;
      case 4: d->baseRect.setTop(d->baseRect.top() + offset.y()); break;
      case 5: d->baseRect.setBottom(d->baseRect.bottom() + offset.y()); break;
      case 6: d->baseRect.setLeft(d->baseRect.left() + offset.x()); break;
      case 7: d->baseRect.setRight(d->baseRect.right() + offset.x()); break;
      default: graphicsItem::movePointBy(offset, pointIndex);
    }
  }

  void Frame::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
  {
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->save();

    QPen pen; // TODO make function in graphicsItem for this
    pen.setWidthF(lineWidth());
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    pen.setColor(getColor());
    painter->setPen(pen);

    QPainterPath painterPath = d->parseFramePath();
    painter->drawPath(painterPath);
    // TODO check path coordinates vs. bounding rect of parentItem()
    // TODO incorporate path size in boundingRect() (maybe

    painter->restore();

    d->highlightPoints(painter);
  }

  QRectF Frame::boundingRect() const // TODO include selectable points if active/selected (hovered/clicked)
  {
    QRectF br = d->parseFramePath().boundingRect();
    if (!d->isHovering) return br;
    br.setTopLeft(br.topLeft() - QPointF(5,5));
    br.setBottomRight(br.bottomRight() + QPointF(5,5));
    return  br;
  }

//  QRectF Frame::boundingRect() const
//  {
//    QRectF moveablePointsRect = moveablePoints().boundingRect();
//    moveablePointsRect.adjust(-pointSelectionDistance(),
//                              -pointSelectionDistance(),
//                              pointSelectionDistance(),
//                              pointSelectionDistance());
//    return boundingRect() | moveablePointsRect; // TODO make bounding rect = ownBoundingRect() | moveablePoints() ?
//  }

  void Frame::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
  {
    event->ignore();
  }

  void Frame::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
  {
    d->isHovering = true;
    graphicsItem::hoverEnterEvent(event);
  }

  void Frame::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
  {
    d->isHovering = false;
    graphicsItem::hoverLeaveEvent(event);
  }

  void Frame::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
  {
    graphicsItem::hoverMoveEvent(event);
  }

  int Frame::coordinateCount() const
  {
    return 2;
  }

  void Frame::setFrameString(const QString &frameString)
  {
    d->framePathCode = frameString;
  }

  QString Frame::frameString() const
  {
    return d->framePathCode;
  }

  QString Frame::xmlName() const {
    return xmlClassName();
  }

  QString Frame::xmlClassName() {
    return "frame";
  }

  void Frame::readGraphicAttributes(const QXmlStreamAttributes &attributes)
  {
    d->framePathCode = attributes.value("framePath").toString();
  }

  QXmlStreamAttributes Frame::graphicAttributes() const
  {
    QXmlStreamAttributes attributes;
    attributes.append("framePath", d->framePathCode);
    return attributes;
  }

  void Frame::prepareContextMenu(QMenu *contextMenu)
  {
    Q_UNUSED(contextMenu);
  }

  abstractXmlObject *Frame::produceChild(const QString &name, const QString &type)
  {
    Q_UNUSED(type)
    if ("molecule" == name) return new Molecule(this);
    if ("arrow" == name) return new Arrow(this); // TODO make static function for this
    if ("frame" == name) return new Frame(this);
    return 0;
  }

  QList<const XmlObjectInterface *> Frame::children() const
  {
    QList<const XmlObjectInterface*> list;
    for (auto child : childItems())
      list << dynamic_cast<graphicsItem*>(child);
    list.removeAll(0);
    return list;
  }

  qreal Frame::sceneLineWidth(MolScene *scene) const {
    return scene->settings()->frameLineWidth()->get();
  }


} // namespace Molsketch
