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
 * This file is part of molsKetch and contains the PeriodicTableWidget class.
 *
 * @author Harm van Eersel <devsciurus@xs4all.nl>
 * @since Deuterium
 */

#ifndef PERIODICTABLEWIDGET_H
#define PERIODICTABLEWIDGET_H

#include <QTableWidget>

class QSize;

/**
 * This class implements a periodic table widget, used for element selection.
 *
 * @author Harm van Eersel <devsciurus@xs4all.nl>
 */
class PeriodicTableWidget : public QTableWidget
{
 Q_OBJECT

 public:
  /**
   * Constructor
   */
  PeriodicTableWidget();

  /** Returns the size hint for the table */
  virtual QSize sizeHint() const;

 protected:
  /** Stores the previous selection */
  QTableWidgetItem * m_previousItem;

 private slots:
  /** Checks whether the new selection is a valid one */
  void checkSelection(QTableWidgetItem * item);

  
};

#endif
