include(../findOpenBabel.pri)
include(../settings.pri)

TEMPLATE = lib
HEADERS += obabeliface.h
SOURCES += obabeliface.cpp
INCLUDEPATH += $$MSK_SOURCE_BASE/libmolsketch
LIBS += -L../lib -lmolsketch$$qtVersionSuffix
CONFIG -= static
CONFIG += shared
QMAKE_LFLAGS -= -static
CONFIG += DLLDESTDIR
TARGET = obabeliface$$qtVersionSuffix
DESTDIR = ../lib
win* : VERSION =

target.path=$${MSK_INSTALL_LIBS}
contains(QT_ARCH, ".*64.*") : target.path = $${MSK_INSTALL_LIBS64}
INSTALLS += target
