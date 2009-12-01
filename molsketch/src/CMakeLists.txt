# CMakeLists.txt for the main sourcecode of molsKetch
cmake_policy(SET CMP0003 NEW)

# Including qt4 and OpenBabel
set(QT_USE_QTASSISTANT TRUE)
set(QT_USE_QTSVG TRUE)
include(${QT_USE_FILE})


# Ensure the Avogadro include directory is always first
get_directory_property(tmp_include_dirs INCLUDE_DIRECTORIES)
set_directory_properties(PROPERTIES INCLUDE_DIRECTORIES 
    "${CMAKE_CURRENT_BINARY_DIR}/../../libmolsketch/src/include;${tmp_include_dirs}")
include_directories(${OPENBABEL2_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

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
qt4_automoc(${molsketch_SRCS})
qt4_add_resources(molsketch_SRCS ${molsketch_RCS})
qt4_wrap_ui(molsketch_UIS_H ${molsketch_UIS})


# Add a program icon for windows
if(WIN32)
  set(molsketch_SRCS ${molsketch_SRCS} icon.rc)
endif(WIN32)

# MOC-ing headers
set(molsketch_MOC_HDRS mainwindow.h settings.h)
qt4_wrap_cpp(molsketch_SRCS ${molsketch_MOC_HDRS})
# Take care of the .moc files
qt4_automoc(${molsketch_SRCS})




# Create the main executable
include_directories(${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/src)
link_directories(${CMAKE_BINARY_DIR}/lib)
add_executable(molsketch ${molsketch_SRCS} ${molsketch_UIS_H} ${molsketch_MOC_HDRS})
install(TARGETS molsketch DESTINATION bin)
# Link the code against libmolsKetch, Qt and OpenBabel
target_link_libraries(molsketch molsketch_LIB ${QT_LIBRARIES} ${OPENBABEL2_LIBRARIES})


# Install the documentation
install(DIRECTORY ${PROJECT_SOURCE_DIR}/doc DESTINATION share/doc/molsketch)

# Install menu entries on Linux
if(UNIX)
  install(FILES ${PROJECT_SOURCE_DIR}/molsketch/src/molsketch.desktop DESTINATION share/applications)
  install(FILES ${PROJECT_SOURCE_DIR}/molsketch/src/images/molsketch.xpm DESTINATION share/pixmaps)
  install(FILES ${PROJECT_SOURCE_DIR}/molsketch/src/images/molsketch.png DESTINATION share/icons/hicolor/128x128/apps)
endif(UNIX)
