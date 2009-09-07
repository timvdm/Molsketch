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

//#include <openbabel/obiter.h>
#include <openbabel/mol.h>
#include <openbabel/obconversion.h>

#include "fileio.h"

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


  bool saveFile(const QString &fileName, QGraphicsScene* scene)
  {
    using namespace OpenBabel;
    OBConversion conversion;

    if (!conversion.SetOutFormat(QFileInfo(fileName).suffix().toAscii())) {
      qDebug() << "Error while saving #1";
      return false;
    }

    // Create the output molecule
    OBMol obmol;
    obmol.SetDimension(2);

    // Add all molecules on the scene
    foreach(QGraphicsItem* item, scene->items()) {
      if (item->type() == Molecule::Type) {
        Molecule* mol = static_cast<Molecule*>(item);

        QHash<Atom*,OpenBabel::OBAtom*> hash;

        obmol.BeginModify();
        // creating OBAtoms...
        qDebug() << "Creating OBAtom objects...";
        foreach (Atom* atom, mol->atoms()) {
          OpenBabel::OBAtom* obatom = obmol.NewAtom();
          obatom->SetVector(atom->scenePos().x()/40,atom->scenePos().y()/40,0);
          std::string element = atom->element().toStdString();
          obatom->SetAtomicNum(Molsketch::symbol2number(atom->element()));
          hash.insert(atom,obatom);
          qDebug() << "  " << obatom->GetIdx() << "=" << obatom;
        }

        foreach (Bond* bond, mol->bonds()) {
          Atom* a1 = bond->beginAtom();
          Atom* a2 = bond->endAtom();

          unsigned int beginIdx = hash.value(a1)->GetIdx();
          unsigned int endIdx = hash.value(a2)->GetIdx();
          unsigned int swapIdx = beginIdx;
          int flags = 0;

          // Setting bondtype
          switch (bond->bondType()) {
            case Bond::Wedge:
              flags |= OB_WEDGE_BOND;
              break;
            case Bond::InvertedWedge:
              flags |= OB_WEDGE_BOND;
              beginIdx = endIdx;
              endIdx = swapIdx;
              break;
            case Bond::Hash:
              flags |= OB_HASH_BOND;
              break;
            case Bond::InvertedHash:
              flags |= OB_HASH_BOND;
              beginIdx = endIdx;
              endIdx = swapIdx;
              break;
            default:
              break;
          }
          obmol.AddBond(beginIdx, endIdx, bond->bondOrder(), flags);

        }
        obmol.EndModify();
      }
    }

    // Checking if the file exists and making a backup
    if (QFile::exists(fileName)) 
    {
      QFile::remove(fileName + "~");
      QFile::copy(fileName,fileName + "~");
    }

    // Writing the final result to the file
    conversion.WriteFile(&obmol,fileName.toStdString());

    return true;
  }

  bool saveFile3D(const QString &fileName, QGraphicsScene* scene)
  {
    //     QMessageBox::warning(this,tr(PROGRAM_NAME),tr("Saving is only partially implemented. You may lose data if you overwrite an existing file."),QMessageBox::Ok,QMessageBox::Ok);
    using namespace OpenBabel;
    OBConversion * conversion = new OBConversion;

    if (conversion->SetOutFormat(QFileInfo(fileName).suffix().toAscii()))
    {
      // Create the output molecule
      OBMol* obmol = new OBMol;
      QHash<Atom*,OpenBabel::OBAtom*> hash;

      // Add all molecules on the scene
      foreach(QGraphicsItem* item, scene->items())
      {
        if (item->type() == Molecule::Type)
        {
          Molecule* mol = dynamic_cast<Molecule*>(item);

          hash.clear();

          obmol->BeginModify();
          //                 obmol->ReserveAtoms(mol->countAtoms());
          foreach (Atom* atom, mol->atoms())
          {
            OpenBabel::OBAtom* obatom = obmol->NewAtom();
            //                 Atom* atom = mol->atom(j);
            obatom->SetVector(atom->scenePos().x(),-atom->scenePos().y(),0);
            std::string element = atom->element().toStdString();
            //                 obatom->SetType(element);
            obatom->SetAtomicNum(Molsketch::symbol2number(atom->element()));
            //                 obmol->AddAtom(*obatom);
            hash.insert(atom,obatom);
            //                 cerr << hash.count() << "\n";
          }
          foreach (Bond* bond, mol->bonds())
          {
            //                 Bond* bond = mol->bonds[j];
            Atom* a1 = bond->beginAtom();
            Atom* a2 = bond->endAtom();

            OBAtom* oba1 = hash.value(a1);
            OBAtom* oba2 = hash.value(a2);

            OpenBabel::OBBond* obbond = new OpenBabel::OBBond();
            //                 OBBond* obbond = obmol->NewBond();

            // Set identifier
            //                 obbond->SetIdx(j);

            // Set bondorder
            obbond->SetBO(bond->bondOrder());

            // Setting bondtype
            switch (bond->bondType())
            {
              case Bond::Wedge:
                obbond->SetWedge();
              case Bond::InvertedWedge:
                obbond->SetBegin(oba2);
                obbond->SetEnd(oba1);
                obbond->SetWedge();
                break;
              case Bond::Hash:
                obbond->SetHash();
              case Bond::InvertedHash:
                obbond->SetBegin(oba2);
                obbond->SetEnd(oba1);
                obbond->SetHash();
                break;
              default:
                obbond->SetBegin(oba1);
                obbond->SetEnd(oba2);
            }

            // Adding the bond
            //                 obmol->AddBond(oba1->GetIdx(),oba2->GetIdx(),bond->getOrder());

            obmol->AddBond(*obbond);

          }
          if(dynamic_cast<MolScene*>(scene) && dynamic_cast<MolScene*>(scene)->autoAddHydrogen())
            obmol->AddHydrogens();
          obmol->EndModify();
        }
      }

      // Checking if the file exists and making a backup
      if (QFile::exists(fileName)) 
      {
        QFile::remove(fileName + "~");
        QFile::copy(fileName,fileName + "~");
      }

      // Writing the final result to the file
      conversion->WriteFile(obmol,fileName.toStdString());
      //       qDebug << "File saved: " << fileName.toStdString() << "\n";
    }
    else
    {
      return false;
    }

    return true;
  }

  Molecule* loadFile(const QString &fileName)
  {
    // Creating and setting conversion classes
    using namespace OpenBabel;
    OBConversion * conversion = new OBConversion;
    conversion->SetInFormat(conversion->FormatFromExt(fileName.toAscii()));
    OBMol obmol;

    // Try to load a file
    if (!conversion->ReadFile(&obmol, fileName.toStdString())) {
      return false;
    }

    // Create a new molecule
    Molecule* mol = new Molecule();
    mol->setPos(QPointF(0,0));

    //       if(dynamic_cast<MolScene*>(scene) && dynamic_cast<MolScene*>(scene)->autoAddHydrogen())
    //         obmol->AddHydrogens();
    //       else
    //         obmol->DelHydrogens();
    //         cerr << "mol " << "x: " << mol->pos().x() << " y: " << mol->pos().y() << "\n";

    // Initialize normalization factor
    //       qreal factor = 1;
    // molfile.GetInternalCoord(0,0,0);

    // Add atoms one-by-ons
    for (unsigned int i = 1; i <= obmol.NumAtoms();i++)
      // 	FOR_ATOMS_OF_MOL(obatom,obmol)
    {
      OpenBabel::OBAtom *obatom = obmol.GetAtom(i);
      //  			scene->addRect(QRectF(atom->GetX(),atom->GetY(),5,5));
      //           Atom* atom =
      mol->addAtom(Molsketch::number2symbol(obatom->GetAtomicNum()),QPointF(obatom->x()*40,obatom->y()*40), false);

    }

    // Add bonds one-by-one
    /// Mind the numbering!
    for (unsigned int i = 0; i < obmol.NumBonds(); ++i) {
      // Loading the OpenBabel objects
      OpenBabel::OBBond *obbond = obmol.GetBond(i);
      OBAtom *a1 = obbond->GetBeginAtom();
      OBAtom *a2 = obbond->GetEndAtom();

      // Creating their internal counterparts
      Atom* atomA = mol->atomAt(QPointF(a1->x()*40,a1->y()*40));
      Atom* atomB = mol->atomAt(QPointF(a2->x()*40,a2->y()*40));
      Bond* bond  = mol->addBond(atomA,atomB,obbond->GetBondOrder());

      // Set special bond types
      if (obbond->IsWedge()) 
        bond->setType( Bond::Wedge );
      if (obbond->IsHash()) 
        bond->setType( Bond::Hash );
    }

    return mol;
  }
  
  return 0;
}

  Molecule* loadFile3D(const QString &fileName)
  {
    // Creating and setting conversion classes
    using namespace OpenBabel;
    OBConversion * conversion = new OBConversion;
    conversion->SetInFormat(conversion->FormatFromExt(fileName.toAscii()));
    OBMol obmol;

    // Try to load a file
    if (conversion->ReadFile(&obmol, fileName.toStdString()))
    {
      // Create a new molecule
      Molecule* mol = new Molecule();
      mol->setPos(QPointF(0,0));

      //       if(dynamic_cast<MolScene*>(scene) && dynamic_cast<MolScene*>(scene)->autoAddHydrogen())
      //         obmol->AddHydrogens();
      //       else
      //         obmol->DelHydrogens();
      //         cerr << "mol " << "x: " << mol->pos().x() << " y: " << mol->pos().y() << "\n";

      // Initialize normalization factor
      //       qreal factor = 1;
      // molfile.GetInternalCoord(0,0,0);

      // Add atoms one-by-ons
      for (unsigned int i = 1; i <= obmol.NumAtoms();i++)
        // 	FOR_ATOMS_OF_MOL(obatom,obmol)
      {
        OpenBabel::OBAtom *obatom = obmol.GetAtom(i);
        //  			scene->addRect(QRectF(atom->GetX(),atom->GetY(),5,5));
        //           Atom* atom =
        mol->addAtom(Molsketch::number2symbol(obatom->GetAtomicNum()),QPointF(obatom->x(),-obatom->y()), false);

      }

      // Add bonds one-by-one
      /// Mind the numbering!
      for (unsigned int i = 0; i < obmol.NumBonds();i++)
        // 	FOR_BONDS_OF_MOL(obbond,obmol)
      {
        // Loading the OpenBabel objects
        OpenBabel::OBBond *obbond = obmol.GetBond(i);
        OBAtom *a1 = obbond->GetBeginAtom();
        OBAtom *a2 = obbond->GetEndAtom();

        // Creating their internal counterparts
        Atom* atomA = mol->atomAt(QPointF(a1->x(),-a1->y()));
        Atom* atomB = mol->atomAt(QPointF(a2->x(),-a2->y()));
        Bond* bond  = mol->addBond(atomA,atomB,obbond->GetBondOrder());

        // Set special bond types
        if (obbond->IsWedge())
          bond->setType( Bond::Wedge );
        if (obbond->IsHash()) 
          bond->setType( Bond::Hash );

        // Normalizing
        //             factor = scene->getBondLength()/obbond->GetLength();
      }

      // // Normalizing molecule
      // mol->scale(factor,factor);
      // mol->setAtomSize(LABEL_SIZE/factor);

      return mol;
    }
    else
    {
      return 0;
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
