include(../../settings.pri)

TEMPLATE = lib

INCLUDEPATH += $$sourceDir/../../obabeliface

HEADERS += \
    atom.h \
    atomnumberitem.h \
    bond.h \
    commands.h \
    electronsystem.h \
    element.h \
    fileio.h \
    graphsymitem.h \
    itemplugin.h \
    math2d.h \
    mimemolecule.h \
    minimise.h \
    molecule.h \
    molinputitem.h \
    mollibitem.h \
    molscene.h \
    molview.h \
    residue.h \
    ring.h \
    smilesitem.h \
    stereocenteritem.h \
    TextInputItem.h \
    actions/coloraction.h \
    graphicsitem.h \
    actions/linewidthaction.h \
    actions/abstractitemaction.h \
    abstractxmlobject.h \
    actions/genericaction.h \
    actions/multiaction.h \
    actions/incdecaction.h \
    actions/connectaction.h \
    actions/minimizeaction.h \
    actions/rotateaction.h \
    actions/transformcommand.h \
    actions/reactionarrowaction.h \
    arrow.h \
    actions/periodictablewidget.h \
    actions/bondtypewidget.h \
    actions/drawaction.h \
    actions/ringaction.h \
    actions/mechanismarrowaction.h \
    actions/pluginaction.h \
    actions/deleteaction.h \
    actions/bondtypeaction.h \
    actions/itemtypewidget.h \
    actions/itemtypeaction.h \
    actions/arrowtypeaction.h \
    actions/arrowtypewidget.h \
    actions/flipbondaction.h \
    stringify.h \
    frame.h \
    actions/frameaction.h \
    actions/frametypewidget.h \
    actions/frametypeaction.h \
    actions/itemgrouptypeaction.h \
    arrowpopup.h \
    coordinatedelegate.h \
    coordinatemodel.h \
    coordinatetableview.h \
    grid.h \
    actions/flipstereobondsaction.h \
    generics.h

SOURCES += \
    atom.cpp \
    atomnumberitem.cpp \
    bond.cpp \
    commands.cpp \
    electronsystem.cpp \
    element.cpp \
    fileio.cpp \
    graphsymitem.cpp \
    itemplugin.cpp \
    mimemolecule.cpp \
    minimise.cpp \
    molecule.cpp \
    molinputitem.cpp \
    mollibitem.cpp \
    molscene.cpp \
    molview.cpp \
    residue.cpp \
    ring.cpp \
    smilesitem.cpp \
    stereocenteritem.cpp \
    TextInputItem.cpp \
    actions/coloraction.cpp \
    graphicsitem.cpp \
    actions/linewidthaction.cpp \
    actions/abstractitemaction.cpp \
    abstractxmlobject.cpp \
    actions/genericaction.cpp \
    actions/multiaction.cpp \
    actions/incdecaction.cpp \
    actions/connectaction.cpp \
    actions/minimizeaction.cpp \
    actions/rotateaction.cpp \
    actions/transformcommand.cpp \
    actions/reactionarrowaction.cpp \
    arrow.cpp \
    actions/periodictablewidget.cpp \
    actions/bondtypewidget.cpp \
    actions/drawaction.cpp \
    actions/ringaction.cpp \
    actions/mechanismarrowaction.cpp \
    actions/pluginaction.cpp \
    actions/deleteaction.cpp \
    actions/bondtypeaction.cpp \
    actions/itemtypewidget.cpp \
    actions/itemtypeaction.cpp \
    actions/arrowtypeaction.cpp \
    actions/arrowtypewidget.cpp \
    actions/flipbondaction.cpp \
    frame.cpp \
    actions/frameaction.cpp \
    actions/frametypewidget.cpp \
    actions/frametypeaction.cpp \
    actions/itemgrouptypeaction.cpp \
    arrowpopup.cpp \
    coordinatedelegate.cpp \
    coordinatemodel.cpp \
    coordinatetableview.cpp \
    grid.cpp \
    actions/flipstereobondsaction.cpp \
    generics.cpp

FORMS += \
    arrowpopup.ui

RESOURCES += \
    tools/toolicons.qrc

QT += svg

TARGET = molsketch$$qtVersionSuffix
DESTDIR = ../../lib

INSTALLS += target includes
win* : VERSION =

target.path = $${MSK_INSTALL_LIBS}

includes.files = $$PWD/*.h
includes.path = $${MSK_INSTALL_INCLUDES}
