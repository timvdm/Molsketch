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
#include "radicalelectron.h"
#include "graphicsitem.h"
#include <QColor>
#include <QDebug>
#include <QPainter>

namespace Molsketch {

  struct RadicalElectronPrivate {
    qreal diameter;
    QColor color;
    BoundingBoxLinker linker;
    bool operator==(const RadicalElectronPrivate& other) {
      return diameter == other.diameter
          && color == other.color
          && linker == other.linker;
    }
  };


  RadicalElectron::RadicalElectron(qreal diameter, BoundingBoxLinker linker, const QColor &color)
    : d_ptr(new RadicalElectronPrivate)
  {
    Q_D(RadicalElectron);
    d->diameter = diameter;

    d->color = color;
    d->linker = linker;
  }

  RadicalElectron::RadicalElectron(const RadicalElectron &other)
    : QGraphicsItem(nullptr),
      d_ptr(new RadicalElectronPrivate(*(other.d_ptr))) {}

  RadicalElectron::~RadicalElectron() {}

  QRectF RadicalElectron::boundingRect() const {
    if (!parentItem()) return QRectF();
    Q_D(const RadicalElectron);
    QRectF bounds(0,0,d->diameter,d->diameter);
    bounds.translate(d->linker.getShift(parentItem()->boundingRect(), bounds));
    return bounds;
  }

  void RadicalElectron::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option)
    Q_UNUSED(widget)
    Q_D(const RadicalElectron);
    if (!parentItem()) return;
    painter->save(); // TODO write test for this
    // TODO maybe use offset if atom not shown or force rendering of atom if it has children?
    painter->setBrush(d->color);
    painter->drawEllipse(boundingRect());
    painter->restore();
  }

  BoundingBoxLinker RadicalElectron::linker() const {
    Q_D(const RadicalElectron);
    return d->linker;
  }

  bool RadicalElectron::operator==(const RadicalElectron &other) {
    return *(other.d_ptr) == *(d_ptr);
  }

  QString RadicalElectron::xmlName() const {
    return "radicalElectron";
  }

  void RadicalElectron::readAttributes(const QXmlStreamAttributes &attributes){
    Q_D(RadicalElectron);
    d->color = graphicsItem::extractColor(attributes);
    d->diameter = attributes.value("diameter").toDouble();
  }

  QDebug operator<<(QDebug debug, const RadicalElectron &radicalElectron) {
    debug.nospace() << "RadicalElectron(diameter: " << radicalElectron.d_func()->diameter
                    << ", color: " << radicalElectron.d_func()->color
                    << ", linker: " << radicalElectron.d_func()->linker
                    << ")";
    return debug;
  }

  QXmlStreamAttributes RadicalElectron::xmlAttributes() const {
    Q_D(const RadicalElectron);
    QXmlStreamAttributes attributes;
    attributes.append("diameter", QString::number(d->diameter));
    graphicsItem::addColor(attributes, d->color);
    return attributes;
  }

  QList<const XmlObjectInterface *> RadicalElectron::children() const {
    Q_D(const RadicalElectron);
    return QList<const XmlObjectInterface*>() << &(d->linker);
  }

  XmlObjectInterface *RadicalElectron::produceChild(const QString &name, const QString &type) {
    Q_D(RadicalElectron);
    if (d->linker.xmlName() == name) return &(d->linker);
    return abstractXmlObject::produceChild(name, type);
  }
} // namespace Molsketch
