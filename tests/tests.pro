include(../settings.pri)

QT += testlib widgets

INCLUDEPATH += $$topLevelDir/libmolsketch/src

SOURCES += valencetest.cpp \
    #drawingtest.cpp

LIBS += -L$$topLevelDir/lib -lmolsketch$$qtVersionSuffix

TARGET = molsketch-test$$qtVersionSuffix
DESTDIR = $$topLevelDir/bin

HEADERS +=
