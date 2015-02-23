# CMakeLists.txt for the main sourcecode of molsKetch
cmake_policy(SET CMP0003 NEW)

# Including qt4 and OpenBabel
set(QT_USE_QTASSISTANT TRUE)
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
    abstractxmlobject.h
    arrow.h
    atom.h
    atomnumberitem.h
    bond.h
    commands.h
    electronsystem.h
    element.h
    fileio.h
    graphicsitem.h
    graphsymitem.h
    itemplugin.h
    math2d.h
    mechanismarrowdialog.h
    mechanismarrow.h
    mimemolecule.h
    minimise.h
    molecule.h
    molinputitem.h
    mollibitem.h
    molscene.h
    molview.h
    qtversionmacros.h
    reactionarrowdialog.h
    reactionarrow.h
    residue.h
    ring.h
    smilesitem.h
    stereocenteritem.h
    TextInputItem.h
   )

set(action_HDRS
    actions/abstractitemaction.h
    actions/bondtypewidget.h
    actions/coloraction.h
    actions/connectaction.h
    actions/drawaction.h
    actions/genericaction.h
    actions/incdecaction.h
    actions/linewidthaction.h
    actions/mechanismarrowaction.h
    actions/minimizeaction.h
    actions/multiaction.h
    actions/periodictablewidget.h
    actions/pluginaction.h
    actions/reactionarrowaction.h
    actions/ringaction.h
    actions/rotateaction.h
    actions/transformcommand.h
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
    abstractxmlobject.cpp
    arrow.cpp
    atom.cpp
    atomnumberitem.cpp
    bond.cpp
    commands.cpp
    electronsystem.cpp
    element.cpp
    fileio.cpp
    graphicsitem.cpp
    graphsymitem.cpp
    itemplugin.cpp
    mechanismarrow.cpp
    mechanismarrowdialog.cpp
    mimemolecule.cpp
    minimise.cpp
    molecule.cpp
    molinputitem.cpp
    mollibitem.cpp
    molscene.cpp
    molview.cpp
    reactionarrow.cpp
    reactionarrowdialog.cpp
    residue.cpp
    ring.cpp
    smilesitem.cpp
    stereocenteritem.cpp
    TextInputItem.cpp
    actions/abstractitemaction.cpp
    actions/bondtypewidget.cpp
    actions/coloraction.cpp
    actions/connectaction.cpp
    actions/drawaction.cpp
    actions/genericaction.cpp
    actions/incdecaction.cpp
    actions/linewidthaction.cpp
    actions/mechanismarrowaction.cpp
    actions/minimizeaction.cpp
    actions/multiaction.cpp
    actions/periodictablewidget.cpp
    actions/pluginaction.cpp
    actions/reactionarrowaction.cpp
    actions/ringaction.cpp
    actions/rotateaction.cpp
    actions/transformcommand.cpp
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
set(molsketch_LIB_MOC_HDRS ${libmolsketch_HDRS} ${action_HDRS})
set(molsketch_MOC_HDRS mainwindow.h settings.h drawwidget.h reactionarrowdialog.h mechanismarrowdialog.h)
if(FORCEQt4)
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
target_link_libraries(molsketch_LIB ${QT_LIBRARIES} ${OPENBABEL2_LIBRARIES})

# Install the executable and the library
install(TARGETS molsketch_LIB
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib/static)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/library DESTINATION share/molsketch)
