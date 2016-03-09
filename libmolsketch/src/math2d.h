/***************************************************************************
 *   Copyright (C) 2009 Tim Vandermeersch                                  *
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

#ifndef MSK_MATH2D_H
#define MSK_MATH2D_H

#include <QtMath>

namespace Molsketch {

  inline qreal length(const QPointF &v)
  {
    return sqrt(v.x() * v.x() + v.y() * v.y());
  }

  inline QPointF normalized(const QPointF &v)
  {
    QPointF n = v;
    qreal length = sqrt(n.x() * n.x() + n.y() * n.y());
    n /= length;
    return n;
  }

  inline qreal angle(const QPointF &v1, const QPointF &v2) 
  {
    QPointF n1 = normalized(v1);
    QPointF n2 = normalized(v2);

    qreal dot = n1.x() * n2.x() + n1.y() * n2.y();
    return acos(dot);
  }

  inline qreal angleSign(const QPointF &v1, const QPointF &v2)
  {
    return v1.x() * v2.y() - v1.y() * v2.x();
  }

  inline double triangleSign(const QPointF &a, const QPointF &b, const QPointF &c)
  {
    return (a.x() - c.x()) * (b.y() - c.y()) - (a.y() - c.y()) * (b.x() - c.x());  
  }

  /*
  QPointF intersectionPoint(const QLineF &line1, const QLineF &line2)
  {
    return QPointF( ((line2.x2() - line2.x1()) * (line1.y1() - line2.y1()) - (line2.y2() - line2.y1()) * (line1.x1() - line2.x1())) / 
                    ((line2.y2() - line2.y1()) * (line1.x2() - line1.x1()) - (line2.x2() - line2.x1()) * (line1.y2() - line1.y1())) ,
                    ((line1.x2() - line1.x1()) * (line1.y1() - line2.y1()) - (line1.y2() - line1.y1()) * (line1.x1() - line2.x1())) / 
                    ((line2.y2() - line2.y1()) * (line1.x2() - line1.x1()) - (line2.x2() - line2.x1()) * (line1.y2() - line1.y1())) );
  }
  */

}

#endif
