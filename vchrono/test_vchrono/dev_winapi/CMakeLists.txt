cmake_minimum_required(VERSION 2.8)

set( VLIBS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../.." )

#set(CMAKE_SYSTEM_NAME Windows)
project( dev_win_vchrono )

#include( "${VLIBS_DIR}/cmake/gtest.cmake" )
include( "${VLIBS_DIR}/cmake/vchrono.cmake" )

add_executable( ${PROJECT_NAME} ${V_HEADERS} ${V_SOURCES} "main.cpp" )

target_link_libraries( ${PROJECT_NAME} ${V_LIBRARIES} )
