include(../settings.pri)

TEMPLATE = app
RESOURCES += *.qrc
FORMS += *.ui
HEADERS += *.h
SOURCES += *.cpp
LIBS += -L../lib -lmolsketch$$qtVersionSuffix
TARGET = molsketch$$qtVersionSuffix
DESTDIR = ../bin
target.path=$${MSK_INSTALL_BINS}
INSTALLS += target
QT += network

INCLUDEPATH += \
        $$MSK_SOURCE_BASE/libmolsketch \
        $$MSK_SOURCE_BASE/obabeliface
