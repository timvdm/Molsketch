/***************************************************************************
 *   Copyright (C) 2018 by Hendrik Vennekate                               *
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
#include "sumformula.h"

#include <QMap>
#include <QDebug>

namespace Molsketch {

  class ElementSymbol : public QString {
  public:
    ElementSymbol(const QString& other) : QString(other) {}
  };
}

template<>
bool qMapLessThanKey<Molsketch::ElementSymbol>(const Molsketch::ElementSymbol &a, const Molsketch::ElementSymbol &b) {
  if (a == b) return false;
  if (a == "C") return true;
  if (b == "C") return false;
  if (a == "H") return true;
  if (b == "H") return false;
  return a < b;
}

namespace Molsketch {
  struct SumFormulaPrivate {
    QMap<ElementSymbol, int> elementCounts;
    QString format(const QString &subscriptOpen = "", const QString &subscriptClose = "") const {
      QString result;
      for (auto element : elementCounts.keys()) {
        int count = elementCounts[element];
        if (count != 1) result += element + subscriptOpen + QString::number(count) + subscriptClose;
        else result += element;
      }
      return result;
    }
  };

  SumFormula::SumFormula(const QString& atomSymbol, int count)
    : SumFormula()
  {
    Q_D(SumFormula);
    d->elementCounts[atomSymbol] = count;
  }

  SumFormula::SumFormula(const std::initializer_list<SumFormula> &items)
    : SumFormula()
  {
    for (auto item : items)
      *this += item;
  }

  SumFormula::SumFormula(const SumFormula &other)
    : SumFormula()
  {
    Q_D(SumFormula);
    d->elementCounts = other.d_ptr->elementCounts;
  }

  SumFormula::SumFormula()
    : d_ptr(new SumFormulaPrivate)
  {}

  SumFormula::~SumFormula() {}

  SumFormula &SumFormula::operator+=(const SumFormula &other) {
    Q_D(SumFormula);
    for (auto element: other.d_ptr->elementCounts.keys())
      d->elementCounts[element] += other.d_ptr->elementCounts[element];
    return *this;
  }

  QString SumFormula::toHtml() const {
    Q_D(const SumFormula);
    return d->format("<sub>", "</sub>");
  }

  Molsketch::SumFormula::operator QString() const {
    Q_D(const SumFormula);
    return d->format();
  }

} // namespace Molsketch
