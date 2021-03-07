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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/
#ifndef OBABELIFACELOADER_H
#define OBABELIFACELOADER_H

#include <QObject>

class OBabelIfaceLoaderPrivate;
class QString;
class QGraphicsScene;

namespace Molsketch {
  class Molecule;
}

class OBabelIfaceLoader : public QObject
{
  Q_OBJECT
public:
  explicit OBabelIfaceLoader(QObject *parent = 0);
  ~OBabelIfaceLoader();
  QStringList inputFormats();
  QStringList outputFormats();
  Molsketch::Molecule* loadFile(const QString& filename);
  Molsketch::Molecule* callOsra(const QString filename);
  bool saveFile(const QString& fileName, QGraphicsScene* scene, bool use3d);
  Molsketch::Molecule* convertInChI(const QString& InChI);
  QVector<QPointF> optimizeCoordinates(const Molsketch::Molecule* molecule);

signals:
  void obabelIfaceAvailable(bool);
  void inchiAvailable(bool);
  void optimizeAvailable(bool);
  void obabelIfaceFileNameChanged(QString);

public slots:
  void reloadObabelIface(const QString& path);
  void setObabelFormats(const QString& folder);
private:
  Q_DECLARE_PRIVATE(OBabelIfaceLoader)
  OBabelIfaceLoaderPrivate* d_ptr;
};

#endif // OBABELIFACELOADER_H
