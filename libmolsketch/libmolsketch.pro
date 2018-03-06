include(../settings.pri)

TEMPLATE = lib
HEADERS += $$files(*.h, true)
SOURCES += $$files(*.cpp, true)
FORMS += *.ui
RESOURCES += tools/toolicons.qrc
TARGET = molsketch$$qtVersionSuffix
DESTDIR = ../lib
QMAKE_CXXFLAGS_DEBUG +=  -g -gstabs
win* : VERSION =

target.path=$${MSK_INSTALL_LIBS}
contains(QT_ARCH, ".*64.*") : target.path = $${MSK_INSTALL_LIBS64}
INSTALLS += target includes
includes.files = $$HEADERS
includes.path = $${MSK_INSTALL_INCLUDES}

