include(settings.pri)
message("Welcome to molsKetch build")
message("Sources in: $$sourceDir")
message("Executable will be placed in dir: $$[QT_INSTALL_BINS]")
message("Libraries will be placed in dir: $$[QT_INSTALL_LIBS]")
message("Documentation will be placed in dir: $$[QT_INSTALL_BINS]/../share/doc/molsketch")
message("Structure library will be placed in ir: $$[QT_INSTALL_BINS]/../share/molsketch/library")

TEMPLATE = subdirs
CONFIG += ordered

# TODO (install uninstall)-targets
SUBDIRS += \
	libmolsketch/src \
	molsketch/src \
	obabeliface \
	tests

INSTALLS += documentation_en \
	    documentation_cs \
	    documentation_nl \
	    molecule_library \
	    custom_library

documentation_en.files = $$PWD/doc/en/*
documentation_cs.files = $$PWD/doc/cs/*
documentation_nl.files = $$PWD/doc/nl/*
molecule_library.files = $$PWD/library/*
custom_library.files = $$PWD/library/custom/*

documentation_en.path = $$[QT_INSTALL_BINS]/../share/doc/molsketch/en
documentation_cs.path = $$[QT_INSTALL_BINS]/../share/doc/molsketch/cs
documentation_nl.path = $$[QT_INSTALL_BINS]/../share/doc/molsketch/nl
molecule_library.path = $$[QT_INSTALL_BINS]/../share/molsketch/library
custom_library.path = $$[QT_INSTALL_BINS]/../share/molsketch/library/custom

contains(CONFIG, static) { SUBDIRS -= tests obabeliface }
