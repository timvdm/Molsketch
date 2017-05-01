include(../settings.pri)

TEMPLATE = lib
INCLUDEPATH += $$MSK_SOURCE_BASE/obabeliface
HEADERS += *.h $$files(actions/*.h)
SOURCES += *.cpp $$files(actions/*.cpp)
FORMS += *.ui
RESOURCES += tools/toolicons.qrc
TARGET = molsketch$$qtVersionSuffix
DESTDIR = ../lib
QMAKE_CXXFLAGS_DEBUG +=  -g -gstabs
win* : VERSION =

target.path=$${MSK_INSTALL_LIBS}
contains(QT_ARCH, ".*64.*") : target.path = $${MSK_INSTALL_LIBS64}
INSTALLS += target
