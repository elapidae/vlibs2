cmake_minimum_required(VERSION 3.5)

project( vutf LANGUAGES CXX )

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include( ../../cmake/vutf.cmake )

add_executable( vutf main.cpp ${V_SOURCES} )
