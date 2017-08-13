include(../settings.pri)

TEMPLATE = lib
HEADERS += *.h $$files(actions/*.h)
SOURCES += *.cpp $$files(actions/*.cpp)
FORMS += *.ui
RESOURCES += tools/toolicons.qrc
TARGET = molsketch$$qtVersionSuffix
DESTDIR = ../lib
QMAKE_CXXFLAGS_DEBUG +=  -g -gstabs
win* : VERSION =

INSTALLS += target includes
target.path = $${MSK_INSTALL_LIBS}

includes.files = $$PWD/*.h
includes.path = $${MSK_INSTALL_INCLUDES}
