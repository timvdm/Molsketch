# define TESTDATADIR for tests that need input files
add_definitions(-DTESTDATADIR="\\"${CMAKE_SOURCE_DIR}/tests/files/\\"")

set(QT_USE_QTSVG TRUE)


# Ensure the molsketch include directory is always first
get_directory_property(tmp_include_dirs INCLUDE_DIRECTORIES)
set_directory_properties(PROPERTIES INCLUDE_DIRECTORIES 
    "${CMAKE_CURRENT_BINARY_DIR}/../libmolsketch/src/include;${tmp_include_dirs}")
include_directories(${OPENBABEL2_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

set(test_SRCS
    main.cpp 
    coordinatestest.cpp 
    valencetest.cpp 
    drawingtest.cpp 
    xmltest.cpp
    renderingtest.cpp
   )
   
set(test_HDRS
    coordinatestest.h 
    valencetest.h 
    drawingtest.h 
    xmltest.h
    renderingtest.h
)

link_directories(${CMAKE_BINARY_DIR}/lib)

if(FORCEQt4)
    include(${QT_USE_FILE})
    qt4_wrap_cpp(test_SRCS ${test_HDRS})
    qt4_automoc(${test_SRCS})
    add_executable(molsketch-test ${test_SRCS})
    target_link_libraries(molsketch-test molsketch_LIB ${QT_LIBRARIES})
    add_test(molsketch-test ${CMAKE_BINARY_DIR}/molsketch-Test)
    set_property(TARGET molsketch-test PROPERTY CXX_STANDARD 14)
else(FORCEQt4)
    find_package(Qt5Test)
    INCLUDE_DIRECTORIES(${Qt5Test_INCLUDE_DIRS})
    add_definitions(${Qt5Test_DEFINITIONS})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Test_EXECUTABLE_COMPILE_FLAGS}")
    add_executable(molsketch-test-qt5 ${test_SRCS})
    target_link_libraries(molsketch-test-qt5 molsketch_LIB Qt5Test)
    foreach(package ${Qt5RequiredPackages})
        target_link_libraries(molsketch-test-qt5 Qt5::${package})
    endforeach(package)
    add_test(molsketch-qt5-Test ${CMAKE_BINARY_DIR}/molsketch-qt5-Test)
    set_property(TARGET molsketch-test-qt5 PROPERTY CXX_STANDARD 14)
endif(FORCEQt4)

