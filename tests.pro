isEmpty(CXXTEST_PATH) : error("Pass CXXTEST_PATH on command line")

TESTS = *test.h

HEADERS += $$TESTS \
    rectanglevaluetrait.h \
    qstringvaluetrait.h \
    qvariantvaluetrait.h \
    utilities.h


CONFIG += c++14

INCLUDEPATH += $$CXXTEST_PATH \
    ../libmolsketch/src

QT += widgets printsupport svg testlib

TEMPLATE = app

TARGET = msktests

cxxtest.output = ${QMAKE_FILE_BASE}.cpp
cxxtest.commands = $$CXXTEST_PATH/bin/cxxtestgen --xunit-printer --part ${QMAKE_FILE_NAME} -o ${QMAKE_FILE_OUT}
cxxtest.depency_type = TYPE_C
cxxtest.input = TESTS
cxxtest.variable_out = SOURCES
QMAKE_EXTRA_COMPILERS += cxxtest

cxxrunner.target = cxxrunner.cpp
cxxrunner.commands = $$CXXTEST_PATH/bin/cxxtestgen --have-eh --xunit-printer --root -o cxxrunner.cpp --template $$PWD/runnerTemplate.tpl
cxxrunner.depends = $$PWD/runnerTemplate.tpl
#cxxrunner.output = cxxrunner.cpp
#cxxrunner.variable_out = SOURCES
QMAKE_EXTRA_TARGETS += cxxrunner
SOURCES += $$OUT_PWD/cxxrunner.cpp \
    rectanglevaluetrait.cpp \
    qstringvaluetrait.cpp \
    qvariantvaluetrait.cpp \
    utilities.cpp

LIBS += -L../lib -lmolsketch-qt5

OTHER_FILES += \
    runnerTemplate.tpl
