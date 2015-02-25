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
	$$topLevelDir/libmolsketch/src \
	$$topLevelDir/obabeliface

LIBS += -L$$topLevelDir/lib -lmolsketch$$qtVersionSuffix

TARGET = molsketch$$qtVersionSuffix
DESTDIR = $$topLevelDir/bin

target.path=$$PREFIX/bin
INSTALLS += target
