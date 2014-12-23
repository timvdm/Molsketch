/***************************************************************************
 *   Copyright (C) 2007 by Harm van Eersel                                 *
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
 * This file is part of molsKetch and contains the colorAction class.
 *
 * @author Hendrik Vennekate <HVennekate@gmx.de>
 * @since Lithium
 */

#ifndef COLORACTION_H
#define COLORACTION_H

#include "abstractitemaction.h"


namespace Molsketch {
  class graphicsItem ;

  class colorAction : public abstractItemAction
  {
    Q_OBJECT
  public:
    explicit colorAction(QObject *parent = 0);
  private:
    void execute() ;
  };
} // namespace

#endif // COLORACTION_H
