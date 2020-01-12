defineTest(findOpenBabel) {
        message("Trying to find OpenBabel-2.0")
        possibleOBIncDirs = \
                $${OPENBABEL_INCLUDE_DIR} \
                C:/openbabel/include \
                C:/openbabel/include/openbabel3 \
                C:/openbabel/include/openbabel-2.0 \
                /usr/local/include \
                /usr/local/include/openbabel3 \
                /usr/local/include/openbabel-2.0 \
                /usr/include \
                /usr/include/openbabel3 \
                /usr/include/openbabel-2.0 \
                ${GNUWIN32_DIR}/include
        possibleOBLibDirs = \
                $${OPENBABEL_LIBRARIES} \
                C:/openbabel/lib \
                /usr/lib \
                /usr/lib64 \
                /usr/local/lib \
                /usr/local/lib64 \
                ${GNUWIN32_DIR}/lib

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
        isEmpty(OBINCLUDEPATH) : error("Could not find OpenBabel includes (try setting OPENBABEL_INCLUDE_DIR or '-DMSK_OBABELIFACE=false')")
        isEmpty(OBLIBS) : error("Could not find OpenBabel libs (try setting OPENBABEL_LIBRARIES or '-DMSK_OBABELIFACE=false')")
        message("Found OpenBabel.  Includes: $$OBINCLUDEPATH Libs: $$OBLIBS")
        LIBS += -L$${OBLIBS} -lopenbabel
        INCLUDEPATH += $$OBINCLUDEPATH
        export(LIBS)
        export(INCLUDEPATH)
        return(true)
}

unix {
        CONFIG += link_pkgconfig
        packagesExist(openbabel-3) {
                message("Using pkgconfig to find OpenBabel.")
                PKGCONFIG += openbabel-3
        } else {
                packagesExist(openbabel-2.0) {
                        message("Using pkgconfig to find OpenBabel.")
                        PKGCONFIG += openbabel-2.0
                } else {
                        findOpenBabel()
                }
        }
} else {
        findOpenBabel()
}
