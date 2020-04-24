cmake_minimum_required(VERSION 2.8)

project(tests)

set( VLIBS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vlibs" )


set( PROJECT vcat )
add_subdirectory( "${VLIBS_DIR}/${PROJECT}/test_${PROJECT}/" )

set( PROJECT vgit )
add_subdirectory( "${VLIBS_DIR}/${PROJECT}/test_${PROJECT}/" )

set( PROJECT vcompiler_traits )
add_subdirectory( "${VLIBS_DIR}/${PROJECT}/test_${PROJECT}/" )

set( PROJECT vsignal )
add_subdirectory( "${VLIBS_DIR}/${PROJECT}/test_${PROJECT}/" )

set( PROJECT vchrono )
add_subdirectory( "${VLIBS_DIR}/${PROJECT}/test_${PROJECT}/" )

set( PROJECT vlog )
add_subdirectory( "${VLIBS_DIR}/${PROJECT}/test_${PROJECT}/" )

set( PROJECT vbyte_buffer )
add_subdirectory( "${VLIBS_DIR}/${PROJECT}/test_${PROJECT}/" )

set( PROJECT vposix )
add_subdirectory( "${VLIBS_DIR}/${PROJECT}/test_${PROJECT}/" )

set( PROJECT vtimer )
add_subdirectory( "${VLIBS_DIR}/${PROJECT}/test_${PROJECT}/" )

set( PROJECT vnetwork )
add_subdirectory( "${VLIBS_DIR}/${PROJECT}/test_${PROJECT}/" )

set( PROJECT vserial_port )
add_subdirectory( "${VLIBS_DIR}/${PROJECT}/test_${PROJECT}/" )

set( PROJECT vsettings )
add_subdirectory( "${VLIBS_DIR}/${PROJECT}/test_${PROJECT}/" )

set( PROJECT vcan_socket )
add_subdirectory( "${VLIBS_DIR}/${PROJECT}/test_${PROJECT}/" )

set( PROJECT vprofile )
add_subdirectory( "${VLIBS_DIR}/${PROJECT}/test_${PROJECT}/" )

set( PROJECT vcmdline_parser )
add_subdirectory( "${VLIBS_DIR}/${PROJECT}/test_${PROJECT}/" )

