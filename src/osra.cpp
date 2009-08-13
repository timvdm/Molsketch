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
