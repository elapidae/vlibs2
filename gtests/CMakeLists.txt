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


#add_subdirectory( "${VLIBS_DIR}/vcrc/test_vcrc/" )
#add_subdirectory( "${VLIBS_DIR}/vgeometry/test_vgeometry/" )
#add_subdirectory( "${VLIBS_DIR}/vgio/keyfile/test_keyfile/" )
#add_subdirectory( "${VLIBS_DIR}/vapplication/test_vapplication/" )
#add_subdirectory( "${VLIBS_DIR}/vhdlc/test_vhdlc/" )
#add_subdirectory( "${VLIBS_DIR}/vchrono/test_vchrono/" )
#add_subdirectory( "${VLIBS_DIR}/vlog/test_vlog/" )
