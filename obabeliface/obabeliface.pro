include(../findOpenBabel.pri)

TEMPLATE = lib

HEADERS += \
    obabeliface.h

SOURCES += \
    obabeliface.cpp

INCLUDEPATH += ../libmolsketch/src
LIBS += -L../lib -lmolsketch
TARGET = ../lib/obabeliface
equals(QT_MAJOR_VERSION, 5) {
    TARGET = ../lib/obabeliface-qt5
    LIBS -= -lmolsketch
    LIBS += -lmolsketch-qt5
}

QT += widgets