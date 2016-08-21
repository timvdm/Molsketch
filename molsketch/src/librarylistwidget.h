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
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef LIBRARYLISTWIDGET_H
#define LIBRARYLISTWIDGET_H

#include <QListWidget>

class LibraryListWidget : public QListWidget
{
  Q_OBJECT
public:
  explicit LibraryListWidget(QString directory, QWidget *parent = 0);
  QString title() const;
protected:
  QStringList mimeTypes() const;
  QMimeData* mimeData(const QList<QListWidgetItem *> items) const;
  void startDrag(Qt::DropActions supportedActions);
private:
  void setGuiConfiguration(const QString &directory);
  void buildItems(QString directory);
  QString Title;
};

#endif // LIBRARYLISTWIDGET_H
