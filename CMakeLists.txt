## CMakeLists.txt for molsKetch

# The name of the project
project(molsKetch)

# Include the extra modules
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)

###########################################################
# Here I am adding GCC magic from KDE. With these settings#
# compiler issues are fixed and more warnings are issued. #
###########################################################
if (CMAKE_COMPILER_IS_GNUCXX)
	set ( CMAKE_C_FLAGS     "${CMAKE_C_FLAGS} -Wno-long-long -std=iso9899:1990 -Wundef -Wcast-align -Werror-implicit-function-declaration -Wchar-subscripts -Wall -W -Wpointer-arith -Wwrite-strings -Wformat-security -Wmissing-format-attribute -fno-common")
	set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wnon-virtual-dtor -Wno-long-long -ansi -Wundef -Wcast-align -Wchar-subscripts -Wall -W -Wpointer-arith -Wformat-security -fno-exceptions -fno-check-new -fno-common")
	add_definitions (-D_BSD_SOURCE)
endif (CMAKE_COMPILER_IS_GNUCXX)

# Enable CPack
include(CPack)

# Add support for the "make uninstall" target
configure_file(
  "${CMAKE_MODULE_PATH}/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)

add_custom_target(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

# Enabling debugging symbols
set(CMAKE_BUILD_TYPE Debug)

# Enabling shared libraries
set(BUILD_SHARED_LIBS OFF)

# Setting paths
set(_obIncDir ${CMAKE_SOURCE_DIR}/include)
set(_obLinkDir ${CMAKE_SOURCE_DIR}/lib)

# Find and setup Qt4 and OpenBabel2
set(QT_MIN_VERSION "4.3.0") # we need at least version 4.3 of Qt
cmake_minimum_required(VERSION 2.4.0) # Depend on CMake 2.4 or better

find_package(Qt4 REQUIRED)
find_package(KDE4)
find_package(OpenBabel2 REQUIRED)

# Take a look at the CMakeLists.txt in src
add_subdirectory(src)
