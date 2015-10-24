# CMakeLists.txt for the main sourcecode of molsKetch
cmake_policy(SET CMP0003 NEW)

# Including qt4 and OpenBabel
if(FORCEQt4)
	include(${QT_USE_FILE})
endif(FORCEQt4)


# Ensure the Molsketch include directory is always first
get_directory_property(tmp_include_dirs INCLUDE_DIRECTORIES)
set_directory_properties(PROPERTIES INCLUDE_DIRECTORIES
        "${CMAKE_CURRENT_BINARY_DIR}/include;${tmp_include_dirs}")
include_directories(${OPENBABEL2_INCLUDE_DIR} 
                    ${CMAKE_CURRENT_BINARY_DIR})

set(obabeliface_HDRS
    obabeliface.h
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
foreach(headerFile ${obabeliface_HDRS})
  set(from "${CMAKE_CURRENT_SOURCE_DIR}/${headerFile}")
  set(to   "${CMAKE_CURRENT_BINARY_DIR}/include/molsketch/${headerFile}")
  execute_process(COMMAND ${CMAKE_COMMAND} -E ${header_cmd} "${from}" "${to}")
endforeach(headerFile ${obabeliface_HDRS})

# Source files
set(obabeliface_SRCS 
    obabeliface.cpp
    )


# Create the molsKetch libraries
add_library(obabeliface_LIB SHARED ${obabeliface_SRCS})
if(FORCEQt4)
	set_target_properties(obabeliface_LIB PROPERTIES OUTPUT_NAME "obabeliface")
else(FORCEQt4)
	set_target_properties(obabeliface_LIB PROPERTIES OUTPUT_NAME "obabeliface-qt5")
endif(FORCEQt4)
target_link_libraries(obabeliface_LIB ${QT_LIBRARIES} ${OPENBABEL2_LIBRARIES})

# Install the executable and the library
install(TARGETS obabeliface_LIB
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION ${QT_LIBRARY_DIR}
	ARCHIVE DESTINATION lib/static)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/library DESTINATION share/molsketch)
 
