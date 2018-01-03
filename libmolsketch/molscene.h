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
  class SceneSettings;

  /**
   * This class is a subclassed version of QGraphicsScene with addition of some extra methods and
   * event handling for molecules. In can be used in the same fashion as QGraphicsScene.
   *
   */
  class MolScene : public QGraphicsScene, public abstractXmlObject
  {
    Q_OBJECT

    public:
    static const QString mouseWheelForCyclingTools;
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
      explicit MolScene(QObject* parent = 0);
      /** Creates a new MolScene with @p settings. Will assume ownership of @p settings! */
      MolScene(SceneSettings *settings, QObject* parent = 0);
      /**
       * Cleans up the UndoStack and deletes the molscene.
       */
      ~MolScene();

      SceneSettings* settings() const;

      QFont getAtomFont() const;

      static QString mimeType();
      // Queries
      /**
       * @return The current EditMode of the scene.
       */
      int editMode() const; // TODO obsolete?

      qreal bondAngle() const;

      /**
       * @return The current RenderMode. Default is RenderLabels
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

      /** Calculates the nearest magnetic point around @p curPos. */
      QPointF nearestPoint(const QPointF &curPos);

      TextInputItem *inputItem() ;

      QMenu* sceneMenu() const;
      QList<Atom*> atoms() const;

    signals:
      /** Signal emitted if copy becomes available. */
      void copyAvailable(bool);
      /** Signal emitted if the edit mode changes. */
      void editModeChange(int);
      /** Signal emitted if the contents of the scene change. */
      void documentChange( );
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

      void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
      void dragLeaveEvent(QGraphicsSceneDragDropEvent *event);
      void dropEvent(QGraphicsSceneDragDropEvent *event);
      void dragMoveEvent(QGraphicsSceneDragDropEvent *event);

      void wheelEvent(QGraphicsSceneWheelEvent *event);
  public:

      QString xmlName() const;
      static QString xmlClassName();
      QPointF snapToGrid(const QPointF& point, bool force = false);
      bool snappingToGrid() const;
      bool cyclingByMouseWheelEnaled() const;
      Atom *atomNear(const QPointF &pos, qreal tolerance = 10.0);
      template <class T>
      T *itemNear(const QPointF &pos, qreal tolerance);
      qreal getRadicalDiameter() const;
      qreal getLonePairLength() const;
      qreal getLonePairLineWidth() const;
  protected:
      XmlObjectInterface *produceChild(const QString &childName, const QString& type) ;
      QList<const XmlObjectInterface *> children() const ;
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

      // Undo stack
      /** The undo stack of the commands used to edit the scene. */
      QUndoStack * m_stack;

      // Event handlers

      // Auxillary feedback items
      /** The highlight rectangle. */
      QGraphicsPathItem* m_hoverRect;
      void initialize(SceneSettings *settings);

      void initiializeGrid();

  private slots:
      void updateAll() ;
      void selectionSlot();
      void updateGrid(const QRectF &newSceneRect);
  };

} // namespace

#endif
