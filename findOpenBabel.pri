defineTest(findOpenBabel) {
	message("Trying to find OpenBabel-2.0")
	possibleOBIncDirs = \
		C:/openbabel/include \
		/usr/local/include \
		/usr/local/include/openbabel-2.0 \
		/usr/include \
		/usr/include/openbabel-2.0 \
# TODO		${GNUWIN32_DIR}/include \
		$$(OPENBABEL2_INCLUDE_DIR)
	possibleOBLibDirs = \
		C:/openbabel/lib \
		/usr/lib \
		/usr/lib64 \
		/usr/local/lib \
		/usr/local/lib64 \
# TODO		${GNUWIN32_DIR}/lib \
		$$(OPENBABEL2_LIBRARIES)

	for(dir, possibleOBIncDirs) {
		exists($$dir/openbabel/obconversion.h) {
			OBINCLUDEPATH = $$dir
			break()
		}
	}
	for(dir, possibleOBLibDirs) {
		exists($${dir}/*openbabel*) {
			OBLIBS = $${dir}
			break()
		}
	}
	isEmpty(OBINCLUDEPATH) : error("Could not find OpenBabel-2.0 includes")
	isEmpty(OBLIBS) : error("Could not find OpenBabel-2.0 libs")
	message("Found OpenBabel-2.0.  Includes: $$OBINCLUDEPATH Libs: $$OBLIBS")
	LIBS += -L$${OBLIBS} -lopenbabel
	INCLUDEPATH += $$OBINCLUDEPATH
	export(LIBS)
	export(INCLUDEPATH)
	return(true)
}

unix {
	CONFIG += link_pkgconfig
	packagesExist(openbabel-2.0) {
		message("Using pkgconfig to find OpenBabel.")
		PKGCONFIG += openbabel-2.0
	} else {
		findOpenBabel()
	}
} else {
	findOpenBabel()
}
