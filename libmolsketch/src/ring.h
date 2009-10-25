/***************************************************************************
 *   Copyright (C) 2009 by Tim Vandermeersch                               *
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

#ifndef MSK_RING_H
#define MSK_RING_H

#include "atom.h"

namespace Molsketch {

  class Molecule;

  /**
   * Ring class
   */
  class Ring
  {
    friend class Molecule;
    public:
      QList<Atom*> atoms() const
      {
        return m_atoms;
      }

      int size() const 
      {
        return m_atoms.size();
      }

      QPointF center() const
      {
        QPointF c(0.0, 0.0);
        foreach (Atom *atom, m_atoms)
          c += atom->pos();
        c /= m_atoms.size();
        return c;
      }
    protected:
      Ring() {}

      void setAtoms(const QList<Atom*> &atoms)
      {
        m_atoms = atoms;
      }


    private:
      QList<Atom*> m_atoms;
  };

} // namespace

#endif
