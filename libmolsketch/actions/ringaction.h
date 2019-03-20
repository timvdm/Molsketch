/***************************************************************************
 *   Copyright (C) 2015 Hendrik Vennekate                                  *
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
#ifndef MOLSKETCH_RINGACTION_H
#define MOLSKETCH_RINGACTION_H

#include "multiaction.h"


namespace Molsketch {

  class Atom;
  class Bond;
  class Molecule;

  class ringAction : public multiAction
  {
    Q_OBJECT
  public:
    explicit ringAction(MolScene* scene);
    ~ringAction();

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void leaveSceneEvent(QEvent *event) override;
    void enterSceneEvent(QEvent *event) override;

  private:
    class privateData;
    privateData *d;
    QList<Atom *> getRingAtoms(Molecule *newMolecule);
    QList<Bond *> getRingBonds(QList<Atom*> ringAtoms, Molecule *newMolecule);
    void addAromaticity(QList<Bond*> ringBonds);
  private slots:
    void changeRing();
  };

} // namespace Molsketch

#endif // MOLSKETCH_RINGACTION_H
