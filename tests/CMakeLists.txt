# define TESTDATADIR for tests that need input files
add_definitions(-DTESTDATADIR="\\"${CMAKE_SOURCE_DIR}/tests/files/\\"")
if(FORCEQt4)
	include(${QT_USE_FILE})
else(FORCEQt4)
	find_package(Qt5Test)
	INCLUDE_DIRECTORIES(${Qt5Test_INCLUDE_DIRS})
	add_definitions(${Qt5Test_DEFINITIONS})
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Test_EXECUTABLE_COMPILE_FLAGS}")
endif(FORCEQt4)

# Ensure the molsketch include directory is always first
get_directory_property(tmp_include_dirs INCLUDE_DIRECTORIES)
set_directory_properties(PROPERTIES INCLUDE_DIRECTORIES 
    "${CMAKE_CURRENT_BINARY_DIR}/../libmolsketch/src/include;${tmp_include_dirs}")
include_directories(${OPENBABEL2_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

set(tests
    valence
   )
  

foreach(test ${tests})
  message(STATUS "Test:  ${test}")
  set(test_SRCS ${test}test.cpp)
  set(test_MOC_CPPS ${test}test.cpp)
  if(FORCEQt4)
	qt4_wrap_cpp(test_MOC_SRCS ${test_MOC_CPPS})
  else(FORCEQt4)
	qt5_wrap_cpp(test_MOC_SRCS ${test_MOC_CPPS})
  endif(FORCEQt4)
  add_custom_target(${test}testmoc ALL DEPENDS ${test_MOC_SRCS})
  add_executable(${test}test ${test_SRCS})
  add_dependencies(${test}test ${test}testmoc)
  if(FORCEQt4)
	target_link_libraries(${test}test
		${QT_LIBRARIES}
		${QT_QTTEST_LIBRARY}
		molsketch_LIB)
  else(FORCEQt4)
	target_link_libraries(${test}test
		Qt5::Test
		molsketch_LIB)
	foreach(package ${Qt5RequiredPackages})
		target_link_libraries(${test}test Qt5::${package})
	endforeach(package)
  endif(FORCEQt4)
  add_test(${test}Test ${CMAKE_BINARY_DIR}/tests/${test}test)
endforeach(test ${tests})

