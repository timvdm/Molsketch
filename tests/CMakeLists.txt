# define TESTDATADIR for tests that need input files
add_definitions(-DTESTDATADIR="\\"${CMAKE_SOURCE_DIR}/tests/files/\\"")
include(${QT_USE_FILE})

include_directories("${CMAKE_SOURCE_DIR}/include" ${CMAKE_CURRENT_BINARY_DIR})

set(tests
    valence
   )
  

foreach(test ${tests})
  message(STATUS "Test:  ${test}")
  set(test_SRCS ${test}test.cpp)
  set(test_MOC_CPPS ${test}test.cpp)
  qt4_wrap_cpp(test_MOC_SRCS ${test_MOC_CPPS})
  add_custom_target(${test}testmoc ALL DEPENDS ${test_MOC_SRCS})
  add_executable(${test}test ${test_SRCS})
  add_dependencies(${test}test ${test}testmoc)
  target_link_libraries(${test}test
    ${QT_LIBRARIES}
    ${QT_QTTEST_LIBRARY}
    molsketch_LIB)
  add_test(${test}Test ${CMAKE_BINARY_DIR}/tests/${test}test)
endforeach(test ${tests})

