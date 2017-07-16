include(../findOpenBabel.pri)
include(../settings.pri)

TEMPLATE = lib
HEADERS += obabeliface.h
SOURCES += obabeliface.cpp
INCLUDEPATH += $$sourceDir/../libmolsketch
LIBS += -L../lib -lmolsketch$$qtVersionSuffix
CONFIG -= static
CONFIG += shared
QMAKE_LFLAGS -= -static
CONFIG += DLLDESTDIR
TARGET = obabeliface$$qtVersionSuffix
DESTDIR = ../lib
win* : VERSION =
QT += widgets

target.path=$${MSK_INSTALL_LIBS}
INSTALLS += target
