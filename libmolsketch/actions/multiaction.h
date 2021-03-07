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
#ifndef MULTIACTION_H
#define MULTIACTION_H

#include "genericaction.h"

namespace Molsketch {
  class multiAction : public genericAction
  {
    Q_OBJECT
  public:
    explicit multiAction(MolScene *scene) ;
    ~multiAction() ;
    void wheelEvent(QGraphicsSceneWheelEvent *event) override;
    void cycleSubActions(bool inverse = false);
  protected:
    void addSubAction(QAction*) ;
    void addSeparator() ;
    QAction* activeSubAction() const ;
    QAction* firstSubAction() const;
  private:
    class privateData ;
    privateData *d ;
  private slots:
    void changeIcon();
    void checkActivation(QAction *subaction);
  };

} // namespace

#endif // MULTIACTION_H
