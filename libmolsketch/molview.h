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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

/** @file
 * This file is part of molsKetch and contains the MolView class.
 *
 * @author Harm van Eersel <devsciurus@xs4all.nl>
 * @since Hydrogen
 */

#ifndef molview_H
#define molview_H

#include <QGraphicsView>

#define MOUSEWHEELDIVIDER 120 // TODO change

namespace Molsketch {

/**
 * The view of the molecule scene. This is a subclass of QGraphicsView with
 * with zooming events added.
 *
 * @author Harm van Eersel
 */
class MolView : public QGraphicsView
  {
    Q_OBJECT

  public:
    /** Creates a new MolView.*/
    MolView(QGraphicsScene* scene);
    /*		void itemMoved();*/

    /** Scales the view with factor @p scaleFactor. */
    void scaleView(qreal scaleFactor);
  };

} // namespace
#endif
