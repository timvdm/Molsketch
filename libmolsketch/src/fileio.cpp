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
#include "reactionarrow.h"
#include "mechanismarrow.h"

#include "atom.h"
#include "bond.h"
#include "molecule.h"
#include "element.h"
#include "molscene.h"

namespace Molsketch
{

  QString svgLine(QLineF l, bool dashed = false, QString stroke = "#000000", qreal width = 1)
  {
    // Create the output object
    QString line;
    QTextStream out(&line);

    // Code a SVG line
    out << "<line x1=\"" << l.x1() << "\" y1=\"" << l.y1() << "\" x2=\"" << l.x2() << "\" y2=\"" << l.y2() << "\" stroke=\"" << stroke << "\" stroke-width=\"" << width << "\" ";

    // Check if the line is dotted
    if (dashed) out << "stroke-dasharray = \"9, 5\" ";

    // End and return the SVG tag
    out << "/>" << endl;
    return line;
  }

  QString svgPolygon(QLineF l, bool dashed = false)
  {
    // Create the output object
    QString polygon;
    QTextStream out(&polygon);

    // Get the needed positions
    QLineF lup = Bond::shiftVector(l,3);
    QLineF ldown = Bond::shiftVector(l,-3);

    // Code the SVG polygon
    out << "<polygon points=\"" <<  l.x1() << "," << l.y1() << " " << lup.x2() << "," << lup.y2() << " " << ldown.x2() << "," << ldown.y2() << "\" ";

    // Code the correct fill method
    if (dashed)
      out << "fill=\"url(#StripedGradient)\" stroke=\"none\" ";
    else
      out << "fill=\"black\" stroke=\"black\" ";

    // End and return the SVG tag
    out << "/>" << endl;
    return polygon;
  }

  QString svgDefs()
  {
    // Create the output object
    QString defs;
    QTextStream out(&defs);

    // Begin tag
    out << "<defs>" << endl;

    // Code the stripe pattern
    out << "<pattern id=\"StripedPattern\" x=\"0\" y=\"0\" width=\"100\" height=\"10\" patternUnits=\"userSpaceOnUse\" >" << endl;
    out << svgLine(QLineF(0,0,100,0));
    out << "</pattern>" << endl;

    // Code the stripe gradient
    /* TODO There should be a nicer way of drawing this */
    //     out << "<radialGradient id=\"StripedGradient\" r=\"5%\" spreadMethod=\"repeat\" >" << endl;
    out << "<linearGradient id=\"StripedGradient\" x2=\"5%\" spreadMethod=\"repeat\" >"  << endl;
    out << "<stop offset=\"0\" stop-color=\"white\" />" << endl;
    out << "<stop offset=\"0.3\" stop-color=\"white\" />" << endl;
    out << "<stop offset=\"0.5\" stop-color=\"black\" />" << endl;
    out << "<stop offset=\"0.7\" stop-color=\"white\" />" << endl;
    out << "<stop offset=\"1\" stop-color=\"white\" />" << endl;
    out << "</linearGradient>" << endl;
    //     out << "</radialGradient>" << endl;

    // End tag
    out << "</defs>" << endl;

    return defs;
  }

