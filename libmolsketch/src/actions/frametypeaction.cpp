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
#include <QSet>
#include <QDebug>
#include "frametypeaction.h"
#include "frametypewidget.h"
#include "frame.h"
#include "molecule.h"
#include "commands.h"
#include "molscene.h"

namespace Molsketch {

  struct FrameTypeAction::privateData
  {
    FrameTypeAction *action;
    privateData(FrameTypeAction *action) : action(action) {}
    void applyToFrameItem(Frame* frame, const QString& frameString)
    {
      action->attemptUndoPush(new Commands::SetFrameTypeString(frame, frameString, tr("Set decoration type")));
    }
    void applyToMoleculeItem(Molecule*, int type)
    {

    }
    bool itemsAreOnlyFramesAndNotEmpty()
    {
      if (action->items().isEmpty()) return false;
      for(auto item : action->items())
      {
        if (item->type() != Frame::Type) return false;
        if (!dynamic_cast<Frame*>(item)) return false;
      }
      return true;
    }
  };

  FrameTypeAction::FrameTypeAction(MolScene *scene) :
    ItemGroupTypeAction(scene),
    d(new privateData(this))
  {
    setItemTypeWidget(new FrameTypeWidget);
    setText(tr("Decoration type"));
  }

  FrameTypeAction::~FrameTypeAction()
  {
    delete d;
  }

  void FrameTypeAction::getType(int &type, QVariant &data) const
  {
    Q_UNUSED(type)
    data = QVariant();
    if (!d->itemsAreOnlyFramesAndNotEmpty()) return;
    auto frame = dynamic_cast<Frame*>(items().first());
    if (!frame) return; // TODO hm...
    data = frame->frameString();
  }

  // TODO make utility function or lambda and copy_if
  bool isFrame(QGraphicsItem* item)
  {
    return dynamic_cast<Frame*>(item) && item->type() == Frame::Type;
  }

  // TODO make utility function or lambda and copy_if
  QSet<graphicsItem *> onlyTopLevelItemsOrFrames(const QList<graphicsItem *>& inputList)
  {
    qDebug() << "input list" << inputList;
    QSet<graphicsItem *> resultSet;
    for(auto item : inputList)
    {
      while (isFrame(item->parentItem()))
        item = (graphicsItem*) item->parentItem();
      QGraphicsItem* parent = item;
      while((parent = parent->parentItem()))
        if (inputList.contains((graphicsItem*) parent))
          break;
      if (!parent)
        resultSet << item;
    }
    return resultSet;
  }

  void FrameTypeAction::applyType(int type, const QVariant &data) const
  {
    Q_UNUSED(type)
    auto itemList = onlyTopLevelItemsOrFrames(items());
    if (itemList.isEmpty()) return;


    if (data.isValid() && data.canConvert(QVariant::String))
    {
      if (itemList.size() == 1 && isFrame(itemList.toList().first()))
      {
        attemptUndoPush(new Commands::SetFrameTypeString((Frame*) itemList.toList().first(), data.toString(), tr("Change decoration type")));
      }
      else
      {
        Frame *frame = new Frame();
        frame->setFrameString(data.toString());
        attemptBeginMacro(tr("Add decoration"));
        attemptUndoPush(new Commands::AddItem(frame, scene()));
        for(auto selectedItem : itemList)
          attemptUndoPush(new Commands::SetParentItem(selectedItem, frame));
        attemptEndMacro();
      }
    }
    else
    {
      QList<graphicsItem*> frameList;

      std::copy_if(itemList.begin(),
                   itemList.end(), std::back_inserter(frameList),
                   isFrame);
      if (frameList.isEmpty()) return;
      attemptBeginMacro(tr("Remove decoration"));
      for (auto frame : frameList)
        attemptUndoPush(new Commands::DelItem(frame));
      attemptEndMacro();
    }
  }

} // namespace Molsketch
