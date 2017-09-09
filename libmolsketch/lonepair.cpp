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
#include "lonepair.h"
#include <QDebug>

namespace Molsketch {

struct LonePairPrivate {
  bool operator ==(const LonePairPrivate& other) {}
};

LonePair::LonePair (qreal angle, qreal lineWidth, qreal length, BoundingBoxLinker linker, const QColor& color) {}

LonePair::LonePair (const LonePair& other) {}

LonePair::~LonePair () {}

BoundingBoxLinker LonePair::linker () const {}

QDebug operator<< (QDebug debug, const LonePair& lonePair) {}

QRectF LonePair::boundingRect () const {}

void LonePair::paint (QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {}

QString LonePair::xmlName () const {}

XmlObjectInterface* LonePair::produceChild (const QString& name, const QString& type) {}

void LonePair::readAttributes (const QXmlStreamAttributes& attributes) {}

QList<const XmlObjectInterface*> LonePair::children () const {}

QXmlStreamAttributes LonePair::xmlAttributes () const {}

} // namespace Molsketch
