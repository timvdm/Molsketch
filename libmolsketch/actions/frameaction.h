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
#ifndef MOLSKETCH_FRAMEACTION_H
#define MOLSKETCH_FRAMEACTION_H

#include "multiaction.h"

namespace Molsketch {

  class FrameAction : public multiAction
  {
    Q_OBJECT
  public:
    static QString leftBracketFrame();
    static QString rightBracketFrame();
    static QString bracketsFrame() ;
    static QString angleFrame();
    static QString rectangleFrame();
    static QString roundedCornerRectangleFrame();
    static QString leftCurlyBracketFrame();
    static QString rightCurlyBracketFrame();
    static QString curlyBracketsFrame();

    explicit FrameAction(MolScene* scene);
    ~FrameAction();
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
  private:
    class privateData;
    privateData *d;
  };

} // namespace Molsketch

#endif // MOLSKETCH_FRAMEACTION_H
