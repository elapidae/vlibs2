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
# vnetwork.cmake

#========================================================================================
if ( NOT  VNETWORK_INCLUDED )
    set ( VNETWORK_INCLUDED TRUE )

    message( "=== include vnetwork... ===" )

    include( "${VLIBS_DIR}/cmake/vlog.cmake"    )
    include( "${VLIBS_DIR}/cmake/vposix.cmake"  )
    include( "${VLIBS_DIR}/cmake/vsignal.cmake" )

    include_directories( "${VLIBS_DIR}/vnetwork/" )

    set( V_HEADERS ${V_HEADERS} "${VLIBS_DIR}/vnetwork/vnetwork.h"   )
    set( V_SOURCES ${V_SOURCES} "${VLIBS_DIR}/vnetwork/vnetwork.cpp" )

    set( V_HEADERS ${V_HEADERS} "${VLIBS_DIR}/vnetwork/vsocket_address.h"   )
    set( V_SOURCES ${V_SOURCES} "${VLIBS_DIR}/vnetwork/vsocket_address.cpp" )

    set( V_HEADERS ${V_HEADERS} "${VLIBS_DIR}/vnetwork/vtcp_socket.h"   )
    set( V_SOURCES ${V_SOURCES} "${VLIBS_DIR}/vnetwork/vtcp_socket.cpp" )

    set( V_HEADERS ${V_HEADERS} "${VLIBS_DIR}/vnetwork/vtcp_server.h"   )
    set( V_SOURCES ${V_SOURCES} "${VLIBS_DIR}/vnetwork/vtcp_server.cpp" )

    set( V_HEADERS ${V_HEADERS} "${VLIBS_DIR}/vnetwork/vudp_socket.h"   )
    set( V_SOURCES ${V_SOURCES} "${VLIBS_DIR}/vnetwork/vudp_socket.cpp" )

    message( "=== vnetwork included ===" )
endif()
# vnetwork.cmake
#========================================================================================
