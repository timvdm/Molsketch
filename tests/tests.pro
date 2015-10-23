include(../settings.pri)

QT += testlib widgets

INCLUDEPATH += $$sourceDir/../libmolsketch/src

SOURCES += valencetest.cpp \
    #drawingtest.cpp

LIBS += -L../lib -lmolsketch$$qtVersionSuffix

TARGET = molsketch-test$$qtVersionSuffix
DESTDIR = ../bin

HEADERS +=
