/***************************************************************************
 *   Copyright (C) 2015 Hendrik Vennekate                                  *
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
#ifndef MOLSKETCH_ROTATEACTION_H
#define MOLSKETCH_ROTATEACTION_H

#include "genericaction.h"

namespace Molsketch {
  class graphicsItem ;

  class transformAction : public genericAction
  {
    Q_OBJECT
  public:
    explicit transformAction(MolScene* scene);
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

  protected:
    virtual QTransform generateTransform(const QLineF& originalLine, const QLineF& currentLine) = 0;
    virtual void snapLine(const QLineF &originalLine, QLineF& currentLine) = 0;
    virtual QString cursorLabel(const QLineF &originalLine, QLineF& currentLine);
  private:
    class privateData;
    privateData *d;
  };

  class rotateAction : public transformAction
  {
    Q_OBJECT
  public:
    explicit rotateAction(MolScene* scene) ;
  private:
    QTransform generateTransform(const QLineF &originalLine, const QLineF &currentLine) override;
    void snapLine(const QLineF &originalLine, QLineF &currentLine) override;
    QString cursorLabel(const QLineF &originalLine, QLineF &currentLine) override;
  };

  class translateAction : public transformAction
  {
    Q_OBJECT
  public:
    explicit translateAction(MolScene* scene) ;
  private:
    QTransform generateTransform(const QLineF &originalLine, const QLineF &currentLine) override;
    void snapLine(const QLineF &originalLine, QLineF &currentLine) override;
  };

} // namespace Molsketch

#endif // MOLSKETCH_ROTATEACTION_H
