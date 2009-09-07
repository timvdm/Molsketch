# CMakeLists.txt for the main sourcecode of molsKetch

cmake_policy(SET CMP0003 NEW)

# Including qt4 and OpenBabel
set(QT_USE_QTASSISTANT TRUE)
set(QT_USE_QTSVG TRUE)
include(${QT_USE_FILE})
include_directories(${OPENBABEL2_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

set(libmolsketch_HDRS
    itemplugin.h
    smilesitem.h)

# Source files
set(libmolsketch_SRCS 
    molecule.cpp	
    atom.cpp 
    mollibitem.cpp
    bond.cpp
    element.cpp	
    molview.cpp
    molscene.cpp
    commands.cpp	
    fileio.cpp
    minimise.cpp
    TextInputItem.cpp
    osra.cpp
    electronsystem.cpp
    residue.cpp
    # connect tool items
    itemplugin.cpp
    mimemolecule.cpp
    smilesitem.cpp

)

if (OPENBABEL2_TRUNK)
  set(libmolsketch_SRCS 
      ${libmolsketch_SRCS}
      graphsymitem.cpp)
endif (OPENBABEL2_TRUNK)

    
set(molsketch_SRCS 
    main.cpp 
    mainwindow.cpp 
    settings.cpp 
    drawwidget.cpp)

# Including the resources
file(GLOB molsketch_RCS "*.qrc")
file(GLOB molsketch_UIS "*.ui")
qt4_automoc(${molsketch_SRCS} ${libmolsketch_HDRS})
qt4_add_resources(molsketch_SRCS ${molsketch_RCS})
qt4_wrap_ui(molsketch_UIS_H ${molsketch_UIS})


# Add a program icon for windows
if(WIN32)
  set(molsketch_SRCS ${molsketch_SRCS} icon.rc)
endif(WIN32)

# MOC-ing headers
set(molsketch_LIB_MOC_HDRS molscene.h molview.h ${libmolsketch_HDRS})
set(molsketch_MOC_HDRS mainwindow.h settings.h drawwidget.h)
qt4_wrap_cpp(libmolsketch_SRCS ${molsketch_LIB_MOC_HDRS})
qt4_wrap_cpp(molsketch_SRCS ${molsketch_MOC_HDRS})

# Take care of the .moc files
qt4_automoc(${libmolsketch_SRCS})
qt4_automoc(${molsketch_SRCS})








# Create the molsKetch libraries
add_library(molsketch_LIB ${libmolsketch_SRCS})
set_target_properties(molsketch_LIB PROPERTIES OUTPUT_NAME "molsketch")
target_link_libraries(molsketch_LIB ${QT_LIBRARIES} ${OPENBABEL2_LIBRARIES})

# Create the main executable
include_directories(${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/src)
link_directories(${CMAKE_BINARY_DIR}/lib)
add_executable(molsketch ${molsketch_SRCS} ${molsketch_UIS_H} ${molsketch_MOC_HDRS})
install(TARGETS molsketch DESTINATION bin)

# Link the code against libmolsKetch, Qt and OpenBabel
target_link_libraries(molsketch molsketch_LIB ${QT_LIBRARIES} ${OPENBABEL2_LIBRARIES})


# Install the executable and the library
install(TARGETS molsketch_LIB
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib/static)

# Install the molcules library
install(DIRECTORY ${PROJECT_SOURCE_DIR}/library DESTINATION share/molsketch)

# Install the documentation
install(DIRECTORY ${PROJECT_SOURCE_DIR}/doc DESTINATION share/doc/molsketch)

# Install menu entries on Linux
if(UNIX)
  install(FILES ${PROJECT_SOURCE_DIR}/src/molsketch.desktop DESTINATION share/applications)
  install(FILES ${PROJECT_SOURCE_DIR}/src/images/molsketch.xpm DESTINATION share/pixmaps)
  install(FILES ${PROJECT_SOURCE_DIR}/src/images/molsketch.png DESTINATION share/icons/hicolor/128x128/apps)
endif(UNIX)

# Check subdirs
add_subdirectory(i18n)
add_subdirectory(part)
