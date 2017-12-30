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
#include "signalcounter.h"

#define SIGNAL_COUNTER_DEF(NAME, TYPE) NAME::NAME(QObject *parent) : QObject(parent) {} \
  void NAME::record(const TYPE& value) { addPayload(value); }

SIGNAL_COUNTER_DEF(QRealSignalCounter, qreal)
SIGNAL_COUNTER_DEF(BoolSignalCounter, bool)
SIGNAL_COUNTER_DEF(ColorSignalCounter, QColor)
SIGNAL_COUNTER_DEF(FontSignalCounter, QFont)

ModelIndexSignalCounter::ModelIndexSignalCounter(QObject *parent) : QObject(parent) {}

void ModelIndexSignalCounter::record(const QModelIndex &index, int start, int end) {
  addPayload(index, start, end);
}
