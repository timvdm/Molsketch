message("Welcome to molsKetch build")
message("current dir: $$PWD")

TEMPLATE = subdirs
CONFIG += ordered

# TODO (install uninstall)-targets
SUBDIRS += \
	libmolsketch/src \
	molsketch/src \
	obabeliface \
	tests

program.files=bin/molsketch
program.path=$(bindir)

library.files=lib/*
library.path=$(libdir)

includes.files=library/src/*.h
includes.path=$(includedir)/molsketch

documentation.files=doc/*
documentation.path=$(docdir)/molsketch

INSTALLS += program library includes documentation
