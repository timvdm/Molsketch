/***************************************************************************
 *   Copyright (C) 2007-2008 by Harm van Eersel                            *
 *   Copyright (C) 2009 by Tim Vandermeersch                               *
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

#include <QtGui>

#include "molecule.h"
#include "bond.h"
#include "molscene.h"
#include "mollibitem.h"
#include "fileio.h"

namespace Molsketch {

  MolLibItem::MolLibItem( Molecule* molecule, const QString & name )
  {
    // pre: molecule is a valid molecule
    Q_CHECK_PTR(molecule);
    //Q_ASSERT(!molecule->atoms().isEmpty());

    // Copying the molecule
    m_molecule = new Molecule(molecule);
    m_molecule->setPos(0, 0);

    // Creating pixmap
    MolScene renderScene;
    if (molecule->atoms().size() > 20)
      renderScene.setRenderMode(MolScene::RenderColoredSquares);
    renderScene.addItem(m_molecule);
    renderScene.setChargeVisible(false);
    renderScene.setSceneRect(m_molecule->boundingRect());
    QPixmap pixmap(int(renderScene.width()), int(renderScene.height()));
    pixmap.fill();

    // Creating and setting the painter
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    renderScene.render(&painter);

    setIcon(QIcon(pixmap));

    // Checking dir
    QDir dir;
    /* TODO Shouldn't this be the dir set in the preferences dialog? */
    if (!dir.exists(QDir::homePath() + "/.molsketch/library/custom/")) {
      dir.mkpath(QDir::homePath() + "/.molsketch/library/custom/");
    }

    // Setting name
    m_fileName = name;
    /* TODO Nah, there should be a better way then to load the items from the dir
   * and then check again whether they exist. 
   */
    if (!m_fileName.exists()) {
      m_fileName.setFile(QDir::homePath() + "/.molsketch/library/custom/" + name + ".mol");
      Molsketch::saveFile(m_fileName.filePath(),&renderScene);
    }

    setText(m_fileName.baseName());

    // Remove the molecule before destroying the scene
    renderScene.removeItem(m_molecule);

  }

  MolLibItem::~MolLibItem( )
  {
    delete m_molecule;
  }

  Molecule* MolLibItem::getMolecule( )
  {
    // Return a copy of the m_molecule
    return new Molecule(m_molecule);
  }

} // namespace
