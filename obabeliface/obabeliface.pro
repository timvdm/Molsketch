include(../findOpenBabel.pri)
include(../settings.pri)

TEMPLATE = lib

HEADERS += \
    obabeliface.h

SOURCES += \
    obabeliface.cpp

INCLUDEPATH += ../libmolsketch/src
LIBS += -L../lib -lmolsketch$$qtVersionSuffix
TARGET = ../lib/obabeliface$$qtVersionSuffix

QT += widgets

target.path=$$PREFIX/lib/molsketch
INSTALLS += target
message("obabeliface prefix: $$PREFIX")
