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
#ifndef PROPERTYLISTENER_H
#define PROPERTYLISTENER_H

#include <QWidget>

class QUndoCommand;

namespace Molsketch {

  class MolScene;

  class graphicsItem;

  class PropertiesWidget : public QWidget {
    Q_OBJECT
  private:
    class PrivateData;
    PrivateData *d;
  protected:
    virtual void propertiesChanged() = 0;
    void attemptToPushUndoCommand(QUndoCommand*cmd);
    MolScene *scene() const;
    bool blocked() const;
    void attemptBeginMacro(const QString &text) const;
    void attemptEndMacro() const;
    bool itemValid(graphicsItem *item) const;
  public:
    explicit PropertiesWidget(QWidget *parent = 0);
    virtual ~PropertiesWidget();
    void setScene(MolScene* scene);
  public slots:
    void propertiesChange();
  private slots:
    void sceneDeleted();
  };

} // namespace Molsketch

#endif // PROPERTYLISTENER_H
