function(findOpenBabel)
  find_path(OPENBABEL_INCLUDE_DIR openbabel/obconversion.h
      ${OPENBABEL_INCLUDE_DIR}
      C:/openbabel/include
      C:/openbabel/include/openbabel3
      C:/openbabel/include/openbabel-2.0
      /usr/local/include
      /usr/local/include/openbabel3
      /usr/local/include/openbabel-2.0
      /usr/include
      /usr/include/openbabel3
      /usr/include/openbabel-2.0
      ${GNUWIN32_DIR}/include
  )

  find_library(OPENBABEL_LIBRARIES NAMES openbabel
      PATHS
      ${OPENBABEL2_LIBRARIES}
      C:/openbabel/lib
      /usr/lib
      /usr/lib64
      /usr/local/lib
      /usr/local/lib64
      ${GNUWIN32_DIR}/lib
  )

  if(NOT OPENBABEL_INCLUDE_DIR)
    message(FATAL_ERROR "Could not find OpenBabel includes (try setting OPENBABEL_INCLUDE_DIR or '-DMSK_OBABELIFACE=false')")
  endif(NOT OPENBABEL_INCLUDE_DIR)
  if(NOT OPENBABEL_LIBRARIES)
    message(FATAL_ERROR "Could not find OpenBabel libs (try setting OPENBABEL_LIBRARIES or '-DMSK_OBABELIFACE=false')")
  endif(NOT OPENBABEL_LIBRARIES)

  set(OPENBABEL_FOUND true)
  message(STATUS "Found OpenBabel.  Includes: ${OPENBABEL_INCLUDE_DIR} Libs: ${OPENBABEL_LIBRARIES}")
endfunction(findOpenBabel)

if(NOT WIN32)
  pkg_search_module(OPENBABEL openbabel-3)
  if(NOT OPENBABEL_FOUND)
    pkg_search_module(OPENBABEL openbabel-2.0)
    if(NOT OPENBABEL_FOUND)
      findOpenBabel()
    endif(NOT OPENBABEL_FOUND)
  endif(NOT OPENBABEL_FOUND)
else(NOT WIN32)
  findOpenBabel()
endif(NOT WIN32)
