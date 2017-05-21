/***************************************************************************
 *   Copyright (C) 2017 Hendrik Vennekate                                  *
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
#ifndef MOLSKETCH_LIBRARYMODEL_H
#define MOLSKETCH_LIBRARYMODEL_H

#include <QAbstractTableModel>

namespace Molsketch {
class Molecule;

class LibraryModelPrivate;

class LibraryModel : public QAbstractListModel
{
  Q_DECLARE_PRIVATE(LibraryModel)
  QScopedPointer<LibraryModelPrivate> d_ptr;
public:
    LibraryModel();
    virtual ~LibraryModel() override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QMimeData* mimeData(const QModelIndexList &indexes) const override;
    /** Sets the molecules to be listed. Note that the model takes ownership. */
    void setMolecules(QList<Molecule *> molecules);
    /** Adds the molecule to the model's list. Assumes ownership. */
    void addMolecule(Molecule*molecule);
    QStringList mimeTypes() const override;
};

} // namespace Molsketch

#endif // MOLSKETCH_LIBRARYMODEL_H
