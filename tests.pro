isEmpty(CXXTEST_PATH) : error("Pass CXXTEST_PATH on command line")

HEADERS += *.h

INCLUDEPATH += $$CXXTEST_PATH \
    ../libmolsketch/src

QT += widgets printsupport svg

TEMPLATE = app

TARGET = msktests

cxxtest.output = ${QMAKE_FILE_BASE}.cpp
cxxtest.commands = $$CXXTEST_PATH/bin/cxxtestgen --xunit-printer --part ${QMAKE_FILE_NAME} -o ${QMAKE_FILE_OUT}
cxxtest.depency_type = TYPE_C
cxxtest.input = HEADERS
cxxtest.variable_out = SOURCES
QMAKE_EXTRA_COMPILERS += cxxtest

cxxrunner.target = cxxrunner.cpp
cxxrunner.commands = $$CXXTEST_PATH/bin/cxxtestgen --xunit-printer --root -o cxxrunner.cpp
QMAKE_EXTRA_TARGETS += cxxrunner
SOURCES += $$OUT_PWD/cxxrunner.cpp

LIBS += -L../lib -lmolsketch-qt5
