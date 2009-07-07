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

/** @file
 * This file is part of molsKetch and contains routines for loading and 
 * saving files.
 *
 * @author Harm van Eersel
 * @since Hydrogen
 */

 
class MolScene;
class Molecule;

/**
 * Load and save routines
 *
 * @author Harm van Eersel
 */
namespace molsKetch
{
/** 
 * Loads file with @p fileName and returns it as pointer to a new Molecule 
 * object. 
 */
Molecule* loadFile(const QString &fileName);
/** 
 * Loads file with @p fileName and returns it as pointer to a new Molecule 
 * object. 
 */
Molecule* loadFile3D(const QString &fileName);
/** 
 * Saves the current document under @p fileName and returns @c false if the
 * save failed.
 */
bool saveFile(const QString &fileName, QGraphicsScene * scene);
/** 
 * Saves the current document with coordinates suitable for 3D applications
 * under @p fileName and returns @c false if the save failed.
 */
bool saveFile3D(const QString &fileName, QGraphicsScene * scene);
/** 
 * Exports the document on MolScene @p scene as a bitmap with @p fileName 
 * and returns @c false if the export failed.
 */
bool exportFile(const QString &fileName, MolScene * scene);
/** 
 * Prints the document on MolScene @p scene on QPrinter @p printer and 
 * returns @c false if the print failed.
 */
bool printFile(QPrinter &printer, MolScene * scene);
/** 
 * Exports the current document on MolScene @p scene under @p fileName and 
 * returns @c false if the export failed.
 */
bool saveToSVG(const QString &fileName, MolScene * scene);

// Molecule* smiles(QString formula);

}
