/***************************************************************************
 *   Copyright (C) 2017 by Hendrik Vennekate                               *
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
#include "graphicsitem.h"
#include "lonepair.h"
#include <QDebug>
#include <QPen>

namespace Molsketch {

struct LonePairPrivate {
  BoundingBoxLinker linker;
};

LonePair::LonePair (qreal angle, qreal lineWidth, qreal length, BoundingBoxLinker linker, const QColor& color)
  : QGraphicsLineItem(QLineF::fromPolar(length, angle)),
    d_ptr(new LonePairPrivate)
{
  Q_D(LonePair);
  d->linker = linker;
  setPen(QPen(color, lineWidth));
}

LonePair::LonePair (const LonePair& other)
  : QGraphicsLineItem(other.line()),
    d_ptr(new LonePairPrivate(*(other.d_ptr)))
{
  setPen(other.pen());
}

LonePair::~LonePair () {}

BoundingBoxLinker LonePair::linker () const {
  Q_D(const LonePair);
  return d->linker;
}

bool LonePair::operator ==(const LonePair &other) {
  return other.d_ptr->linker == d_ptr->linker
      && other.line() == line()
      && other.pen() == pen();
}

QDebug operator<< (QDebug debug, const LonePair& lonePair) {
  return debug.nospace() << "LonePair("
                         << "line: " << lonePair.line()
                         << ", pen: " << lonePair.pen()
                         << ", linker: " << lonePair.d_ptr->linker
                         << ")";
}

QRectF LonePair::boundingRect () const {
  // TODO
  return QRectF();
}

void LonePair::paint (QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
  if (!parentItem()) return;
  // TODO round line caps
  QGraphicsLineItem::paint(painter, option, widget);
}

QString LonePair::xmlName () const {
  return "lonePair";
}

XmlObjectInterface* LonePair::produceChild (const QString& name, const QString& type) {
  Q_D(LonePair);
  if (d->linker.xmlName() == name) return &(d->linker);
  return abstractXmlObject::produceChild(name, type);
}

void LonePair::readAttributes (const QXmlStreamAttributes& attributes) {
  Q_D(LonePair);
  setPen(QPen(graphicsItem::extractColor(attributes),
              attributes.value("lineWidth").toDouble()));
  setLine(QLineF::fromPolar(attributes.value("length").toDouble(),
                            attributes.value("angle").toDouble()));
}

QList<const XmlObjectInterface*> LonePair::children () const {
  Q_D(const LonePair);
  return QList<const XmlObjectInterface*>() << &(d->linker);
}

QXmlStreamAttributes LonePair::xmlAttributes () const {
  Q_D(const LonePair);
  QXmlStreamAttributes attributes;
  attributes.append("angle", QString::number(line().angle()));
  attributes.append("length", QString::number(line().length()));
  attributes.append("lineWidth", QString::number(pen().widthF()));
  graphicsItem::addColor(attributes, pen().color());
  return attributes;
}

} // namespace Molsketch
