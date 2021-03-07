/***************************************************************************
 *   Copyright (C) 2007-2008 by Harm van Eersel                            *
 *   Copyright (C) 2009 Tim Vandermeersch                                  *
 *   Copyright (C) 2009 by Nicola Zonta                                    *
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

#include "arrow.h"
#include "bond.h"
#include "boundingboxlinker.h"
#include "frame.h"
#include "lonepair.h"
#include "molecule.h"
#include "molscene.h"
#include "radicalelectron.h"
#include "textitem.h"
#include "xmlobjectinterface.h"
#include <QMap>
#include <functional>
#include <atom.h>

class TypeMap : public QMap<QString,std::function<Molsketch::XmlObjectInterface*()> > {
  public:
  TypeMap() {
    using namespace Molsketch;
#define REGISTER_XML_CLASS(CLASS_NAME) { insert(CLASS_NAME::xmlClassName(), []() -> CLASS_NAME* { return new CLASS_NAME;}); }
    REGISTER_XML_CLASS(Atom);
    REGISTER_XML_CLASS(Bond);
    REGISTER_XML_CLASS(Molecule);
    REGISTER_XML_CLASS(TextItem);
    REGISTER_XML_CLASS(BoundingBoxLinker);
    REGISTER_XML_CLASS(LonePair);
    REGISTER_XML_CLASS(MolScene);
    REGISTER_XML_CLASS(RadicalElectron);
    REGISTER_XML_CLASS(Arrow);
    REGISTER_XML_CLASS(Frame);
    // TODO register settings items here?
  }
};
const TypeMap registeredTypes;

QXmlStreamReader &operator>>(QXmlStreamReader &in, Molsketch::XmlObjectInterface &object)
{
  return object.readXml(in) ;
}

QXmlStreamWriter &operator<<(QXmlStreamWriter &out, const Molsketch::XmlObjectInterface &object)
{
  return object.writeXml(out) ;
}

Molsketch::XmlObjectInterface *Molsketch::produceXmlObject(const QString &type) {
  if (!registeredTypes.contains(type)) return nullptr;
  return registeredTypes[type]();
}
