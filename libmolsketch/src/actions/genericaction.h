/***************************************************************************
 *   Copyright (C) 2015 Hendrik Vennekate                                  *
 *      --- in part based on code originally written by: ---               *
 *   Copyright (C) 2007-2008 by Harm van Eersel                            *
 *   Copyright (C) 2009 Tim Vandermeersch                                  *
 *   Copyright (C) 2009 by Nicola Zonta                                    *
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
#ifndef GENERICACTION_H
#define GENERICACTION_H

#include <QAction>

class QGraphicsSceneMouseEvent ;
class QUndoStack ;
class QUndoCommand;

namespace Molsketch {

  class MolScene ;

  class genericAction : public QAction
  {
    Q_OBJECT
  public:
    explicit genericAction(MolScene* scene);
    virtual ~genericAction() {}

    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) { Q_UNUSED(event) }
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event) { Q_UNUSED(event) }
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) { Q_UNUSED(event) }
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) { Q_UNUSED(event) }
    virtual void leaveSceneEvent(QEvent *event) { Q_UNUSED(event) }
    virtual void enterSceneEvent(QEvent *event) { Q_UNUSED(event) }
  protected:
    MolScene *scene() const ;
    QUndoStack* undoStack() const ;
    bool eventFilter(QObject *object, QEvent *event);
    void attemptUndoPush(QUndoCommand* command) ;
    void attemptBeginMacro(const QString& text);
    void attemptEndEndMacro();
  private slots:
    void activationSlot(const bool& b) ;
  };

} // namespace

#endif // ABSTRACTACTION_H
