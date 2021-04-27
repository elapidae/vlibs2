#########################################################################################
##
##  VLIBS codebase, NIIAS
##
##  GNU Lesser General Public License Usage
##  This file may be used under the terms of the GNU Lesser General Public License
##  version 3 as published by the Free Software Foundation and appearing in the file
##  LICENSE.LGPL3 included in the packaging of this file. Please review the following
##  information to ensure the GNU Lesser General Public License version 3 requirements
##  will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
#########################################################################################
# vbyte_buffer.cmake

#========================================================================================
if ( NOT  VBYTE_BUFFER_INCLUDED )
    set ( VBYTE_BUFFER_INCLUDED TRUE )

    message( "=== include vbyte_buffer... ===" )

    include( "${VLIBS_DIR}/cmake/c++11.cmake" )
    include( "${VLIBS_DIR}/cmake/vcompiler_traits.cmake" )

    include_directories( "${VLIBS_DIR}/vbyte_buffer/" )

    set( V_HEADERS ${V_HEADERS} "${VLIBS_DIR}/vbyte_buffer/vbyte_buffer.h"   )
    set( V_SOURCES ${V_SOURCES} "${VLIBS_DIR}/vbyte_buffer/vbyte_buffer.cpp" )

    set( V_HEADERS ${V_HEADERS} "${VLIBS_DIR}/vbyte_buffer/vbyte_buffer_view.h"   )
    set( V_SOURCES ${V_SOURCES} "${VLIBS_DIR}/vbyte_buffer/vbyte_buffer_view.cpp" )

    message( "=== vbyte_buffer included ===" )
endif()
# vbyte_buffer.cmake
#========================================================================================
