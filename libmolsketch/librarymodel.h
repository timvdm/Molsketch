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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/
#ifndef MOLSKETCH_LIBRARYMODEL_H
#define MOLSKETCH_LIBRARYMODEL_H

#include <QAbstractTableModel>

namespace Molsketch {
class Molecule;
class MoleculeModelItem;
class LibraryModelPrivate;

class LibraryModel : public QAbstractListModel
{
  Q_DECLARE_PRIVATE(LibraryModel)
  QScopedPointer<LibraryModelPrivate> d_ptr;
public:
    explicit LibraryModel(QObject *parent = nullptr);
    virtual ~LibraryModel() override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QMimeData* mimeData(const QModelIndexList &indexes) const override;
    /** Sets the molecules to be listed. Note that the model takes ownership. */
    void setMolecules(QList<MoleculeModelItem *> molecules);
    /** Adds the molecule to the model's list. Assumes ownership. */
    void addMolecule(MoleculeModelItem *molecule);
    QStringList mimeTypes() const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool canFetchMore(const QModelIndex &parent) const override;
    void fetchMore(const QModelIndex &parent) override;
};

} // namespace Molsketch

#endif // MOLSKETCH_LIBRARYMODEL_H
