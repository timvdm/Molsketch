/***************************************************************************
 *   Copyright (C) 2017 Hendrik Vennekate                                  *
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
#ifndef SIGNALCOUNTER_H
#define SIGNALCOUNTER_H

#include <tuple>
#include <functional>
#include <QList>

template<typename ... Types>
class SignalCounter
{
private:
  QList<std::tuple<Types...>> payloads;
public:
  std::function<void(Types...)> callback;
  std::tuple<Types...> getLatestPayload() const {Q_ASSERT(!payloads.empty()); return payloads.last();}
  int count() const { return payloads.size(); }
  std::tuple<Types...> getPayload(int index) { Q_ASSERT(index >= 0 && index < payloads.size()) ; return payloads[index]; }
  void addPayload(Types... newPayload) {
    payloads << std::make_tuple(newPayload...);
    if (callback) callback(newPayload...);
  }
};

#endif // SIGNALCOUNTER_H