  bool saveToSVG( const QString & fileName, MolScene * scene )
  {
    // Trying to open a file with filename
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return false;

    // Making the writeobject
    QTextStream out(&file);

    // Writing the header
    out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl;
    out<< "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.0//EN\" \"http://www.w3.org/TR/2001/REC-SVG-20010904/DTD/svg10.dtd\">" << endl;
    out << "<svg version=\"1.1\"  width=\"" << scene->itemsBoundingRect().width() << "\" height=\"" << scene->itemsBoundingRect().height() << "\" viewBox=\"" <<  scene->itemsBoundingRect().left() << " " <<  scene->itemsBoundingRect().top() << " " << scene->itemsBoundingRect().width() << " " << scene->itemsBoundingRect().height() << "\">" << endl;

    out << svgDefs();

    // Writing the molecule
    for (int i = 0; i < scene->items().count(); i++)
    {
      if (scene->items().at(i)->type() == Molecule::Type)
      {
        Molecule* mol = (Molecule*)(scene->items().at(i));

        foreach (Bond* bond, mol->bonds())
        {
          // Get the bond and its attributes
          //               Bond* bond = mol->bond(j);
          QPointF p1 = bond->beginAtom()->scenePos();
          QPointF p2 = bond->endAtom()->scenePos();
          QLineF line = QLineF(p1,p2);

          // Check the type and order
          switch (bond->bondType())
          {
            case Bond::Wedge:
              out << svgPolygon(line);
              break;
            case Bond::InvertedWedge:
              out << svgPolygon(line);
              break;
            case Bond::Hash:
              out << svgPolygon(line,true);
              break;
            case Bond::InvertedHash:
              out << svgPolygon(line,true);
              break;
            case Bond::WedgeOrHash:
              out << svgLine(line,true);
              break;
            default:
              switch (bond->bondOrder())
              {
                case Bond::Single:
                  out << svgLine(line);
                  break;
                case Bond::Double:
                  out << svgLine(Bond::shiftVector(line,2));
                  out << svgLine(Bond::shiftVector(line,-2));
                  break;
                case Bond::Triple:
                  out << svgLine(Bond::shiftVector(line,3));
                  out << svgLine(Bond::shiftVector(line,-3));
                  out << svgLine(line);
                  break;
              }
          }
        }

        foreach (Atom* atom, mol->atoms())
        {
          //               Atom* atom = mol->atom(j);

          // Check if atom is visible and draw
          if (atom->isDrawn())
          {
            // Mask the bonds under the atom
            //                     out << "<mask>" << endl;
            out << "<circle cx=\"" << atom->scenePos().x() << "\" cy=\"" << atom->scenePos().y() << "\" r=\"" << 13 << "\" fill=\"#FFFFFF\"/>" << endl;
            //                     out << "</mask>" << endl;

            // Draw the element
            out << "<text x=\"" << atom->scenePos().x() << "\" y=\"" << atom->scenePos().y() + 5 << "\" font-family=\"Helvetica\" font-size=\"12\" fill=\"#000000\">";
            out << "<tspan text-anchor=\"middle\">" << atom->element();
            if (atom->numImplicitHydrogens()>0) out << "H";
            if (atom->numImplicitHydrogens()>1) out << "<tspan dy=\"4\" font-size=\"8\" >" << atom->numImplicitHydrogens() << "</tspan>"; // baseline-shift=\"sub\"
            out << "<tspan font-size=\"8\" dy=\"-10\" >" << atom->chargeString() << "</tspan>";
            out << "</tspan>"; //baseline-shift=\"super\"
            out << "</text>" << endl;
          }
        }

      }
    }

    // Writing the final tag and closing the file
    out << "</svg>" << endl;
    file.close();

    return true;
  }

  void writeMskFile(const QString &fileName, MolScene *scene) // TODO compare output to old version
  {
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
      return;

    QXmlStreamWriter xml(&file);
    xml.setAutoFormatting(true);
    xml.writeStartDocument();
    xml << *scene ;
    xml.writeEndDocument();
  }

  void readMskFile(const QString &fileName, MolScene *scene)
  {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
      return;

    QXmlStreamReader xml(&file);
    while (xml.name().toString() != scene->xmlName())
      xml.readNext() ;
    xml >> *scene ;

      //if (xml.name()

    if (xml.hasError()) {
      qDebug() << "ERROR while reading " << fileName;
      qDebug() << xml.errorString();
    }
  }


  // Molecule* smiles(QString formula)
  // {
  //     Molecule* mol = new Molecule();
  // //   QGraphicsScene scene;
  // //   scene.addItem(mol);
  //
  //     for (int i = 0; i < formula.length();i++)
  //     {
  //         if (formula.at( i ).isLetter())
  //             mol->addAtom( QString(formula.at( i )), QPoint(0,0) );
  //     }
  //
  //     return mol;
  // }

  bool exportFile(const QString &fileName, MolScene * scene)
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

}
