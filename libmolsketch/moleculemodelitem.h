/***************************************************************************
 *   Copyright (C) 2017 by Hendrik Vennekate                               *
 *   HVennekate@gmx.de                                                     *
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
#ifndef MOLSKETCH_MOLECULEMODELITEM_H
#define MOLSKETCH_MOLECULEMODELITEM_H

#include <QScopedPointer>

class QIcon;
class QString;
class QXmlStreamWriter;

namespace Molsketch {
  class Molecule;
  class MoleculeModelItemPrivate;

  /**
   * @brief The MoleculeModelItem class for use in the LibraryModel
   *
   * This class facilitates lazy generation of molecules and model information
   * (icon, XML data) and caching thereof.
   * Subclasses need only implement the produceMolecule() method which shall
   * generate a molecule from which this class will then obtain all further
   * information as the need arises.
   */
  class MoleculeModelItem
  {
    Q_DECLARE_PRIVATE(MoleculeModelItem)
    QScopedPointer<MoleculeModelItemPrivate> d_ptr;
  protected:
    MoleculeModelItem();
    virtual Molecule* produceMolecule() const = 0;
  public:
    virtual ~MoleculeModelItem();
    QIcon icon();
    QString name();
    void writeXml(QXmlStreamWriter& writer);
    static MoleculeModelItem* fromXml(QByteArray xml);
    static MoleculeModelItem* fromInChI(QString name, QString inchi);
  };
} // namespace Molsketch

#endif // MOLSKETCH_MOLECULEMODELITEM_H
