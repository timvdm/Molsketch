# CMakeLists.txt for the main sourcecode of molsKetch
cmake_policy(SET CMP0003 NEW)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
set(obabeliface_HDRS obabeliface.h)
set(obabeliface_SRCS obabeliface.cpp)

if (MSK_OPEN_BABEL)

  # Including qt4
  if(FORCEQt4)
          include(${QT_USE_FILE})
  endif(FORCEQt4)

  # and OpenBabel2
  find_package(PkgConfig)
  find_package(OpenBabel2 REQUIRED)
  # Check if we are using openbabel trunk... (graphsym.h is not in 2.2.x)
  if (EXISTS ${OPENBABEL2_INCLUDE_DIR}/openbabel/graphsym.h)
    message(STATUS "Found OpenBabel Trunk with graphsym.h")
    set(OPENBABEL2_TRUNK TRUE) # for use in cmake
    add_definitions(-DOPENBABEL2_TRUNK) # for use in code
  endif (EXISTS ${OPENBABEL2_INCLUDE_DIR}/openbabel/graphsym.h)

  # Ensure the Molsketch include directory is always first
  get_directory_property(tmp_include_dirs INCLUDE_DIRECTORIES)
  set_directory_properties(PROPERTIES INCLUDE_DIRECTORIES
          "${CMAKE_CURRENT_BINARY_DIR}/include;${tmp_include_dirs}")
  include_directories(${OPENBABEL2_INCLUDE_DIR}
                      ${CMAKE_CURRENT_BINARY_DIR})

  # Create the obabeliface library
  add_library(obabeliface_LIB SHARED ${obabeliface_SRCS})
  if(FORCEQt4)
          set_target_properties(obabeliface_LIB PROPERTIES OUTPUT_NAME "obabeliface")
  else(FORCEQt4)
          set_target_properties(obabeliface_LIB PROPERTIES OUTPUT_NAME "obabeliface-qt5")
  endif(FORCEQt4)
  set_property(TARGET obabeliface_LIB PROPERTY CXX_STANDARD 14)
  set_target_properties(obabeliface_LIB PROPERTIES VERSION ${MSK_VERSION} )
  if(WIN32 OR WIN64)
          set_target_properties(obabeliface_LIB PROPERTIES PREFIX "")
  endif(WIN32 OR WIN64)
  target_link_libraries(obabeliface_LIB molsketch_LIB ${QT_LIBRARIES} ${OPENBABEL2_LIBRARIES})

  # Install the executable and the library
  install(TARGETS obabeliface_LIB
          RUNTIME DESTINATION ${MSK_INSTALL_BINS}
          LIBRARY DESTINATION ${MSK_INSTALL_LIBS}
          ARCHIVE DESTINATION ${MSK_INSTALL_LIBS}/static)

endif (MSK_OPEN_BABEL)
