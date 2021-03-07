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
#include "undostackassertion.h"
#include <QUndoStack>
#include <molscene.h>
#include "cxxtest/TestSuite.h"

class UndoStackAssertionPrivate {
public:
  QUndoStack *stack;
};

UndoStackAssertion::~UndoStackAssertion() {}

UndoStackAssertion &UndoStackAssertion::hasElementCount(int count) {
  Q_D(UndoStackAssertion);
  TSM_ASSERT_EQUALS("Undo stack count mismatch", d->stack->count(), count);
  return *this;
}

UndoStackAssertion &UndoStackAssertion::isCurrentlyAtElementNo(int index) {
  Q_D(UndoStackAssertion);
  TSM_ASSERT_EQUALS("Undo stack index mismatch", d->stack->index(), index);
  return *this;
}

UndoStackAssertion UndoStackAssertion::undoStackOf(Molsketch::MolScene *scene) {
  return undoStack(scene->stack());
}

UndoStackAssertion UndoStackAssertion::undoStack(QUndoStack *stack) {
  return UndoStackAssertion(stack);
}

UndoStackAssertion::UndoStackAssertion(QUndoStack *stack)
  : d_ptr(new UndoStackAssertionPrivate)
{
  Q_D(UndoStackAssertion);
  d->stack = stack;
}

UndoStackAssertion UndoStackAssert::undoStackOf(Molsketch::MolScene *scene) {
  return UndoStackAssertion::undoStackOf(scene);
}

UndoStackAssertion UndoStackAssert::undoStack(QUndoStack *stack) {
  return UndoStackAssertion::undoStack(stack);
}
