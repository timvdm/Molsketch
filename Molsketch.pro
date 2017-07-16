message("======= Welcome to molsKetch build =======")
include(settings.pri)

TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS += \
        libmolsketch \
        molsketch
equals(MSK_OBABELIFACE, true) : SUBDIRS += obabeliface

INSTALLS += documentation_en \
            documentation_cs \
            documentation_nl

documentation_en.files = $$PWD/doc/en/*
documentation_cs.files = $$PWD/doc/cs/*
documentation_nl.files = $$PWD/doc/nl/*
molecule_library.files = $$PWD/library/*
custom_library.files = $$PWD/library/custom/*

documentation_en.path = $${MSK_INSTALL_DOCS}/en
documentation_cs.path = $${MSK_INSTALL_DOCS}/cs
documentation_nl.path = $${MSK_INSTALL_DOCS}/nl
molecule_library.path = $${MSK_INSTALL_LIBRARY}
custom_library.path = $${MSK_INSTALL_CUSTOM}

OTHER_FILES += \
    version \
    versionnick \
    CHANGELOG
