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
#ifndef MOLINPUTITEM_H
#define MOLINPUTITEM_H

#include "itemplugin.h"

namespace Molsketch {

  class Molecule;
  class Atom ;

  /**
   * Subclass of ItemPlugin to take a molecule as input. The type of output
   * is specified by an argument in the constructor.
   */
  class MolInputItem : public ItemPlugin
  {
    public:
      /**
       * It is important to use the right output type for subclasses. This
       * determines the boundingRect and shape() of the item while connected.
       */
      enum OutputType {
        LabelOuput, //!< output is a label only, no drawing "inside" the molecule (e.g. SMILES, name, logP, ...)
        MoleculeOutput, //!< output is can be drawn "inside" the molecule and there is also an optional label (e.g. atom numbers, symmetry classes, pKa values, ...)
      };

      /**
       * Constructor specifying the OutputType.
       */
      explicit MolInputItem(OutputType type);

      /**
       * Overload this method for custom labels while connected.
       */
      virtual QString label() const { return QString(); }

      Molecule* molecule() const { return m_molecule; }

      /**
       * Get the boundingRect for this item. While this item is not connected,
       * both the boundingRect and shape are the same. Once connected, the 
       * bounding depends on the LabelOutput used. LabelOutput sets the 
       * boundingRect and shape to the result of the label() method using
       * QFontMetrics. MoleculeOutput uses the molecule's boundingRect so when
       * it paints items "inside" the molecule, the paint operations will be 
       * within the boundingRect.
       *
       * To be complete, the boundingRect is actualy set to the previous QRectF
       * united with the optional label's boundingRect. The shape will be set to
       * the label only allowing the label to be moved freely.
       */
      QRectF boundingRect() const;
      /**
       * See boundingRect().
       */
      QPainterPath shape() const;

      virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
      void dropEvent(QGraphicsSceneDragDropEvent *event);
  protected:
      void drawTextNearAtom(QPainter* painter, Atom* atom, const QString& text, const Molecule *mol) ;
    private:
      OutputType m_output;
      Molecule *m_molecule;
      QRectF m_rect;
  };

}

#endif // SMILESITEM_H
