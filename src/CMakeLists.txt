# CMakeLists.txt for the main sourcecode of molsKetch

# Including qt4 and OpenBabel
set(QT_USE_QTASSISTANT TRUE)
set(QT_USE_QTSVG TRUE)
include(${QT_USE_FILE})
include_directories(${OPENBABEL2_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

# Set the variable with the .ui files
set(molsketch_UIS settings.ui)
qt4_wrap_ui(molsketch_UIS_H ${molsketch_UIS})

# Set the variables with main and library source
set(molsketch_LIB_SRCS 
molecule.cpp	atom.cpp 
mollibitem.cpp	bond.cpp
element.cpp	molview.cpp
molscene.cpp	periodictablewidget.cpp
commands.cpp	fileio.cpp
)
set(molsketch_SRCS main.cpp mainwindow.cpp settings.cpp )

# Including the resources
qt4_add_resources(molsketch_SRCS molsketch.qrc)

# Add a program icon for windows
if(WIN32)
  set(molsketch_SRCS ${molsketch_SRCS} icon.rc)
endif(WIN32)

# MOC-ing headers
set(molsketch_LIB_MOC_HDRS molscene.h molview.h periodictablewidget.h)
set(molsketch_MOC_HDRS mainwindow.h settings.h)
qt4_wrap_cpp(molsketch_LIB_SRCS ${molsketch_LIB_MOC_HDRS})
qt4_wrap_cpp(molsketch_SRCS ${molsketch_MOC_HDRS})

# Take care of the .moc files
qt4_automoc(${molsketch_LIB_SRCS})
qt4_automoc(${molsketch_SRCS})

# Create the molsKetch libraries
add_library(molsketch_LIB ${molsketch_LIB_SRCS})
set_target_properties(molsketch_LIB PROPERTIES OUTPUT_NAME "molsketch")
target_link_libraries(molsketch_LIB ${QT_LIBRARIES} ${OPENBABEL2_LIBRARIES})

# Create the main executable
include_directories(${CMAKE_SOURCE_DIR}/src)
link_directories(${CMAKE_BINARY_DIR}/lib)
add_executable(molsketch ${molsketch_SRCS} ${molsketch_UIS_H})

# Link the code against libmolsKetch, Qt and OpenBabel
target_link_libraries(molsketch molsketch_LIB ${QT_LIBRARIES} ${OPENBABEL2_LIBRARIES})


# Install the executable and the library
install(TARGETS molsketch_LIB
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib/static)

# Install the molcules library
install(DIRECTORY ${PROJECT_SOURCE_DIR}/library DESTINATION share/molsketch PATTERN ".svn" EXCLUDE)

# Install the documentation
install(DIRECTORY ${PROJECT_SOURCE_DIR}/doc DESTINATION share/doc/molsketch PATTERN ".svn" EXCLUDE)

# Install menu entries on Linux
if(UNIX)
  install(FILES ${PROJECT_SOURCE_DIR}/src/molsketch.desktop DESTINATION share/applications)
  install(FILES ${PROJECT_SOURCE_DIR}/src/images/molsketch.xpm DESTINATION share/pixmaps)
  install(FILES ${PROJECT_SOURCE_DIR}/src/images/molsketch.png DESTINATION share/icons/hicolor/128x128/apps)
endif(UNIX)

# Check subdirs
add_subdirectory(i18n)
add_subdirectory(part)