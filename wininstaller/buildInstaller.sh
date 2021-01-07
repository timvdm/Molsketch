#!/bin/bash

set -eux

#if [ ! ( MSK_BUILD -a $OPEN_BABEL_DIR -a MINGW_SYSROOT_DIR -a MSK_SOURCES ) ] then
#	echo "Need to define MSK_BUILD (currently: '$MSK_BUILD'), MSK_SOURCES (currently: '$MSK_SOURCES'), OPEN_BABEL_DIR (currently: '$OPEN_BABEL_DIR'), and MINGW_SYSROOT_DIR (currently: '$MINGW_SYSROOT_DIR')"
#fi

mkdir -p packages/org.molsketch/data
#cp "$MSK_BUILD/bin/molsketch.exe" "$MSK_SOURCES/molsketch/images/molsketch.ico" packages/org.molsketch/data/

mkdir -p packages/org.molsketch.openbabel.adapter/data
#cp "$MSK_BUILD/lib/obabeliface.dll" packages/org.molsketch.openbabel.adapter/data/

mkdir -p packages/org.openbabel.formats/data
#find "$OPEN_BABEL_DIR" -iname "*.obf" -exec cp {} packages/org.openbabel.formats/data/ \;

mkdir -p packages/org.openbabel.mainlib/data
#find "$OPEN_BABEL_DIR" -iname "*.dll" -exec cp {} packages/org.openbabel.mainlib/data/ \;
#find "$MINGW_SYSROOT_DIR" -name libstdc++-6.dll -or -name libgcc_s_sjlj-1.dll -or -name libwinpthread-1.dll -exec cp {} packages/org.openbabel.mainlib/data/ \;

#binarycreator.exe -c config/config.xml -p packages -f MolsketchInstaller.exe
