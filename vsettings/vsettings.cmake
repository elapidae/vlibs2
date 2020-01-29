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
# vsettings.cmake

#========================================================================================
if ( NOT  VSETTINGS_INCLUDED )
    set ( VSETTINGS_INCLUDED TRUE )

    message( "=== include vsettings... ===" )

    include( "${VLIBS_DIR}/cmake/vlog.cmake"    )
    include( "${VLIBS_DIR}/cmake/vposix.cmake"  )
    include( "${VLIBS_DIR}/cmake/vsignal.cmake" )

    include_directories( "${VLIBS_DIR}/vsettings/" )

    set( V_HEADERS ${V_HEADERS} "${VLIBS_DIR}/vsettings/vsocket_address.h"   )
    set( V_SOURCES ${V_SOURCES} "${VLIBS_DIR}/vsettings/vsocket_address.cpp" )

    set( V_HEADERS ${V_HEADERS} "${VLIBS_DIR}/vsettings/vtcp_socket.h"   )
    set( V_SOURCES ${V_SOURCES} "${VLIBS_DIR}/vsettings/vtcp_socket.cpp" )

    set( V_HEADERS ${V_HEADERS} "${VLIBS_DIR}/vsettings/vtcp_server.h"   )
    set( V_SOURCES ${V_SOURCES} "${VLIBS_DIR}/vsettings/vtcp_server.cpp" )

    set( V_HEADERS ${V_HEADERS} "${VLIBS_DIR}/vsettings/vudp_socket.h"   )
    set( V_SOURCES ${V_SOURCES} "${VLIBS_DIR}/vsettings/vudp_socket.cpp" )

    message( "=== vsettings included ===" )
endif()
# vsettings.cmake
#========================================================================================
