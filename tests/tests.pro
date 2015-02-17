include(../findOpenBabel.pri)

QT += testlib widgets

INCLUDEPATH += ../libmolsketch/src

SOURCES += valencetest.cpp

LIBS += -L../lib -lmolsketch

TARGET = ../bin/molsketch-test
equals(QT_MAJOR_VERSION, 5) {
	TARGET = ../bin/molsketch-test-qt5
	LIBS -= -lmolsketch
	LIBS += -lmolsketch-qt5
}
