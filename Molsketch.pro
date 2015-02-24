include(settings.pri)
message("Welcome to molsKetch build")
message("Current dir: $$PWD")
message("Install dir: $$PREFIX")
message("To change, use \"qmake PREFIX=<your install dir>\"")

TEMPLATE = subdirs
CONFIG += ordered

# TODO (install uninstall)-targets
SUBDIRS += \
	libmolsketch/src \
	molsketch/src \
	obabeliface \
	tests

INSTALLS += documentation_en documentation_cs documentation_nl

documentation_en.files = $$PWD/doc/en/*
documentation_cs.files = $$PWD/doc/cs/*
documentation_nl.files = $$PWD/doc/nl/*

documentation_en.path = $$PREFIX/doc/molsketch/en
documentation_cs.path = $$PREFIX/doc/molsketch/cs
documentation_nl.path = $$PREFIX/doc/molsketch/nl
