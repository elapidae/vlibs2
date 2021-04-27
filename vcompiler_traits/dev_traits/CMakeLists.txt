cmake_minimum_required(VERSION 3.5)

string( FIND ${CMAKE_CXX_COMPILER} "mingw" _HAS_MINGW )
if( ${_HAS_MINGW} GREATER -1 )
    message( STATUS "V_HINT: Use Windows system name" )
    set( CMAKE_SYSTEM_NAME Windows )
    set( V_LIBRARIES ${V_LIBRARIES} -static )
else()
    message( STATUS "V_HINT: Use default system name" )
endif()
project( dev_traits LANGUAGES CXX )


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set( VLIBS_DIR ${CMAKE_CURRENT_LIST_DIR}/../.. )
include( ${VLIBS_DIR}/cmake/vcompiler_traits.cmake )

add_executable( dev_traits main.cpp ${V_SOURCES} ${V_HEADERS} )
target_link_libraries( ${PROJECT_NAME} ${V_LIBRARIES} )

unset( CMAKE_C_COMPILER    )
unset( QT_QMAKE_EXECUTABLE )
