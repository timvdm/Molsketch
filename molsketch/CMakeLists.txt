# CMakeLists.txt for the main sourcecode of molsKetch
cmake_policy(SET CMP0003 NEW)

# Including qt4
set(QT_USE_QTSVG TRUE)
if(FORCEQt4)
        include(${QT_USE_FILE})
endif(FORCEQt4)

# Ensure the molsketch include directory is always first
get_directory_property(tmp_include_dirs INCLUDE_DIRECTORIES)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}
                    ${CMAKE_SOURCE_DIR}/libmolsketch
                    ${CMAKE_SOURCE_DIR}/obabeliface
)

file(GLOB_RECURSE molsketch_SRCS *.cpp)
file(GLOB_RECURSE molsketch_MOC_HDRS *.h)
file(GLOB_RECURSE molsketch_RCS *.qrc)
file(GLOB_RECURSE molsketch_UIS *.ui)

if(FORCEQt4)
        qt4_wrap_cpp(molsketch_SRCS ${molsketch_MOC_HDRS})
        qt4_automoc(${molsketch_SRCS})
        qt4_add_resources(molsketch_SRCS ${molsketch_RCS})
        qt4_wrap_ui(molsketch_UIS_H ${molsketch_UIS})
else(FORCEQt4)
        qt5_add_resources(molsketch_SRCS ${molsketch_RCS})
        qt5_wrap_ui(molsketch_UIS_H ${molsketch_UIS})
endif(FORCEQt4)


# Add a program icon for windows
if(WIN32)
  set(molsketch_SRCS ${molsketch_SRCS} icon.rc)
endif(WIN32)

# Create the main executable
add_definitions( -fexceptions )
link_directories(${CMAKE_BINARY_DIR}/lib)
set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/images/molsketch.ico")
add_executable(molsketch ${molsketch_SRCS} ${molsketch_UIS_H} ${molsketch_MOC_HDRS} ${APP_ICON_RESOURCE_WINDOWS})
install(TARGETS molsketch DESTINATION ${MSK_INSTALL_BINS})
set_property(TARGET molsketch PROPERTY CXX_STANDARD 14)
set_property(TARGET molsketch PROPERTY INSTALL_RPATH ${MSK_INSTALL_LIBS})
target_link_libraries(molsketch molsketch_LIB)
if(FORCEQt4)
        # Link the code against libmolsKetch and Qt
        target_link_libraries(molsketch ${QT_LIBRARIES})
else(FORCEQt4)
        foreach(package ${Qt5RequiredPackages})
                target_link_libraries(molsketch Qt5::${package})
        endforeach(package)
endif(FORCEQt4)
set_target_properties(molsketch PROPERTIES OUTPUT_NAME "molsketch${QTVERSIONSUFFIX}")

# Install the documentation
install(DIRECTORY ${PROJECT_SOURCE_DIR}/doc/ DESTINATION ${MSK_INSTALL_DOCS})

# Install menu entries on Linux
if(UNIX)
  install(FILES ${PROJECT_SOURCE_DIR}/molsketch/images/molsketch.svg DESTINATION share/icons/hicolor/scalable/apps)
  install(FILES ${PROJECT_SOURCE_DIR}/molsketch/images/molsketch.xpm DESTINATION share/pixmaps)
  install(FILES ${PROJECT_SOURCE_DIR}/molsketch/images/application-x-molsketch.svg DESTINATION share/icons/hicolor/scalable/mimetypes)
  install(FILES ${PROJECT_SOURCE_DIR}/molsketch/images/application-x-molsketch-molecule.svg DESTINATION share/icons/hicolor/scalable/mimetypes)
  install(FILES ${PROJECT_SOURCE_DIR}/molsketch/molsketch.xml DESTINATION share/mime/packages)
  install(FILES ${PROJECT_SOURCE_DIR}/molsketch/molsketch.desktop DESTINATION share/applications)
  install(FILES ${PROJECT_SOURCE_DIR}/molsketch/net.sourceforge.molsketch.appdata.xml DESTINATION share/metainfo)
endif(UNIX)
