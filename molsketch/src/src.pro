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
	$$sourceDir/../../libmolsketch/src \
	$$sourceDir/../../obabeliface

LIBS += -L../../lib -lmolsketch$$qtVersionSuffix

TARGET = molsketch$$qtVersionSuffix
DESTDIR = ../../bin

target.path=$${MSK_INSTALL_BINS}
INSTALLS += target
