#========================================================================================
cmake_minimum_required( VERSION 3.10 )

project( test_vtimer LANGUAGES CXX )

set( VLIBS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../.." )
include( "${VLIBS_DIR}/cmake/cxx_standard.cmake" )

#========================================================================================
include( "${VLIBS_DIR}/cmake/gtest.cmake"  )
include( "${VLIBS_DIR}/cmake/vlog.cmake"   )
include( "${VLIBS_DIR}/cmake/vtimer.cmake" )

#========================================================================================
add_executable( ${PROJECT_NAME} ${V_HEADERS} ${V_SOURCES} "main.cpp" )

target_link_libraries( ${PROJECT_NAME} ${V_LIBRARIES} )

#========================================================================================
