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
#ifndef MOLSKETCH_BOUNDINGBOXLINKER_H
#define MOLSKETCH_BOUNDINGBOXLINKER_H

#include "abstractxmlobject.h"

#include <QPointF>
#include <QRectF>
#include <QScopedPointer>
#include <QtGlobal>

namespace Molsketch { // TODO turn on warnings for switch'es
  enum class Anchor : int {
    TopLeft = 0,
    Top = 1,
    TopRight = 2,
    Left = 4,
    BottomLeft = 8,
    Bottom = 9,
    Center = 5,
    Right = 6,
    BottomRight = 10
  };

  int convertAnchor(const Anchor& anchor);
  Anchor convertAnchor(const int& integer);
  Anchor anchorFromString(QString input);
  QString toString(const Anchor& anchor);
  QDebug operator<<(QDebug debug, const Anchor& anchor);

  class BoundingBoxLinkerPrivate;

  class BoundingBoxLinker : public abstractXmlObject {
    Q_DECLARE_PRIVATE(BoundingBoxLinker)
    QScopedPointer<BoundingBoxLinkerPrivate> d_ptr;
  public:
    explicit BoundingBoxLinker(Anchor originAnchor = Anchor::Center, Anchor ownAnchor = Anchor::Center, const QPointF& offset = QPointF());
    BoundingBoxLinker(const BoundingBoxLinker& other);
    virtual ~BoundingBoxLinker();
    QPointF getShift(const QRectF& reference, const QRectF& target) const;
    QString xmlName() const override;
    QXmlStreamAttributes xmlAttributes() const override;
    void readAttributes(const QXmlStreamAttributes& attributes) override;
    bool operator==(const BoundingBoxLinker& other) const;
    friend QDebug operator<<(QDebug debug, const BoundingBoxLinker& linker);
  };

  QDebug operator<<(QDebug debug, const BoundingBoxLinker& linker);

} // namespace Molsketch

#endif // MOLSKETCH_BOUNDINGBOXLINKER_H
