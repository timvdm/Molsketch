# CMakeLists.txt for the sourcecode of the molsKetch KPart

option(WITH_KPART "Build the KPart version of molsKetch" OFF)

if(FALSE) #WITH_KPART AND KDE4_FOUND)

# Include KDE4
include_directories( ${KDE4_INCLUDE_DIR} )

# Include molsKetch source
include_directories( ${CMAKE_SOURCE_DIR}/src )

# Set the variable with all the sourcecode
# ../molscene.cpp ../molview.cpp ../commands.cpp ../molecule.cpp ../atom.cpp ../bond.cpp ../element.cpp ../fileio.cpp ../mollibitem.cpp
set(molsketchpart_PART_SRCS molsketchpart.cpp molsketchpart_shell.cpp)
set(molsketchpart_SHELL_SRCS molsketchpart_shell.cpp molsketchpart.cpp)

# Including the resources
qt4_add_resources(molsketchpart_SHELL_SRCS molsketchpart_shell.qrc)

# Adding the targets
kde4_add_plugin(molsketchpart WITH_PREFIX ${molsketchpart_PART_SRCS})
target_link_libraries(molsketchpart molsketch_LIB ${KDE4_KPARTS_LIBS} ${OPENBABEL2_LIBRARIES})
set_target_properties(molsketchpart PROPERTIES COMPILE_FLAGS -DMSKSRCDIR="\\"${CMAKE_CURRENT_SOURCE_DIR}\\"")

kde4_add_executable(molsketchpartshell ${molsketchpart_SHELL_SRCS})
target_link_libraries(molsketchpartshell molsketch_LIB ${KDE4_KPARTS_LIBS} ${OPENBABEL2_LIBRARIES})
set_target_properties(molsketchpartshell PROPERTIES COMPILE_FLAGS -DMSKSRCDIR="\\"${CMAKE_CURRENT_SOURCE_DIR}\\"")

# Install files
install(TARGETS molsketchpart DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES partshell.rc DESTINATION  ${DATA_INSTALL_DIR}/molsketchpart)

endif(WITH_KPART AND KDE4_FOUND)