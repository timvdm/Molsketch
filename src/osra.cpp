#include "molecule.h"
#include "fileio.h"
#include "atom.h"

namespace Molsketch {

  Molecule*  call_osra(QString fileName)
  {
    int n=0;
    QString tmpresult=tmpnam(NULL);
    tmpresult+=".sdf";
    QString command;
    char *env=getenv("OSRA");
    if (env!=NULL)  command=env;
    else command="osra";
    command+=" -f sdf "+fileName+">"+tmpresult;
    system(command.toAscii());
    Molecule* mol = loadFile(tmpresult);
    if (mol) 
    {
      qreal x_avg=0,y_avg=0;
      foreach(Atom* atom, mol->atoms())
      {
        x_avg+=atom->x();
        y_avg+=atom->y();
        n++;
      }
      if (n>0)
      {
        x_avg/=n;
        y_avg/=n;
        foreach(Atom* atom, mol->atoms()) atom->setPos(atom->x()-x_avg,y_avg-atom->y());
      }
    }
    remove(tmpresult.toAscii());
    return mol;
  }

}
