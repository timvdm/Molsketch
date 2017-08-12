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
#include "obabelifaceloader.h"
#include "obabeliface.h"

#include <QDebug>
#include <QLibrary>

static const char BABEL_LIBDIR_VARIABLE[] = "BABEL_LIBDIR";

class OBabelIfaceLoaderPrivate {
  Q_DISABLE_COPY (OBabelIfaceLoaderPrivate)
  public:
  OBabelIfaceLoaderPrivate (OBabelIfaceLoader* parent) : parent (parent) {}
  OBabelIfaceLoader* parent;
  QLibrary openBabel;
  QString openBabelFormatsPath;
  Molsketch::loadFileFunctionPointer load;
  Molsketch::saveFileFunctionPointer save;
  Molsketch::smilesFunctionPointer toSmiles;
  Molsketch::fromSmilesFunctionPointer fromSmiles;
  Molsketch::formatsFunctionPointer inputFormats;
  Molsketch::formatsFunctionPointer outputFormats;
  Molsketch::fromInChIFunctionPointer fromInChI;
  Molsketch::formatAvailablePointer inChIAvailable;
  Molsketch::callOsraFunctionPointer callOsra;
  void loadFunctions () {
    load = (Molsketch::loadFileFunctionPointer)openBabel.resolve (
      "loadFile"); // TODO put into obabeliface.h -- question: what happens when lib is dynamically loaded (with its constants etc.)?
    save = (Molsketch::saveFileFunctionPointer)openBabel.resolve ("saveFile");
    toSmiles = (Molsketch::smilesFunctionPointer)openBabel.resolve ("smiles");
    fromSmiles = (Molsketch::fromSmilesFunctionPointer)openBabel.resolve ("fromSmiles");
    inputFormats = (Molsketch::formatsFunctionPointer)openBabel.resolve ("inputFormats");
    outputFormats = (Molsketch::formatsFunctionPointer)openBabel.resolve ("outputFormats");
    fromInChI = (Molsketch::fromInChIFunctionPointer)openBabel.resolve ("fromInChI");
    inChIAvailable = (Molsketch::formatAvailablePointer)openBabel.resolve ("inChIAvailable");
    callOsra = (Molsketch::callOsraFunctionPointer)openBabel.resolve("call_osra");
  }
  void unloadFunctions () { // TODO check if this is really necessary
  }
  void emitSignals () {
    bool openBabelAvailable = openBabel.isLoaded ();
    bool isInchiAvailable = inChIAvailable && inChIAvailable ();
    qDebug() << "OpenBabel available: " + QString::number (openBabelAvailable);
    qDebug() << "InChI availabel: " + QString::number (isInchiAvailable);
    emit parent->obabelIfaceAvailable (openBabelAvailable);
    emit parent->inchiAvailable (isInchiAvailable);
    emit parent->obabelIfaceFileNameChanged(openBabel.fileName());
  }
};

OBabelIfaceLoader::OBabelIfaceLoader (QObject* parent) : QObject (parent), d_ptr (new OBabelIfaceLoaderPrivate (this)) {}

OBabelIfaceLoader::~OBabelIfaceLoader () { delete d_ptr; }

QStringList OBabelIfaceLoader::inputFormats () {
  Q_D (OBabelIfaceLoader);
  if (d->inputFormats) return d->inputFormats ();
  qWarning ("No OpenBabel formats available for input");
  return QStringList ();
}

QStringList OBabelIfaceLoader::outputFormats () {
  Q_D (OBabelIfaceLoader);
  if (d->outputFormats) return d->outputFormats();
  qWarning ("No OpenBabel formats available for output");
  return QStringList ();
}

Molsketch::Molecule* OBabelIfaceLoader::loadFile (const QString& filename) {
  Q_D (OBabelIfaceLoader);
  if (d->load) return d->load (filename);
  qWarning ("No OpenBabel support available for loading file");
  return nullptr;
}

Molsketch::Molecule *OBabelIfaceLoader::callOsra(const QString filename) {
  Q_D(OBabelIfaceLoader);
  if (d->callOsra) return d->callOsra(filename);
  return nullptr;
}

bool OBabelIfaceLoader::saveFile (const QString& fileName, QGraphicsScene* scene, bool use3d) {
  Q_D (OBabelIfaceLoader);
  if (d->save) return d->save (fileName, scene, use3d ? 3 : 2);
  qWarning ("No support for saving OpenBabel available");
  return false;
}

void OBabelIfaceLoader::reloadObabelIface (const QString& path) {
  Q_D (OBabelIfaceLoader);
  d->openBabel.unload ();
  d->openBabel.setFileName (path);
  d->loadFunctions ();
  d->emitSignals ();
}

void OBabelIfaceLoader::setObabelFormats (const QString& folder) {
  Q_D (OBabelIfaceLoader);
  d->openBabelFormatsPath = folder;
  qputenv (BABEL_LIBDIR_VARIABLE, folder.toUtf8 ()); // TODO: Latin1?
  d->emitSignals ();
}
