cmake_minimum_required(VERSION 3.5)

string( FIND ${CMAKE_CXX_COMPILER} "mingw" _HAS_MINGW )
if( ${_HAS_MINGW} GREATER -1 )
    set( CMAKE_SYSTEM_NAME Windows )
endif()
project( dev_vbyte_buffer LANGUAGES CXX )

unset( _HAS_MINGW )


set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

set( VLIBS_DIR ${CMAKE_CURRENT_LIST_DIR}/../.. )
include( ${VLIBS_DIR}/cmake/vbyte_buffer.cmake )

add_executable( ${PROJECT_NAME} main.cpp ${V_HEADERS} ${V_SOURCES} )
target_link_libraries( ${PROJECT_NAME} -static )
