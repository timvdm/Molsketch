isEmpty(CXXTEST_PATH) : error("Pass CXXTEST_PATH on command line")
include(../findOpenBabel.pri)
include(../settings.pri)

TESTS = *test.h

originalSources = \
    $$PWD/../libmolsketch/*.cpp \
    $$PWD/../libmolsketch/actions/*.cpp \
    $$PWD/../obabeliface/*.cpp \
    $$files($$PWD/../molsketch/*.cpp)
originalSources -= $$PWD/../molsketch/main.cpp
originalHeaders = \
    $$PWD/../libmolsketch/*.h \
    $$PWD/../libmolsketch/actions/*.h \
    $$PWD/../obabeliface/*.h \
    $$PWD/../molsketch/*.h
originalForms = \
    $$PWD/../libmolsketch/*.ui \
    $$PWD/../molsketch/*.ui

HEADERS += $$TESTS \
    $$originalHeaders \
    rectanglevaluetrait.h \
    qstringvaluetrait.h \
    qvariantvaluetrait.h \
    utilities.h \
    programversionvaluetrait.h \
    noargsignalcounter.h \
    signalcounter.h \
    modelindexsignalcounter.h

FORMS += $$originalForms

CONFIG += c++14

INCLUDEPATH += $$CXXTEST_PATH \
    ../libmolsketch \
    ../molsketch \
    ../obabeliface

SOURCES += \
    programversionvaluetrait.cpp \
    instancecounters.cpp

QT += widgets printsupport svg testlib network

TEMPLATE = app

TARGET = msktests

cxxtest.output = ${QMAKE_FILE_BASE}.cpp
cxxtest.commands = $$CXXTEST_PATH/bin/cxxtestgen --xunit-printer --part ${QMAKE_FILE_NAME} -o ${QMAKE_FILE_OUT}
cxxtest.depency_type = TYPE_C
cxxtest.input = TESTS
cxxtest.variable_out = SOURCES
QMAKE_EXTRA_COMPILERS += cxxtest

changelogSyntax.target = changelogsuccess
changelogSyntax.commands = xmllint --noout $$PWD/../CHANGELOG && touch $$changelogSyntax.target
changelogSyntax.depends = $$PWD/../CHANGELOG
QMAKE_EXTRA_TARGETS += changelogSyntax
POST_TARGETDEPS += $$changelogSyntax.target

cxxrunner.target = cxxrunner.cpp
cxxrunner.commands = $$CXXTEST_PATH/bin/cxxtestgen --have-eh --xunit-printer --root -o cxxrunner.cpp --template $$PWD/runnerTemplate.tpl
cxxrunner.depends = $$PWD/runnerTemplate.tpl
#cxxrunner.output = cxxrunner.cpp
#cxxrunner.variable_out = SOURCES
QMAKE_EXTRA_TARGETS += cxxrunner
SOURCES += $$OUT_PWD/cxxrunner.cpp \
    $$originalSources \
    rectanglevaluetrait.cpp \
    qstringvaluetrait.cpp \
    qvariantvaluetrait.cpp \
    utilities.cpp \
    mocks.cpp \
    noargsignalcounter.cpp \
    modelindexsignalcounter.cpp

OTHER_FILES += \
    runnerTemplate.tpl \
    legacy/Carbon-0.2.msk \
    legacy/Carbon-0.2.svg

legacy_files.commands = ${COPY_DIR} $$_PRO_FILE_PWD_/legacy ./legacy
QMAKE_EXTRA_TARGETS += legacy_files
PRE_TARGETDEPS += legacy_files

QMAKE_CXXFLAGS += -g -Wall -fprofile-arcs -ftest-coverage -O0
QMAKE_LFLAGS += -g -Wall -fprofile-arcs -ftest-coverage  -O0
LIBS += -lgcov
