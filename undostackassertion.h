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
#ifndef UNDOSTACKASSERTION_H
#define UNDOSTACKASSERTION_H

#include <QScopedPointer>

class UndoStackAssertionPrivate;
class QUndoStack;

namespace Molsketch {
  class MolScene;
}

class UndoStackAssertion {
  Q_DECLARE_PRIVATE(UndoStackAssertion)
  QScopedPointer<UndoStackAssertionPrivate> d_ptr;
  UndoStackAssertion(QUndoStack*);
public:
  virtual ~UndoStackAssertion();
  UndoStackAssertion(UndoStackAssertion&&);

  UndoStackAssertion& hasElementCount(int);
  UndoStackAssertion& isCurrentlyAtElementNo(int);

  static UndoStackAssertion undoStackOf(Molsketch::MolScene* scene);
  static UndoStackAssertion undoStack(QUndoStack* stack);
};

namespace UndoStackAssert {
  UndoStackAssertion undoStackOf(Molsketch::MolScene*);
  UndoStackAssertion undoStack(QUndoStack*);
}

#endif // UNDOSTACKASSERTION_H
