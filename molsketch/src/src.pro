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

LIBS += -L../../lib -lmolsketch$$qtVersionSuffix

TARGET = ../../bin/molsketch$$qtVersionSuffix

target.path=$$PREFIX/bin
INSTALLS += target
