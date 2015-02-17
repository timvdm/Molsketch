include(../../settings.pri)

TEMPLATE = app

RESOURCES += \
    molsketch.qrc

FORMS += \
    drawwidget.ui \
    settings.ui

HEADERS += \
    mainwindow.h \
    settings.h

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    settings.cpp

INCLUDEPATH += \
        ../../libmolsketch/src \
        ../../obabeliface

LIBS += -L../../lib -lmolsketch

TARGET = ../../bin/molsketch
equals(QT_MAJOR_VERSION, 5) {
	TARGET = ../../bin/molsketch-qt5
	LIBS -= -lmolsketch
	LIBS += -lmolsketch-qt5
}
