qtVersionSuffix=""
equals(QT_MAJOR_VERSION, 5) {
    qtVersionSuffix= -qt5
}
DEFINES += "QTVERSIONSUFFIX=\"\\\"$$qtVersionSuffix\\\"\""

DEFINES += QMAKEBUILD

CONFIG += silent

qtVersionSuffix=""
equals(QT_MAJOR_VERSION, 5) {
    qtVersionSuffix= -qt5
}

HEADERS += \
    libmolsketch/src/atom.h \
    libmolsketch/src/atomnumberitem.h \
    libmolsketch/src/bond.h \
    libmolsketch/src/commands.h \
    libmolsketch/src/electronsystem.h \
    libmolsketch/src/element.h \
    libmolsketch/src/fileio.h \
    libmolsketch/src/graphsymitem.h \
    libmolsketch/src/itemplugin.h \
    libmolsketch/src/math2d.h \
    libmolsketch/src/mechanismarrow.h \
    libmolsketch/src/mechanismarrowdialog.h \
    libmolsketch/src/mimemolecule.h \
    libmolsketch/src/minimise.h \
    libmolsketch/src/molecule.h \
    libmolsketch/src/molinputitem.h \
    libmolsketch/src/mollibitem.h \
    libmolsketch/src/molscene.h \
    libmolsketch/src/molview.h \
    libmolsketch/src/reactionarrow.h \
    libmolsketch/src/reactionarrowdialog.h \
    libmolsketch/src/residue.h \
    libmolsketch/src/ring.h \
    libmolsketch/src/smilesitem.h \
    libmolsketch/src/stereocenteritem.h \
    libmolsketch/src/TextInputItem.h \
    libmolsketch/src/actions/coloraction.h \
    libmolsketch/src/graphicsitem.h \
    libmolsketch/src/actions/linewidthaction.h \
    libmolsketch/src/actions/abstractitemaction.h \
    libmolsketch/src/abstractxmlobject.h \
    libmolsketch/src/actions/genericaction.h \
    libmolsketch/src/actions/multiaction.h \
    libmolsketch/src/actions/incdecaction.h \
    libmolsketch/src/actions/connectaction.h \
    libmolsketch/src/actions/minimizeaction.h \
    libmolsketch/src/actions/rotateaction.h \
    libmolsketch/src/actions/transformcommand.h \
    libmolsketch/src/actions/reactionarrowaction.h \
    libmolsketch/src/arrow.h \
    libmolsketch/src/actions/periodictablewidget.h \
    libmolsketch/src/actions/bondtypewidget.h \
    libmolsketch/src/actions/drawaction.h \
    libmolsketch/src/actions/ringaction.h \
    libmolsketch/src/actions/mechanismarrowaction.h \
    libmolsketch/src/actions/pluginaction.h \
    libmolsketch/src/actions/deleteaction.h \
    libmolsketch/src/actions/bondtypeaction.h \
    libmolsketch/src/actions/itemtypewidget.h \
    libmolsketch/src/actions/itemtypeaction.h \
    libmolsketch/src/actions/arrowtypeaction.h \
    libmolsketch/src/actions/arrowtypewidget.h \
    libmolsketch/src/actions/flipbondaction.h \
    libmolsketch/src/xmlfont.h \
    libmolsketch/src/grid.h

SOURCES += \
    libmolsketch/src/atom.cpp \
    libmolsketch/src/atomnumberitem.cpp \
    libmolsketch/src/bond.cpp \
    libmolsketch/src/commands.cpp \
    libmolsketch/src/electronsystem.cpp \
    libmolsketch/src/element.cpp \
    libmolsketch/src/fileio.cpp \
    libmolsketch/src/graphsymitem.cpp \
    libmolsketch/src/itemplugin.cpp \
    libmolsketch/src/mechanismarrow.cpp \
    libmolsketch/src/mechanismarrowdialog.cpp \
    libmolsketch/src/mimemolecule.cpp \
    libmolsketch/src/minimise.cpp \
    libmolsketch/src/molecule.cpp \
    libmolsketch/src/molinputitem.cpp \
    libmolsketch/src/mollibitem.cpp \
    libmolsketch/src/molscene.cpp \
    libmolsketch/src/molview.cpp \
    libmolsketch/src/reactionarrow.cpp \
    libmolsketch/src/reactionarrowdialog.cpp \
    libmolsketch/src/residue.cpp \
    libmolsketch/src/ring.cpp \
    libmolsketch/src/smilesitem.cpp \
    libmolsketch/src/stereocenteritem.cpp \
    libmolsketch/src/TextInputItem.cpp \
    libmolsketch/src/actions/coloraction.cpp \
    libmolsketch/src/graphicsitem.cpp \
    libmolsketch/src/actions/linewidthaction.cpp \
    libmolsketch/src/actions/abstractitemaction.cpp \
    libmolsketch/src/abstractxmlobject.cpp \
    libmolsketch/src/actions/genericaction.cpp \
    libmolsketch/src/actions/multiaction.cpp \
    libmolsketch/src/actions/incdecaction.cpp \
    libmolsketch/src/actions/connectaction.cpp \
    libmolsketch/src/actions/minimizeaction.cpp \
    libmolsketch/src/actions/rotateaction.cpp \
    libmolsketch/src/actions/transformcommand.cpp \
    libmolsketch/src/actions/reactionarrowaction.cpp \
    libmolsketch/src/arrow.cpp \
    libmolsketch/src/actions/periodictablewidget.cpp \
    libmolsketch/src/actions/bondtypewidget.cpp \
    libmolsketch/src/actions/drawaction.cpp \
    libmolsketch/src/actions/ringaction.cpp \
    libmolsketch/src/actions/mechanismarrowaction.cpp \
    libmolsketch/src/actions/pluginaction.cpp \
    libmolsketch/src/actions/deleteaction.cpp \
    libmolsketch/src/actions/bondtypeaction.cpp \
    libmolsketch/src/actions/itemtypewidget.cpp \
    libmolsketch/src/actions/itemtypeaction.cpp \
    libmolsketch/src/actions/arrowtypeaction.cpp \
    libmolsketch/src/actions/arrowtypewidget.cpp \
    libmolsketch/src/actions/flipbondaction.cpp \
    libmolsketch/src/xmlfont.cpp \
    libmolsketch/src/grid.cpp

FORMS += \
    libmolsketch/src/mechanismarrowdialog.ui \
    libmolsketch/src/reactionarrowdialog.ui

RESOURCES += \
    libmolsketch/src/tools/toolicons.qrc

QT += svg printsupport

TEMPLATE = app

RESOURCES += \
    molsketch/src/molsketch.qrc

FORMS += \
    molsketch/src/drawwidget.ui \
    molsketch/src/settings.ui

HEADERS += \
    molsketch/src/mainwindow.h \
    molsketch/src/settings.h

SOURCES += \
    molsketch/src/main.cpp \
    molsketch/src/mainwindow.cpp \
    molsketch/src/settings.cpp

INCLUDEPATH += \
	libmolsketch/src \
	obabeliface

TARGET = molsketch$$qtVersionSuffix
DESTDIR = bin

target.path=bin
INSTALLS += target

