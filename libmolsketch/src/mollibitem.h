/***************************************************************************
 *   Copyright (C) 2007-2008 by Harm van Eersel                            *
 *   devsciurus@xs4all.nl                                                  *
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

#ifndef MOLLIBITEM_H
#define MOLLIBITEM_H

#include <QListWidgetItem>
#include <QFileInfo>

namespace Molsketch {

class Molecule;

// class QTableWidgetItem;

/**
 * Represents an item in the molecule library
 *
 * @author Harm van Eersel
 */
class MolLibItem : public QListWidgetItem
{
  public:
  /** Creates a new library item.
   *
   * @param molecule a pointer to the molecule that should be copied to the library
   * @param name the name of the library item
   */
  MolLibItem(Molecule* molecule, const QString & name);
  
  /** Destructor of the library item. */
  virtual ~MolLibItem();

  /** Sets a copy of @p molecule as the molecule of the library item */
  //void setMolecule(Molecule* molecule);

  /** Returns a pointer to copy of molecule of the library item. */
  Molecule* getMolecule();
  
  /** Returns the filename of the library item. */
  QFileInfo getFileName() { return m_fileName; };

private:
  /** Stores the molecule of the library item. */
  Molecule* m_molecule;
  /** Stores the filename of the library item. */
  QFileInfo m_fileName;
};

} // namespace

#endif
