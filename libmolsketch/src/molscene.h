/***************************************************************************
 *   Copyright (C) 2007 by Harm van Eersel                                 *
 *   Copyright (C) 2009 by Tim Vandermeersch                               *
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

#ifndef MSK_MOLSCENE_H
#define MSK_MOLSCENE_H

#include <QGraphicsScene>
#include <QUndoCommand>

#include "bond.h"
#include "abstractxmlobject.h"

class QString;
class QImage;
class QListWidgetItem;
class QTableWidgetItem;
class QUndoStack;

namespace Molsketch {

  class Molecule;
  class Atom;
  class Bond;
//  class TextInputItem;
  class genericAction;

  class MolSceneOptions
  {
    public:
  
  };


  /**
   * This class is a subclassed version of QGraphicsScene with addition of some extra methods and
   * event handling for molecules. In can be used in the same fashion as QGraphicsScene.
   *
   */
  class MolScene : public QGraphicsScene, public abstractXmlObject
  {
    Q_OBJECT

    public:
      /** 
       * Enum for the different edit modes. 
       */
      enum EditMode {
        MoveMode,     //!< Atoms and molecules are movable.
        DrawMode,     //!< Mode to add atoms and bonds.
        RotateMode,   //!< Mode to rotate molecules.
        ChargeMode,   //!< Increase/decrease charges
        HydrogenMode, //!< increase/decrease implicit hydrogen count
        LassoMode,    //!< lasso selection tool
        TextMode,     //!< add / edit text
        MinimiseMode, //!< gemoetry optimise
        ConnectMode,  //!< mode to drag/drop items
        ReactionMode, //! mode to insert reaction arrows
        MechanismMode, //!< mode for inserting mechanism arrows
      };

      enum RenderMode
      {
        RenderLabels,
        RenderColoredSquares,
        RenderColoredCircles,
        RenderColoredWireframe,
      };

      /**
       * Creates a new MolScene with @p parent.
       */
      MolScene(QObject* parent = 0);
      /** 
       * Cleans up the UndoStack and deletes the molscene. 
       */
      ~MolScene();

      void addResidue (QPointF pos = QPointF (0, 0), QString name = "");	

      // Queries
      /** 
       * @return The current EditMode of the scene. 
       */
      int editMode() const;
      /** 
       * @return @c True if neutral carbons are visible, @c false otherwise. 
       */
      bool carbonVisible() const;
      /** 
       * @returns @c True if hydrogens are visible, @c false otherwise. 
       */
      bool hydrogenVisible() const;
      /** Returns @c true if atom valency is visible, return @c false otherwise. */
      bool chargeVisible() const
      {
        return m_chargeVisible;
      }
      bool electronSystemsVisible() const
      {
        return m_electronSystemsVisible;
      }
      bool lonePairsVisible() const
      {
        return false;
      }
      /** Returns @c true if hydrogens are automaticly added, return @c false otherwise. */
      bool autoAddHydrogen() const { return m_autoAddHydrogen; };

      /** Returns the current atom size. */
      qreal atomSize() const;
      /** Returns the current atomsymbol font */
      QFont atomSymbolFont() const;
      /** Returns the current dynamic grid radius. */
      int dynamicGridSize() const;
      
      
      /**
       * @return The current RenderMode. Default is RnderLabels
       */
      RenderMode renderMode() const;
      /**
       * Set the RenderMode.
       */
      void setRenderMode(RenderMode mode);

      // Commands  
      /** Renders the @p rect on the scene in a image. */
      QImage renderImage(const QRectF &rect);

      QImage renderMolToImage (Molecule *mol);


      /** Sets whether neutral carbons are drawn. */
      void setCarbonVisible(bool value);
      /** Sets whether hydrogens are drawn. */
      void setHydrogenVisible(bool value);
      /** Sets whether atom charges are drawn. */
      void setChargeVisible(bool value)
      {
        m_chargeVisible = value;
      }
      /**
       * Set whether electron systems are drawn.
       */
      void setElectronSystemsVisible(bool value)
      {
        m_electronSystemsVisible = value;
      }
      /** Sets whether hydrogens are automaticly added. */
      void setAutoAddHydrogen(bool value) { m_autoAddHydrogen = value; };
      /** Sets the atomsymbol font */
      void setAtomSymbolFont(const QFont & font);
      /** Sets the atom size. */
      void setAtomSize(qreal size);


      /** Access to the stack */
      QUndoStack * stack();

      QList<Molsketch::genericAction *> sceneActions() const;

      // Advanced queries
      /** Returns the first molecule at position @p pos or NULL if none. */
      Molecule* moleculeAt(const QPointF &pos);
      /** Returns the first atom at position @p pos or NULL if none. */
      Atom* atomAt(const QPointF &pos);
      /** Returns the first bond at position @p pos or NULL if none. */
      Bond* bondAt(const QPointF &pos);

      bool textEditItemAt (const QPointF &pos) ;

      /** Calculates the nearest magnetic point around @p curPos. */
      QPointF nearestPoint(const QPointF &curPos);

    signals:
      /** Signal emitted if copy becomes available. */
      void copyAvailable(bool);
      /** Signal emitted if paste becomes available. */
      void pasteAvailable(bool);
      /** Signal emitted if the edit mode changes. */
      void editModeChange(int);
      /** Signal emitted if the contents of the scene change. */
      void documentChange( );
      /** Signal emitted if the selection on the scene changes. */
      void selectionChange( );
      //  /** Signal emitted if a new molecule is added to the scene. */
      //   void newMolecule(QPointF,QString);
      /** 
       * Sets the number of hint points in the dynamic grid. 
       */
      void setHintPointSize(int size);

    public slots:
      void selectionSlot();
      /** Slot to cut the current selection to the clipboard. */
      void cut();
      /** Slot to copy the current selection to the clipboard. */
      void copy();
      /** Slot to paste the current clipboard contents. */
      void paste();
      /** Slot to clear the scene. */
      void clear();
      /** Slot to set the edit mode to @p mode. */
      void setEditMode(int mode);
      /** Slot to select all contents of the scene. */
      void selectAll();
      /** Slot to add a copy of molecule @p mol. */
      void addMolecule(Molecule* mol);
      /** Slot to align the molecules of the scene to the grid. */
      void alignToGrid();

      /** Slot to convert image to mol using OSRA */
      void convertImage();

#ifdef QT_DEBUG
  void debugScene();
#endif


    protected:
      /** Generic event handler. Reimplementation for sceneChanged signals. */
      bool event (QEvent* event);

      /** Event handler for keyboard interaction */
      virtual void keyPressEvent(QKeyEvent* keyEvent);
      /** Event handler to display context menu */
      void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
      /** Event handler for mouse clicks */
      void mousePressEvent(QGraphicsSceneMouseEvent *event);
      /** Event handler for mouse moves */
      void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
      /** event handler for mouse button releases */
      void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
  public:
      //item to accept input for text tool
//      TextInputItem *m_inputTextItem;
      void setColor (QColor);
      QColor color() const;


      qreal bondWidth() const;
      void setBondWidth(const qreal &bondWidth);

      qreal arrowLineWidth() const;
      void setArrowLineWidth(const qreal &arrowLineWidth);

      QString xmlName() const { return "div" ; }
  protected:
      abstractXmlObject* produceChild(const QString &childName, const QString& type) ;
      QList<const abstractXmlObject*> children() const ;
      void readAttributes(const QXmlStreamAttributes &attributes) ;
      QXmlStreamAttributes xmlAttributes() const;

  private:

      // Hinting methods and properties
      /** Shows a highlight rectangle around @p item. */
      void setHoverRect( QGraphicsItem* item );

      // Auxillary methods
      /** Returns the nearest grid point, starting from @p position. */
      QPointF toGrid(const QPointF &position);


      // Scene properties
      /** Stores the edit mode of the scene as an integer. */
      int m_editMode;
      /** Stores the current bond length. */
      //qreal m_bondLength;
      /** Stores the current bond width. */
      qreal m_bondWidth;
      /** Stores the current arrow line width. */
      qreal m_arrowLineWidth ; // TODO add to XML
      /** Stores the current bond order. */
      //int m_bondOrder;
      /** Stores the current bond type. */
      //Bond::BondType m_bondType;
      /** Strores the bond angle. */
      //int m_bondAngle;

      qreal m_atomSize; //!< Stores the current atom size.
      QFont m_atomSymbolFont; //!< Stores the current atomsymbol font.
      bool m_hydrogenVisible; //!< Stores whether hydrogens should be visible.
      bool m_carbonVisible; //!< Stores whether neutral carbons are to be shown.
      bool m_chargeVisible; //!< Stores whether the charge of the atoms is to be shown.
      bool m_autoAddHydrogen; //!< Stores whether hydrogens are to be added automaticly.
      bool m_electronSystemsVisible; //!< Stores whether electron systems should be visible.

      RenderMode m_renderMode;

      QColor m_color;

      class privateData;
      privateData *d;

      // Internal clipboard
      /** Internal clipboard for scene items.*/
      QList<Molecule*> m_clipItems;

      // Undo stack
      /** The undo stack of the commands used to edit the scene. */
      QUndoStack * m_stack;

      // Event handlers

      /** Event handler for mouse presses in text mode. */
      void textModePress(QGraphicsSceneMouseEvent* event);	
      /** Event handler for mouse releases in text mode. */
      void textModeRelease(QGraphicsSceneMouseEvent* event);








      // Auxillary feedback items
      /** The highlight rectangle. */
      QGraphicsPathItem* m_hoverRect;
  private slots:
      void updateAll() ;


};

} // namespace

#endif
