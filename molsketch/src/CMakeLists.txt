# CMakeLists.txt for the main sourcecode of molsKetch
cmake_policy(SET CMP0003 NEW)

# Including qt4 and OpenBabel
set(QT_USE_QTASSISTANT TRUE)
set(QT_USE_QTSVG TRUE)
if(FORCEQt4)
	include(${QT_USE_FILE})
endif(FORCEQt4)


# Ensure the Avogadro include directory is always first
get_directory_property(tmp_include_dirs INCLUDE_DIRECTORIES)
set_directory_properties(PROPERTIES INCLUDE_DIRECTORIES 
    "${CMAKE_CURRENT_BINARY_DIR}/../../libmolsketch/src/include;${tmp_include_dirs}")
include_directories(${OPENBABEL2_INCLUDE_DIR}
		    ${CMAKE_CURRENT_BINARY_DIR}
		    ${CMAKE_CURRENT_BINARY_DIR}/../../obabeliface/include/molsketch
)

#if (OPENBABEL2_TRUNK)
#  set(libmolsketch_SRCS 
#      ${libmolsketch_SRCS}
#endif (OPENBABEL2_TRUNK)

    
set(molsketch_SRCS 
    main.cpp 
    mainwindow.cpp 
    settings.cpp 
    )

# Including the resources
file(GLOB molsketch_RCS "*.qrc")
file(GLOB molsketch_UIS "*.ui")
message("UIS = ${molsketch_UIS}")
if(FORCEQt4)
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

# MOC-ing headers
set(molsketch_MOC_HDRS mainwindow.h settings.h)
if(FORCEQt4)
	qt4_wrap_cpp(molsketch_SRCS ${molsketch_MOC_HDRS})
	# Take care of the .moc files
	qt4_automoc(${molsketch_SRCS})
endif(FORCEQt4)




# Create the main executable
link_directories(${CMAKE_BINARY_DIR}/lib)
if(FORCEQt4)
	add_executable(molsketch ${molsketch_SRCS} ${molsketch_UIS_H} ${molsketch_MOC_HDRS})
	install(TARGETS molsketch DESTINATION ${MSK_INSTALL_BINS})
	# Link the code against libmolsKetch, Qt and OpenBabel
	target_link_libraries(molsketch molsketch_LIB ${QT_LIBRARIES} ${OPENBABEL2_LIBRARIES})
else(FORCEQt4)
	add_executable(molsketch-qt5 ${molsketch_SRCS} ${molsketch_UIS_H} ${molsketch_MOC_HDRS})
	install(TARGETS molsketch-qt5 DESTINATION ${MSK_INSTALL_BINS})
	# Link the code against libmolsKetch, Qt and OpenBabel
	target_link_libraries(molsketch-qt5 molsketch_LIB ${OPENBABEL2_LIBRARIES})
	foreach(package ${Qt5RequiredPackages})
		target_link_libraries(molsketch-qt5 Qt5::${package})
	endforeach(package)
endif(FORCEQt4)


# Install the documentation
install(DIRECTORY ${PROJECT_SOURCE_DIR}/doc/ DESTINATION ${MSK_INSTALL_DOCS})
install(DIRECTORY ${PROJECT_SOURCE_DIR}/library/ DESTINATION ${MSK_INSTALL_LIBRARY})

# Install menu entries on Linux
if(UNIX)
  install(FILES ${PROJECT_SOURCE_DIR}/molsketch/src/molsketch.desktop DESTINATION share/applications)
  install(FILES ${PROJECT_SOURCE_DIR}/molsketch/src/images/molsketch.xpm DESTINATION share/pixmaps)
  install(FILES ${PROJECT_SOURCE_DIR}/molsketch/src/images/molsketch.png DESTINATION share/icons/hicolor/128x128/apps)
endif(UNIX)

# Update library cache. Must be done _after_ installing the libs
if(NOT WIN32 AND NOT WIN64)
	install(CODE "execute_process(COMMAND \"ldconfig\")")
endif(NOT WIN32 AND NOT WIN64)