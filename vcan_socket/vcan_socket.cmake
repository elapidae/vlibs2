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
include_guard()

#========================================================================================
message( STATUS "About include vcan_socket..." )

include( "${VLIBS_DIR}/cmake/vsignal.cmake" )
include( "${VLIBS_DIR}/cmake/vposix.cmake" )

include_directories( "${VLIBS_DIR}/vcan_socket" )

set( V_HEADERS ${V_HEADERS} "${VLIBS_DIR}/vcan_socket/vcan_socket.h"   )
set( V_SOURCES ${V_SOURCES} "${VLIBS_DIR}/vcan_socket/vcan_socket.cpp" )

message( STATUS "vcan_socket has included" )
#========================================================================================
