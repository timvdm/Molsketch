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
#include "librarymodel.h"
#include "constants.h"
#include "molecule.h"
#include "stringify.h"

#include <QIcon>
#include <QMimeData>
#include <QSet>

namespace Molsketch {

  class LibraryModelPrivate {
    Q_DISABLE_COPY(LibraryModelPrivate)
  public:
    LibraryModelPrivate(){}
    QList<Molecule*> molecules;
    void cleanMolecules() {
      qInfo("Clearing list of molecules. Count: %d", molecules.size());
      for (auto molecule : molecules.toSet()) delete molecule;
      molecules.clear();
    }
    ~LibraryModelPrivate() {
      cleanMolecules();
    }
  };

LibraryModel::LibraryModel(QObject *parent)
  : QAbstractListModel(parent),
    d_ptr(new LibraryModelPrivate)
{}

LibraryModel::~LibraryModel() {}

int LibraryModel::rowCount(const QModelIndex &parent) const {
  Q_UNUSED(parent)
  Q_D(const LibraryModel);
  return d->molecules.size();
}

QVariant LibraryModel::data(const QModelIndex &index, int role) const {
  qInfo("Getting data of type %d for index %d", role, index.row());
  Q_D(const LibraryModel);
  int row = index.row();
  if (row < 0 || row >= d->molecules.size()) return QVariant();
  if (Qt::DecorationRole == role) return QIcon(renderMolecule(*(d->molecules[row])));
  if (Qt::DisplayRole == role) return d->molecules[row]->getName();
  return QVariant();
}

QMimeData *LibraryModel::mimeData(const QModelIndexList &indexes) const {
  qDebug("Preparing MIME data of molecules: %s",
         stringify<QModelIndex>(indexes,
                                [](const QModelIndex& i) {return QString::number(i.row());}).toUtf8().data());
  Q_D(const LibraryModel);
  QMimeData *result = new QMimeData;
  QByteArray xml;
  QXmlStreamWriter out(&xml);
  out.writeStartDocument();
  for (QModelIndex index : indexes) {
    int row = index.row();
    if (row >= 0 && row < d->molecules.size())
      out << *(d->molecules.at(row));
  }
  out.writeEndDocument();
  result->setData(moleculeMimeType, xml);
  return result;
}

void LibraryModel::setMolecules(QList<Molecule *> molecules) {
  qDebug("Setting molecules");
  Q_D(LibraryModel);
  beginResetModel();
  d->cleanMolecules();
  d->molecules = molecules;
  endResetModel();
}

void LibraryModel::addMolecule(Molecule *molecule) {
  qDebug("Adding molecule");
  Q_D(LibraryModel);
  beginInsertRows(QModelIndex(), d->molecules.size(), d->molecules.size());
  d->molecules << molecule;
  endInsertRows();
}

QStringList LibraryModel::mimeTypes() const {
  return  moleculeMimeTypes;
}

Qt::ItemFlags LibraryModel::flags(const QModelIndex &index) const {
  return QAbstractListModel::flags(index) | Qt::ItemIsDragEnabled;
}

} // namespace Molsketch
