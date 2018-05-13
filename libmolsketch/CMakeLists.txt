# CMakeLists.txt for the main sourcecode of molsKetch's library
cmake_policy(SET CMP0003 NEW)

# Including qt4
set(QT_USE_QTSVG TRUE)
if(FORCEQt4)
        include(${QT_USE_FILE})
endif(FORCEQt4)


# Ensure the Molsketch include directory is always first
get_directory_property(tmp_include_dirs INCLUDE_DIRECTORIES)
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/obabeliface)

file(GLOB_RECURSE libmolsketch_HDRS *.h)
file(GLOB_RECURSE libmolsketch_SRCS *.cpp)
file(GLOB_RECURSE molsketch_UIS *.ui)
file(GLOB_RECURSE toolicon_RCS *.qrc)

if(FORCEQt4)
    qt4_automoc(${libmolsketch_HDRS})
    qt4_add_resources(libmolsketch_SRCS ${toolicon_RCS})
    qt4_wrap_ui(molsketch_UIS_H ${molsketch_UIS})
else(FORCEQt4)
    qt5_add_resources(libmolsketch_SRCS ${toolicon_RCS})
    qt5_wrap_ui(molsketch_UIS_H ${molsketch_UIS})
endif(FORCEQt4)


# MOC-ing headers
if(FORCEQt4)
    set(molsketch_LIB_MOC_HDRS ${libmolsketch_HDRS})
    qt4_wrap_cpp(libmolsketch_SRCS ${molsketch_LIB_MOC_HDRS})
    # Take care of the .moc files
    qt4_automoc(${libmolsketch_SRCS})
endif(FORCEQt4)


# Create the molsKetch libraries
if(MSK_STATIC_LIB)
        add_library(molsketch_LIB STATIC ${libmolsketch_SRCS} ${molsketch_UIS_H})
else(MSK_STATIC_LIB)
        add_library(molsketch_LIB SHARED ${libmolsketch_SRCS} ${molsketch_UIS_H})
endif(MSK_STATIC_LIB)
if(FORCEQt4)
        set_target_properties(molsketch_LIB PROPERTIES OUTPUT_NAME "molsketch")
else(FORCEQt4)
        set_target_properties(molsketch_LIB PROPERTIES OUTPUT_NAME "molsketch-qt5")
endif(FORCEQt4)
set_property(TARGET molsketch_LIB PROPERTY CXX_STANDARD 14)
set_target_properties(molsketch_LIB PROPERTIES VERSION ${MSK_VERSION})
target_link_libraries(molsketch_LIB ${QT_LIBRARIES})

# Install the executable and the library
install(TARGETS molsketch_LIB
    RUNTIME DESTINATION ${MSK_INSTALL_BINS}
    LIBRARY DESTINATION ${MSK_INSTALL_LIBS}
    ARCHIVE DESTINATION ${MSK_INSTALL_LIBS}/static)
