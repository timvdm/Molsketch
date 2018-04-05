/***************************************************************************
 *   Copyright (C) 2007-2008 by Harm van Eersel                            *
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

#include <QtGui>
#include <QtSvg>

#include "fileio.h"
#include "atom.h"
#include "bond.h"
#include "molecule.h"
#include "element.h"
#include "molscene.h"

namespace Molsketch
{

  bool saveToSVG( const QString & fileName, MolScene * scene )
  {
    // Trying to open a file with filename
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return false;

    file.write(scene->toSvg());
    file.close();

    return true;
  }

  bool writeMskFile(const QString &fileName, abstractXmlObject *xmlObject) // TODO compare output to old version // TODO turn into static member function
  {
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
      qCritical() << "Could not open file to write: " + fileName;
      return false;
    }

    QXmlStreamWriter xml(&file);
    xml.setAutoFormatting(true);
    xml.writeStartDocument();
    xml << *xmlObject ;
    xml.writeEndDocument();
    qDebug() << "File written: " + fileName;
    return true;
  }

  void readMskFile(const QString &fileName, MolScene *scene) // TODO this should be a static member of MolScene and create the scene from the file
  {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
      return;

    QXmlStreamReader xml(&file);
    while (xml.name().toString() != scene->xmlName() && xml.name().toString() != "div") // TODO div is only for old versions
      xml.readNext() ;
    xml >> *scene ;
    if (xml.hasError()) {
      qDebug() << "ERROR while reading " << fileName;
      qDebug() << xml.errorString();
    }
  }

  bool exportFile(const QString &fileName, MolScene * scene) // TODO make part of scene
  {
    // Clear selection
    QList<QGraphicsItem*> selList(scene->selectedItems());
    scene->clearSelection();

    QImage image = scene->renderImage(scene->itemsBoundingRect());

    // Restore selection
    foreach(QGraphicsItem* item, selList) item->setSelected(true);

    return image.save(fileName);
  }


  bool printFile(QPrinter &printer, MolScene * scene)
  {
    // Creating the painter
    QPainter painter(&printer);

    // Clear selection
    QList<QGraphicsItem*> selList(scene->selectedItems());
    scene->clearSelection();

    // Rendering on the printer
    QRectF rect(scene->itemsBoundingRect());
    scene->render(&painter,printer.pageRect(),rect);

    // Restore selection
    foreach(QGraphicsItem* item, selList) item->setSelected(true);

    return true;
  }

  QList<Molecule *> moleculesFromFile(const QString &fileName)
  {
    QList<Molecule*> result;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
      return result;

    QXmlStreamReader xml(&file);
    while (xml.readNextStartElement())
    {
      if(xml.name() != "molecule") continue;
      Molecule *m = new Molecule;
      m->readXml(xml);
      result << m;
    }

    if (xml.hasError()) {
      qDebug() << "ERROR while reading " << fileName;
      qDebug() << xml.errorString();
    }

    return result;
  }

}
