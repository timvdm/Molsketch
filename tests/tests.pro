include(../findOpenBabel.pri)
include(../settings.pri)

QT += testlib widgets

INCLUDEPATH += ../libmolsketch/src

SOURCES += valencetest.cpp \
    #drawingtest.cpp

LIBS += -L../lib -lmolsketch$$qtVersionSuffix

TARGET = ../bin/molsketch-test$$qtVersionSuffix

HEADERS +=
