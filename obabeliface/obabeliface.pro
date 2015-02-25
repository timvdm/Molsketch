include(../findOpenBabel.pri)
include(../settings.pri)

TEMPLATE = lib

HEADERS += \
    obabeliface.h

SOURCES += \
    obabeliface.cpp

INCLUDEPATH += $$topLevelDir/libmolsketch/src
LIBS += -L$$topLevelDir/lib -lmolsketch$$qtVersionSuffix
TARGET = obabeliface$$qtVersionSuffix
DESTDIR = $$topLevelDir/lib

QT += widgets

target.path=$$PREFIX/lib/molsketch
INSTALLS += target
