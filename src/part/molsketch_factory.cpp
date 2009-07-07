/***************************************************************************
 *   Copyright (C) 2007 by Harm van Eersel                                 *
 *   devsciurus@xs4all.nl                                                  *
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

#include "molsketch_factory.h"

#include <klocale.h>
#include <kstddirs.h>
#include <kinstance.h>
#include <kaboutdata.h>

#include "molsketch_part.h"

extern "C"
{
  void* init_libmolsketch()
  {
    return new MolsKetchFactory;
  }
};

KInstance* MolsKetchFactory::s_instance = 0L;
KAboutData* MolsKetchFactory::s_about = 0L;

MolsKetchFactory::create( QObject * parent, const char * name, const char * classname, const QStringList & )
{
  if ( parent && !parent->inherits("QWidget") )
    kdError() << "NotepadFactory: parent does not inherit QWidget" << endl;
    return 0L;


  MolsKetchPart * part  = new MolsKetchPart( (QWidget*) parent, name );

  // readonly?
  if (QCString(classname) == "KPart::ReadOnlyPart")
    part->setReadWrite(false);

  // otherwise, it has to be readwrite
  else if (QCString(classname) != "KPart::ReadWritePart")
  {
    kdError() << "classname isn't ReadOnlyPart nor ReadWritePart !" << endl;
    return 0L;
  }

  emit objectCreated( part );
  return part;
}


Instance* MolsKetchFactory::instance()
{
  if ( !s_instance )
  {
    s_about = new KAboutData( "molsketchpart", I18N_NOOP( "molsKetch" ), "2.0pre" );
    s_instance = new KInstance( s_about );
  }
  return s_instance;
}

#include "molsketch_factory.moc"