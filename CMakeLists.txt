## CMakeLists.txt for molsKetch
cmake_minimum_required(VERSION 2.6.0) # Depend on CMake 2.6 or better

# The name of the project
project(molsKetch)

# Include the extra modules (OpenBabel)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)

###########################################################
# Here I am adding GCC magic from KDE. With these settings#
# compiler issues are fixed and more warnings are issued. #
###########################################################
if (CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-long-long -std=iso9899:1990 -Wundef -Wcast-align")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror-implicit-function-declaration -Wchar-subscripts")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -W -Wpointer-arith -Wwrite-strings -Wformat-security")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wmissing-format-attribute -fno-common")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wnon-virtual-dtor -Wno-long-long -ansi -Wundef -Wcast-align") 
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wchar-subscripts -Wall -W -Wpointer-arith -Wformat-security")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions -fno-check-new -fno-common")
  add_definitions(-D_BSD_SOURCE)
endif (CMAKE_COMPILER_IS_GNUCXX)

# Enable CPack
include(CPack)

# Unit testing, mainly useful for developers and so defaults to off
option(ENABLE_TESTS "Enable unit testing" ON)
if(ENABLE_TESTS)
  set(QT_USE_QTTEST TRUE)
  enable_testing()
  include (CTest)
  message(STATUS "Unit testing enabled, run make test to run them")
endif(ENABLE_TESTS)


# Add support for the "make uninstall" target
configure_file("${CMAKE_MODULE_PATH}/cmake_uninstall.cmake.in"
               "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
               IMMEDIATE @ONLY)
add_custom_target(uninstall "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

# Enabling debugging symbols
set(CMAKE_BUILD_TYPE Debug)

# Enabling shared libraries
set(BUILD_SHARED_LIBS OFF)

# Setting paths
set(_obIncDir ${CMAKE_SOURCE_DIR}/include)
set(_obLinkDir ${CMAKE_SOURCE_DIR}/lib)

# Find and setup Qt4 
set(QT_MIN_VERSION "4.3.0") # we need at least version 4.3 of Qt
find_package(Qt4 REQUIRED)
# and OpenBabel2
find_package(PkgConfig)
find_package(OpenBabel2 REQUIRED)
# Check if we are using openbabel trunk... (graphsym.h is not in 2.2.x)
if (EXISTS ${OPENBABEL2_INCLUDE_DIR}/openbabel/graphsym.h)
  message(STATUS "Found OpenBabel Trunk with graphsym.h")
  set(OPENBABEL2_TRUNK TRUE) # for use in cmake
  add_definitions(-DOPENBABEL2_TRUNK) # for use in code
endif (EXISTS ${OPENBABEL2_INCLUDE_DIR}/openbabel/graphsym.h)


# Optional KDE4 for "KDE Part"
find_package(KDE4)

add_subdirectory(src)
if(ENABLE_TESTS)
  add_subdirectory(tests)
endif(ENABLE_TESTS)
