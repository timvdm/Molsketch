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
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef MODELINDEXSIGNALCOUNTER_H
#define MODELINDEXSIGNALCOUNTER_H

#include <QObject>
#include <QModelIndex>
#include <signalcounter.h>

class ModelIndexSignalCounter : public QObject, public SignalCounter<QModelIndex, int, int>
{
  Q_OBJECT
public:
  explicit ModelIndexSignalCounter(QObject *parent = 0);
public slots:
  void record(const QModelIndex& index, int start, int end);
};

#endif // MODELINDEXSIGNALCOUNTER_H
