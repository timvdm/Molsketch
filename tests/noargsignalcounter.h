/***************************************************************************
 *   Copyright (C) 2017 by Hendrik Vennekate                               *
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

#ifndef NOARGSIGNALCOUNTER_H
#define NOARGSIGNALCOUNTER_H

#include <functional>
#include <QObject>

class NoArgSignalCounter : public QObject
{
  Q_OBJECT
public:
  explicit NoArgSignalCounter(QObject *parent = 0); // TODO constructor with signal argument
  int count;
  std::function<void()> callback;
public slots:
  void record();
};

// TODO output from QDebug; record senders

#endif // NOARGSIGNALCOUNTER_H
