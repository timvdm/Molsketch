## CMakeLists.txt for Molsketch
cmake_minimum_required(VERSION 2.6.0) # Depend on CMake 2.6 or better
cmake_policy(SET CMP0005 OLD)
cmake_policy(SET CMP0020 OLD)

# The name of the project
project(Molsketch)

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
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "1")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_SOURCE_PACKAGE_NAME "${CMAKE_PROJECT_NAME}-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
set(CPACK_SOURCE_IGNORE_FILES "/build;/.git")
include(CPack)

# Add support for the "make uninstall" target
configure_file("${CMAKE_MODULE_PATH}/cmake_uninstall.cmake.in"
               "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
               IMMEDIATE @ONLY)
add_custom_target(uninstall "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

# Enabling debugging symbols
set(CMAKE_BUILD_TYPE Debug)

# Enabling shared libraries
set(BUILD_SHARED_LIBS ON)

# Setting paths
set(_obIncDir ${CMAKE_SOURCE_DIR}/include)
set(_obLinkDir ${CMAKE_SOURCE_DIR}/lib)

# Find and setup Qt4 or Qt5
set(QT_MIN_VERSION "4.5.0") # we need at least version 4.5 of Qt
if(NOT FORCEQt4)

#	CMAKE_MINIMUM_REQUIRED_VERSION(VERSION 2.8.11)
	set(CMAKE_INCLUDE_CURRENT_DIR ON)
	set(Qt5RequiredPackages
		Core
		Widgets
		Gui
		PrintSupport
		Svg
	)

	foreach(package ${Qt5RequiredPackages})
		find_package(Qt5${package})
		INCLUDE_DIRECTORIES(${Qt5${package}_INCLUDE_DIRS})
		add_definitions(${Qt5${package}_DEFINITIONS})
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5${package}_EXECUTABLE_COMPILE_FLAGS}")
		set(QT_LIBRARIES ${QT_LIBRARIES} Qt5::${package})
	endforeach(package)

endif(NOT FORCEQt4)
# if Qt5 is not available, try Qt4
if(Qt5Core_FOUND AND Qt5Widgets_FOUND AND Qt5Gui_FOUND AND Qt5PrintSupport_FOUND AND Qt5Svg_FOUND)
	message(STATUS "Using Qt5. Use option \"-D FORCEQt4=TRUE\" to switch to Qt4.")
	set(CMAKE_AUTOMOC ON)
else(Qt5Core_FOUND AND Qt5Widgets_FOUND AND Qt5Gui_FOUND AND Qt5PrintSupport_FOUND AND Qt5Svg_FOUND)
	find_package(Qt4)
	if(NOT Qt4_FOUND)
		message(FATAL_ERROR "Could not find Qt4 or Qt5.")
	endif(NOT Qt4_FOUND)
	if(NOT FORCEQt4)
		message(STATUS "Could not find Qt5.")
	endif(NOT FORCEQt4)
	set(FORCEQt4 TRUE)
	message(STATUS "Using Qt4.")
endif(Qt5Core_FOUND AND Qt5Widgets_FOUND AND Qt5Gui_FOUND AND Qt5PrintSupport_FOUND AND Qt5Svg_FOUND)

if(FORCEQt4)
	add_definitions(-DQTVERSIONSUFFIX="")
else(FORCEQt4)
	add_definitions(-DQTVERSIONSUFFIX="\\\"-qt5\\\"")
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

################ Reading version and default paths

message(STATUS "======= Welcome to molsKetch build =======")
message(STATUS "Sources in: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "Installation directories and corresponding cmake variables (use \"-D <variableName>=<path>\" to change):")

list(APPEND mskprefix "Global prefix" MSK_INSTALL_PREFIX /usr/local "C:/Program Files/MolsKetch")
list(APPEND mskbins Executable MSK_INSTALL_BINS /bin "")
if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
	list(APPEND msklibs Libraries MSK_INSTALL_LIBS /lib64 "")
else( CMAKE_SIZEOF_VOID_P EQUAL 8 )
	list(APPEND msklibs Libraries MSK_INSTALL_LIBS /lib "")
endif( CMAKE_SIZEOF_VOID_P EQUAL 8 )
list(APPEND mskincludes Headers MSK_INSTALL_INCLUDES /include/molsketch /include)
list(APPEND msklibrary "Molecule library" MSK_INSTALL_LIBRARY /share/molsketch/library /library)
list(APPEND mskcustom "Custom molecule library" MSK_INSTALL_CUSTOM /share/molsketch/library/custom /library/custom)
list(APPEND mskdocs Documentation MSK_INSTALL_DOCS /share/doc/molsketch /doc)

file(STRINGS version TEMPVERSION)
file(STRINGS versionnick TEMPVERSIONNICK)
list(APPEND mskversion "Version" MSK_VERSION ${TEMPVERSION})
list(APPEND msknick "Nickname" MSK_VERSION_NICK ${TEMPVERSIONNICK})

foreach(varname mskprefix mskbins msklibs mskincludes msklibrary mskcustom mskdocs mskversion msknick)
	list(GET ${varname} 0 descriptor)
	list(GET ${varname} 1 variableName)
	list(GET ${varname} 2 defaultValue)
	list(LENGTH ${varname} listLength)
	if(listLength EQUAL 3 AND NOT ${variableName})
		set(${variableName} "${defaultValue}")
	endif(listLength EQUAL 3 AND NOT ${variableName})
	if(listLength EQUAL 4 AND NOT ${variableName})
		if(WIN32 OR WIN64)
			list(GET ${varname} 3 defaultValue)
		endif(WIN32 OR WIN64)
		set(${variableName} "${MSK_INSTALL_PREFIX}${defaultValue}")
	endif(listLength EQUAL 4 AND NOT ${variableName})
	message(STATUS "${descriptor}: ${variableName} = ${${variableName}}")
	add_definitions(-D${variableName}="\\\"${${variableName}}\\\"")
endforeach(varname)

set(CMAKE_INSTALL_PREFIX ${MSK_INSTALL_PREFIX})

################ Adding subdirs

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/libmolsketch/src)
add_subdirectory(libmolsketch)
add_subdirectory(obabeliface)
add_subdirectory(molsketch)
