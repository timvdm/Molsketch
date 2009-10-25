# CMakeLists.txt for the main sourcecode of molsKetch
cmake_policy(SET CMP0003 NEW)

# Including qt4 and OpenBabel
set(QT_USE_QTASSISTANT TRUE)
set(QT_USE_QTSVG TRUE)
include(${QT_USE_FILE})


# Ensure the Molsketch include directory is always first
get_directory_property(tmp_include_dirs INCLUDE_DIRECTORIES)
set_directory_properties(PROPERTIES INCLUDE_DIRECTORIES
        "${CMAKE_CURRENT_BINARY_DIR}/include;${tmp_include_dirs}")
include_directories(${OPENBABEL2_INCLUDE_DIR} 
                    ${CMAKE_CURRENT_BINARY_DIR})

set(libmolsketch_HDRS
    atom.h
    bond.h
    element.h
    itemplugin.h
    fileio.h
    graphicsitemtypes.h
    reactionarrowdialog.h
    mechanismarrowdialog.h
    molecule.h
    molscene.h
    molinputitem.h
    molview.h
    osra.h
    residue.h
    smilesitem.h)

# Set up the include directory and symlink/copy all headers to it.
# Note: Windows lacks symlinks and so the files must be copied.
message("creating ${CMAKE_CURRENT_BINARY_DIR}/include/molsketch")
make_directory("${CMAKE_CURRENT_BINARY_DIR}/include/molsketch")
if(UNIX)
  set(header_cmd "create_symlink")
else(UNIX)
  set(header_cmd "copy_if_different")
endif(UNIX)
foreach(headerFile ${libmolsketch_HDRS})
  set(from "${CMAKE_CURRENT_SOURCE_DIR}/${headerFile}")
  set(to   "${CMAKE_CURRENT_BINARY_DIR}/include/molsketch/${headerFile}")
  execute_process(COMMAND ${CMAKE_COMMAND} -E ${header_cmd} "${from}" "${to}")
endforeach(headerFile ${libmolsketch_HDRS})



set (libmolsketch_UIS)
qt4_wrap_ui(libmolsketch_UIS_H ${libmolsketch_UIS})

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
    molinputitem.cpp
    mimemolecule.cpp
    reactionarrow.cpp
    mechanismarrow.cpp
    smilesitem.cpp
    atomnumberitem.cpp
    stereocenteritem.cpp
    reactionarrowdialog.cpp
    mechanismarrowdialog.cpp
    graphsymitem.cpp)


#if (OPENBABEL2_TRUNK)
#  set(libmolsketch_SRCS 
#      ${libmolsketch_SRCS}
#endif (OPENBABEL2_TRUNK)

    
file(GLOB molsketch_UIS "*.ui")
message("UIS = ${molsketch_UIS}")
qt4_automoc(${libmolsketch_HDRS})
qt4_add_resources(molsketch_SRCS ${molsketch_RCS})
qt4_wrap_ui(molsketch_UIS_H ${molsketch_UIS})


# MOC-ing headers
set(molsketch_LIB_MOC_HDRS molscene.h molview.h ${libmolsketch_HDRS})
set(molsketch_MOC_HDRS mainwindow.h settings.h drawwidget.h reactionarrowdialog.h mechanismarrowdialog.h)
qt4_wrap_cpp(libmolsketch_SRCS ${molsketch_LIB_MOC_HDRS})

# Take care of the .moc files
qt4_automoc(${libmolsketch_SRCS})




# Create the molsKetch libraries
add_library(molsketch_LIB SHARED ${libmolsketch_SRCS} ${molsketch_UIS_H})
set_target_properties(molsketch_LIB PROPERTIES OUTPUT_NAME "molsketch")
target_link_libraries(molsketch_LIB ${QT_LIBRARIES} ${OPENBABEL2_LIBRARIES})

# Install the executable and the library
install(TARGETS molsketch_LIB
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib/static)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/library DESTINATION share/molsketch)
