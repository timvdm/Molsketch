/***************************************************************************
 *   Copyright (C) 2008 by Igor Filippov                                   *
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
#include "molecule.h"
#include "fileio.h"
#include "atom.h"

#include <QDesktopServices>
#include <QProcess>
#include <QDir>
#include <QFile>

namespace Molsketch {

  Molecule* call_osra(QString fileName)
  {
    int n=0;
    QString tmpresult = QDesktopServices::storageLocation(QDesktopServices::TempLocation) + QDir::separator() + "osra";
    tmpresult += ".sdf";
    QString command;
    char *env = getenv("OSRA");
    if (env != NULL)
      command = env;
    else 
      command = "osra";
    
    command += " -f sdf " + fileName + ">" + tmpresult;

    QStringList arguments;
    arguments << "-f";
    arguments << "sdf";
    arguments << fileName;
    arguments << ">";
    arguments << tmpresult;

    if (QProcess::execute(command, arguments))
      return 0;

    Molecule* mol = loadFile(tmpresult);
    if (mol) {
      qreal x_avg = 0, y_avg = 0;

      foreach(Atom* atom, mol->atoms()) {
        x_avg += atom->x();
        y_avg += atom->y();
        n++;
      }
      if (n > 0) {
        x_avg /= n;
        y_avg /= n;
        foreach(Atom* atom, mol->atoms()) 
          atom->setPos(atom->x() - x_avg, y_avg - atom->y());
      }
    }
    QFile::remove(tmpresult);
    return mol;
  }

}
