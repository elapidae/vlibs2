#========================================================================================
cmake_minimum_required(VERSION 2.8)

set(CMAKE_SYSTEM_NAME Windows)
project(test_vcat_dev_winapi)

set( VLIBS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../.." )

include( "${VLIBS_DIR}/cmake/vlog.cmake"  )

add_executable( ${PROJECT_NAME} ${V_HEADERS} ${V_SOURCES} "main.cpp" )

target_link_libraries( ${PROJECT_NAME}
                       ${V_LIBRARIES}
                     )

#========================================================================================
