/***************************************************************************
 *   Copyright (C) 2016 Hendrik Vennekate                                  *
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
#ifndef MOLSKETCH_HELPTEXTEMPTYDOCK_H
#define MOLSKETCH_HELPTEXTEMPTYDOCK_H

#include "helptextonemptylabel.h"

namespace Molsketch {

  class HelpTextEmptyDock : public HelpTextOnEmptyLabel
  {
    Q_OBJECT
  public:
    explicit HelpTextEmptyDock(const QString& helpText, QWidget *parent = 0);
  protected:
    bool isEmpty() const override;
  };

} // namespace Molsketch

#endif // MOLSKETCH_HELPTEXTEMPTYDOCK_H
