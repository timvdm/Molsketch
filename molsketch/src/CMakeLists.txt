# CMakeLists.txt for the main sourcecode of molsKetch
cmake_policy(SET CMP0003 NEW)

# Including qt4 and OpenBabel
set(QT_USE_QTSVG TRUE)
if(FORCEQt4)
	include(${QT_USE_FILE})
endif(FORCEQt4)

# Ensure the molsketch include directory is always first
get_directory_property(tmp_include_dirs INCLUDE_DIRECTORIES)
set_directory_properties(PROPERTIES INCLUDE_DIRECTORIES 
    "${CMAKE_CURRENT_BINARY_DIR}/../../libmolsketch/src/include;${tmp_include_dirs}")
include_directories(${OPENBABEL2_INCLUDE_DIR}
		    ${CMAKE_CURRENT_BINARY_DIR}
		    ${CMAKE_CURRENT_BINARY_DIR}/../../obabeliface/include/molsketch
)
    
set(molsketch_SRCS
    main.cpp
    mainwindow.cpp
    settings.cpp
    librarylistwidget.cpp
    helpforemptytoolbox.cpp
    )

set(molsketch_MOC_HDRS
    mainwindow.h
    settings.h
    librarylistwidget.h
    helpforemptytoolbox.h
    )

# Including the resources
file(GLOB molsketch_RCS "*.qrc")
file(GLOB molsketch_UIS "*.ui")
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
link_directories(${CMAKE_BINARY_DIR}/lib)
if(FORCEQt4)
	add_executable(molsketch ${molsketch_SRCS} ${molsketch_UIS_H} ${molsketch_MOC_HDRS})
	install(TARGETS molsketch DESTINATION ${MSK_INSTALL_BINS})
	# Link the code against libmolsKetch, Qt and OpenBabel
	target_link_libraries(molsketch molsketch_LIB ${QT_LIBRARIES})
	set_property(TARGET molsketch PROPERTY CXX_STANDARD 14)
else(FORCEQt4)
	add_executable(molsketch-qt5 ${molsketch_SRCS} ${molsketch_UIS_H} ${molsketch_MOC_HDRS})
	install(TARGETS molsketch-qt5 DESTINATION ${MSK_INSTALL_BINS})
	# Link the code against libmolsKetch, Qt and OpenBabel
	target_link_libraries(molsketch-qt5 molsketch_LIB)
	set_property(TARGET molsketch-qt5 PROPERTY CXX_STANDARD 14)
	foreach(package ${Qt5RequiredPackages})
		target_link_libraries(molsketch-qt5 Qt5::${package})
	endforeach(package)
endif(FORCEQt4)


# Install the documentation
install(DIRECTORY ${PROJECT_SOURCE_DIR}/doc/ DESTINATION ${MSK_INSTALL_DOCS})

# Install menu entries on Linux
if(UNIX)
  install(FILES ${PROJECT_SOURCE_DIR}/molsketch/src/molsketch.desktop DESTINATION share/applications)
  install(FILES ${PROJECT_SOURCE_DIR}/molsketch/src/images/molsketch.xpm DESTINATION share/pixmaps)
  install(FILES ${PROJECT_SOURCE_DIR}/molsketch/src/images/molsketch.svg DESTINATION share/icons/hicolor/scalable/apps)
endif(UNIX)
