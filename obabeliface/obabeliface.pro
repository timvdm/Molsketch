include(../findOpenBabel.pri)
include(../settings.pri)

TEMPLATE = lib

HEADERS += \
    obabeliface.h

SOURCES += \
    obabeliface.cpp

INCLUDEPATH += $$sourceDir/../libmolsketch/src
LIBS += -L../lib -lmolsketch$$qtVersionSuffix
TARGET = obabeliface$$qtVersionSuffix
DESTDIR = ../lib

QT += widgets

target.path=$$PREFIX/lib/molsketch
INSTALLS += target
