include(../settings.pri)

QT += testlib widgets

INCLUDEPATH += $$sourceDir/../libmolsketch/src

SOURCES += \
    main.cpp \
    coordinatestest.cpp \
    valencetest.cpp \
    drawingtest.cpp \
    xmltest.cpp

LIBS += -L../lib -lmolsketch$$qtVersionSuffix

TARGET = molsketch-test$$qtVersionSuffix
DESTDIR = ../bin

HEADERS += \
    coordinatestest.h \
    valencetest.h \
    drawingtest.h \
    xmltest.h
