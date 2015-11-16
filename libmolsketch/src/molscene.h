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
#include "stringify.h"

class QString;
class QImage;
class QListWidgetItem;
class QTableWidgetItem;
class QUndoStack;

namespace Molsketch {

  class Molecule;
  class Atom;
  class Bond;
  class TextInputItem;
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
      int editMode() const; // TODO obsolete?

#define SCENEPROPERTY(CAPLETTER, LOWERLETTER, PROPNAME, TYPE, DEFAULTVALUE) \
  void set##CAPLETTER##PROPNAME(const TYPE& value) { setProperty("Molscene" #PROPNAME, value); } \
  TYPE LOWERLETTER##PROPNAME() const { QVariant value = property("Molscene" #PROPNAME); \
      if (value.isValid()) return value.value<TYPE>(); \
      return DEFAULTVALUE; }

#define STRINGIFIEDPROPERTY(CAPLETTER, LOWERLETTER, PROPNAME, TYPE, DEFAULTVALUE) \
  void set##CAPLETTER##PROPNAME(const TYPE& value) { setProperty("Molscene" #PROPNAME, stringify(value)); } \
  TYPE LOWERLETTER##PROPNAME() const { QVariant value = property("Molscene" #PROPNAME); \
      if (value.isValid()) return makeFromString<TYPE>(value.toString()); \
      return DEFAULTVALUE; }

      SCENEPROPERTY(B,b,ondLength, qreal, 40)
      SCENEPROPERTY(B,b,ondWidth, qreal, 2)
      SCENEPROPERTY(A,a,rrowWidth, qreal, 1.5)
      SCENEPROPERTY(F,f,rameLinewidth, qreal, 1.5)
      SCENEPROPERTY(B,b,ondAngle, qreal, 30)
      SCENEPROPERTY(A,a,tomSize, qreal, 5)
      STRINGIFIEDPROPERTY(A,a,tomFont, QFont, QFont())
      SCENEPROPERTY(H,h,ydrogenVisible, bool, true)
      SCENEPROPERTY(C,c,arbonVisible, bool, false)
      SCENEPROPERTY(L,l,onePairsVisible, bool, false)
      SCENEPROPERTY(A,a,utoAddHydrogen, bool, true)
      SCENEPROPERTY(E,e,lectronSystemsVisible, bool, false)
      SCENEPROPERTY(C,c,hargeVisible, bool, true)
      STRINGIFIEDPROPERTY(D,d,efaultColor, QColor, QColor(Qt::black))
      
      QColor color() const;
      void setColor(const QColor&);

      /**
       * @return The current RenderMode. Default is RnderLabels
       */
      RenderMode renderMode() const; // TODO do we really need this?
      /**
       * Set the RenderMode.
       */
      void setRenderMode(RenderMode mode);

      // Commands  
      /** Renders the @p rect on the scene in a image. */
      QImage renderImage(const QRectF &rect);

      QImage renderMolToImage (Molecule *mol);

      QByteArray toSvg();


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

      TextInputItem *inputItem() ;

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
      void alignToGrid(); // TODO obsolete

      /** Slot to convert image to mol using OSRA */
      void convertImage();
      /** enable/disable grid */
      void setGrid(bool on = true);

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

      QString xmlName() const { return "div" ; }
      QPointF snapToGrid(const QPointF& point, bool force = false);
      bool snappingToGrid() const;
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
      RenderMode m_renderMode;

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
      void selectionSlot();
};

} // namespace

#endif
