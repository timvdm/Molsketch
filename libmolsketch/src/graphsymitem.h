/***************************************************************************
 *   Copyright (C) 2009 Tim Vandermeersch                                  *
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
#ifndef GRAPHSYMITEM_H
#define GRAPHSYMITEM_H

#ifdef QMAKEBUILD
#include <molinputitem.h>
#else
#include <molsketch/molinputitem.h>
#endif

namespace Molsketch {

  class Molecule;

  class GraphSymItem : public MolInputItem
  {
    public:
      GraphSymItem();

      QString input() const { return "Molecule"; }
      QString output() const { return "Symmetry Classes"; }
      QString label() const { return output(); }
      void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    private:
  };

  ITEM_PLUGIN_FACTORY(GraphSymItem, "Molecule", "Symmetry Classes")

}

#endif // SMILESITEM_H
