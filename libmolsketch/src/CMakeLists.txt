# CMakeLists.txt for the main sourcecode of molsKetch
cmake_policy(SET CMP0003 NEW)

# Including qt4 and OpenBabel
set(QT_USE_QTSVG TRUE)
if(FORCEQt4)
	include(${QT_USE_FILE})
endif(FORCEQt4)


# Ensure the Molsketch include directory is always first
get_directory_property(tmp_include_dirs INCLUDE_DIRECTORIES)
set_directory_properties(PROPERTIES INCLUDE_DIRECTORIES
        "${CMAKE_CURRENT_BINARY_DIR}/include;${tmp_include_dirs}")
include_directories(${OPENBABEL2_INCLUDE_DIR} 
		    ${CMAKE_CURRENT_BINARY_DIR}
		    ${CMAKE_CURRENT_BINARY_DIR}/../../obabeliface/include/molsketch
)

set(libmolsketch_HDRS
    atom.h
    atomnumberitem.h
    bond.h
    commands.h
    electronsystem.h
    element.h
    fileio.h
    graphsymitem.h
    itemplugin.h
    math2d.h
    mimemolecule.h
    minimise.h
    molecule.h
    molinputitem.h
    mollibitem.h
    molscene.h
    molview.h
    residue.h
    ring.h
    smilesitem.h
    stereocenteritem.h
    TextInputItem.h
    arrow.h
    graphicsitem.h
    grid.h
    stringify.h
    frame.h
    arrowpopup.h 
    coordinatedelegate.h 
    coordinatemodel.h 
    coordinatetableview.h
    generics.h
    moleculepopup.h
   )

set(action_HDRS
    actions/coloraction.h
    actions/linewidthaction.h
    actions/abstractitemaction.h
    abstractxmlobject.h
    actions/genericaction.h
    actions/multiaction.h
    actions/incdecaction.h
    actions/connectaction.h
    actions/minimizeaction.h
    actions/rotateaction.h
    actions/transformcommand.h
    actions/reactionarrowaction.h
    actions/periodictablewidget.h
    actions/bondtypewidget.h
    actions/drawaction.h
    actions/ringaction.h
    actions/mechanismarrowaction.h
    actions/pluginaction.h
    actions/deleteaction.h
    actions/bondtypeaction.h
    actions/itemtypewidget.h
    actions/itemtypeaction.h
    actions/arrowtypeaction.h
    actions/arrowtypewidget.h
    actions/flipbondaction.h
    actions/frameaction.h
    actions/frametypewidget.h
    actions/frametypeaction.h
    actions/itemgrouptypeaction.h
    actions/flipstereobondsaction.h
    actions/changemoleculenameaction.h
   )


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
if(FORCEQt4)
	qt4_wrap_ui(libmolsketch_UIS_H ${libmolsketch_UIS})
else(FORCEQt4)
	qt5_wrap_ui(libmolsketch_UIS_H ${libmolsketch_UIS})
endif(FORCEQt4)

# Source files
set(libmolsketch_SRCS 
    atom.cpp
    atomnumberitem.cpp
    bond.cpp
    commands.cpp
    electronsystem.cpp
    element.cpp
    fileio.cpp
    graphsymitem.cpp
    itemplugin.cpp
    mimemolecule.cpp
    minimise.cpp
    molecule.cpp
    molinputitem.cpp
    mollibitem.cpp
    molscene.cpp
    molview.cpp
    residue.cpp
    ring.cpp
    smilesitem.cpp
    stereocenteritem.cpp
    TextInputItem.cpp
    graphicsitem.cpp
    arrow.cpp
    grid.cpp
    frame.cpp
    arrowpopup.cpp
    coordinatedelegate.cpp
    coordinatemodel.cpp
    coordinatetableview.cpp
    generics.cpp
    moleculepopup.cpp
    actions/coloraction.cpp
    actions/linewidthaction.cpp
    actions/abstractitemaction.cpp
    abstractxmlobject.cpp
    actions/genericaction.cpp
    actions/multiaction.cpp
    actions/incdecaction.cpp
    actions/connectaction.cpp
    actions/minimizeaction.cpp
    actions/rotateaction.cpp
    actions/transformcommand.cpp
    actions/reactionarrowaction.cpp
    actions/periodictablewidget.cpp
    actions/bondtypewidget.cpp
    actions/drawaction.cpp
    actions/ringaction.cpp
    actions/mechanismarrowaction.cpp
    actions/pluginaction.cpp
    actions/deleteaction.cpp
    actions/bondtypeaction.cpp
    actions/itemtypewidget.cpp
    actions/itemtypeaction.cpp
    actions/arrowtypeaction.cpp
    actions/arrowtypewidget.cpp
    actions/flipbondaction.cpp
    actions/frameaction.cpp
    actions/frametypewidget.cpp
    actions/frametypeaction.cpp
    actions/itemgrouptypeaction.cpp
    actions/flipstereobondsaction.cpp
    actions/changemoleculenameaction.cpp
    )

set(toolicon_RCS tools/toolicons.qrc)

file(GLOB molsketch_UIS "*.ui")
message("UIS = ${molsketch_UIS}")
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
    set(molsketch_LIB_MOC_HDRS ${libmolsketch_HDRS} ${action_HDRS})
    set(molsketch_MOC_HDRS mainwindow.h settings.h drawwidget.h reactionarrowdialog.h mechanismarrowdialog.h)
    qt4_wrap_cpp(libmolsketch_SRCS ${molsketch_LIB_MOC_HDRS})
    # Take care of the .moc files
    qt4_automoc(${libmolsketch_SRCS})
endif(FORCEQt4)


# Create the molsKetch libraries
add_library(molsketch_LIB SHARED ${libmolsketch_SRCS} ${molsketch_UIS_H})
if(FORCEQt4)
	set_target_properties(molsketch_LIB PROPERTIES OUTPUT_NAME "molsketch")
else(FORCEQt4)
	set_target_properties(molsketch_LIB PROPERTIES OUTPUT_NAME "molsketch-qt5")
endif(FORCEQt4)
set_property(TARGET molsketch_LIB PROPERTY CXX_STANDARD 14)
set_target_properties(molsketch_LIB PROPERTIES VERSION ${MSK_VERSION})
target_link_libraries(molsketch_LIB ${QT_LIBRARIES} ${OPENBABEL2_LIBRARIES}) # TODO remove Openbabel dependency

# Install the executable and the library
install(TARGETS molsketch_LIB
    RUNTIME DESTINATION ${MSK_INSTALL_BINS}
    LIBRARY DESTINATION ${MSK_INSTALL_LIBS}
    ARCHIVE DESTINATION ${MSK_INSTALL_LIBS}/static)
