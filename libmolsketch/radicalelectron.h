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
#ifndef MOLSKETCH_RADICALELECTRON_H
#define MOLSKETCH_RADICALELECTRON_H

#include <QGraphicsItem>
#include <QtGlobal>
#include "boundingboxlinker.h"

namespace Molsketch {

  class RadicalElectronPrivate;

  class RadicalElectron : public QGraphicsItem, public abstractXmlObject {
    Q_DECLARE_PRIVATE(RadicalElectron)
    QScopedPointer<RadicalElectronPrivate> d_ptr;
  public:
    explicit RadicalElectron(qreal diameter = 2, BoundingBoxLinker linker = BoundingBoxLinker(Anchor::Top, Anchor::Bottom), const QColor& color = QColor());
    RadicalElectron(const RadicalElectron& other);
    ~RadicalElectron();
    qreal diameter() const;
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    BoundingBoxLinker linker() const;
    bool operator==(const RadicalElectron& other);
    QString xmlName() const;
    friend QDebug operator<<(QDebug debug, const RadicalElectron& radicalElectron);
  protected:
    void readAttributes(const QXmlStreamAttributes &attributes) override;
    QXmlStreamAttributes xmlAttributes() const override;
    QList<const XmlObjectInterface *> children() const override;
    XmlObjectInterface *produceChild(const QString &name, const QString &type) override;
  };
} // namespace Molsketch

#endif // MOLSKETCH_RADICALELECTRON_H
