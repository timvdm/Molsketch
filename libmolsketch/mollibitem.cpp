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

#include <QtGui>

#include "molecule.h"
#include "bond.h"
#include "molscene.h"
#include "mollibitem.h"
#include "fileio.h"

namespace Molsketch {

  MolLibItem::MolLibItem( Molecule* molecule, const QString & fileName )
      : m_molecule(molecule),
        m_fileName(fileName)
  {
    // pre: molecule is a valid molecule
    Q_CHECK_PTR(molecule);
    //Q_ASSERT(!molecule->atoms().isEmpty());

    // Copying the molecule
//     m_molecule = new Molecule(molecule);
    m_molecule->setPos(0, 0);
    QPolygonF coords = m_molecule->coordinates();
    m_molecule->setCoordinates(coords.translated(-coords.boundingRect().center()));


//    // Creating pixmap
    setIcon(QIcon(renderMolecule(*m_molecule)));

    QString name = molecule->getName();
    setText(name.isEmpty() ? fileName : name);
  }

  MolLibItem::~MolLibItem( )
  {
    delete m_molecule;
  }

  Molecule MolLibItem::getMolecule( )
  {
    return *m_molecule;
  }
} // namespace
