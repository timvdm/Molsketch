include(../settings.pri)

TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS += \
        ../libmolsketch/src \
        ../molsketch/src \
        ../tests
