include(../settings.pri)

TEMPLATE = lib
CONFIG += plugin
HEADERS += *.h $$files(actions/*.h)
SOURCES += *.cpp $$files(actions/*.cpp)
FORMS += *.ui
RESOURCES += tools/toolicons.qrc
TARGET = molsketch$$qtVersionSuffix
DESTDIR = ../lib
QMAKE_CXXFLAGS_DEBUG +=  -g -gstabs
win* : VERSION =

target.path=$${MSK_INSTALL_LIBS}
INSTALLS += target
