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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#include <QString>
#include <QStringList>

#include "element.h"

namespace Molsketch {
  
  QColor elementColor(int element)
  {
    //@todo: Add more colors....
    switch (element) {
      case Element::H:
        return QColor(200,200,200);
      case Element::C:
        return QColor(0, 0, 0);
      case Element::N:
        return QColor(0, 0, 255);
      case Element::O:
        return QColor(255, 0, 0);
      default:
        return QColor(0, 0, 0);    
    }  
  }

  int elementGroup(int element)
  {
    switch (element) {
      case Element::Dummy:
      case -1:
        return 0;
      case Element::H: 
      case Element::Li:
      case Element::Na:
      case Element::K: 
      case Element::Rb: 
      case Element::Cs: 
      case Element::Fr: 
        return 1;
      case Element::Be: 
      case Element::Mg: 
      case Element::Ca: 
      case Element::Sr: 
      case Element::Ba: 
      case Element::Ra: 
        return 2;
      case Element::Ti: 
      case Element::Zr: 
      case Element::Hf: 
      case Element::Rf: 
        return 4;
      case Element::V:
      case Element::Nb:
      case Element::Ta:
      case Element::Db:
        return 5;
      case Element::Cr:
      case Element::Mo:
      case Element::W:
      case Element::Sg:
        return 6;
      case Element::Mn:
      case Element::Tc:
      case Element::Re:
      case Element::Bh:
        return 7;
      case Element::Fe:
      case Element::Ru:
      case Element::Os:
      case Element::Hs:
        return 8;
      case Element::Co:
      case Element::Rh:
      case Element::Ir:
      case Element::Mt:
        return 9;
      case Element::Ni:
      case Element::Pd:
      case Element::Pt:
      case Element::Ds:
        return 10;
      case Element::Cu:
      case Element::Ag:
      case Element::Au:
      case Element::Rg:
        return 11;
      case Element::Zn:
      case Element::Cd:
      case Element::Hg:
      case Element::Uub:
        return 12;
      case Element::B: 
      case Element::Al: 
      case Element::Ga: 
      case Element::In: 
      case Element::Tl: 
      case Element::Uut:
        return 13;
      case Element::C: 
      case Element::Si: 
      case Element::Ge: 
      case Element::Sn: 
      case Element::Pb: 
        return 14;
      case Element::N: 
      case Element::P: 
      case Element::As: 
      case Element::Sb: 
      case Element::Bi: 
        return 15;
      case Element::O: 
      case Element::S: 
      case Element::Se: 
      case Element::Te: 
      case Element::Po: 
        return 16; 
      case Element::F: 
      case Element::Cl: 
      case Element::Br: 
      case Element::I: 
      case Element::At: 
        return 17;
      case Element::He:
      case Element::Ne:
      case Element::Ar:
      case Element::Kr:
      case Element::Xe:
      case Element::Rn:
        return 18;
      default:
        return 3; // Lanthanides & Actinides
    }

  }

  int numValenceElectrons(int element)
  {
    switch (elementGroup(element)) {
      case 0:
        return 0;
      case 1:
        return 1;
      case 2:
        return 2;
      case 3:
        return 3;
      case 4:
        return 4;
      case 5:
        return 5;
      case 6:
        return 6;
      case 7:
        return 7;
      case 8:
        return 8;
      case 9:
        return 9;
      case 10:
        return 10;
      case 11:
        return 11;
      case 12:
        return 2;
      case 13:
        return 3;
      case 14:
        return 4;
      case 15:
        return 5;
      case 16:
        return 6;
      case 17:
        return 7;
      case 18:
        // special case
        if (element == Element::He)
          return 2;
        return 8;
      default:
        return 8; // makes charge 0
    }

  }

  int expectedValence(int element)
  {
    switch (elementGroup(element)) {
      case 0:
        return 0;
      case 1:
        return 1;
      case 2:
        return 2;
      case 13:
        return 3;
      case 14:
        return 4;
      case 15:
        return 3;
      case 16:
        return 2;
      case 17:
        return 1;
      case 18:
        return 0;
      case 3:
        return 3;
      case 4:
        return 4;
      case 5:
        return 5;
      case 6:
        return 3;
      case 7:
        return 2;
      case 8:
        return 2;
      case 9:
        return 2;
      case 10:
        return 2;
      case 11:
        return 1;
      case 12:
        return 2;
      default:
        return 0;
    }
  }


}
