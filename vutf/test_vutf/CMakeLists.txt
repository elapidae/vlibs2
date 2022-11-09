#========================================================================================
cmake_minimum_required( VERSION 3.10 )

project( vutf LANGUAGES CXX )

set( VLIBS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../.." )
include( "${VLIBS_DIR}/cmake/cxx_standard.cmake" )

#========================================================================================
include( ${VLIBS_DIR}/cmake/vutf.cmake )

add_executable( vutf main.cpp ${V_HEADERS} ${V_SOURCES} )

#========================================================================================
