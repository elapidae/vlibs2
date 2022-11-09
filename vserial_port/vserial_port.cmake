#########################################################################################
##  GNU Lesser General Public License Usage
##  This file may be used under the terms of the GNU Lesser General Public License
##  version 3 as published by the Free Software Foundation and appearing in the file
##  LICENSE.LGPL3 included in the packaging of this file. Please review the following
##  information to ensure the GNU Lesser General Public License version 3 requirements
##  will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
#########################################################################################
include_guard()

#========================================================================================
message( STATUS "include vserial_port..." )

include( "${VLIBS_DIR}/cmake/vlog.cmake"    )
include( "${VLIBS_DIR}/cmake/vposix.cmake"  )
include( "${VLIBS_DIR}/cmake/vsignal.cmake" )

include_directories( "${VLIBS_DIR}/vserial_port/" )

set( V_HEADERS ${V_HEADERS} "${VLIBS_DIR}/vserial_port/vserial_port.h"   )
set( V_SOURCES ${V_SOURCES} "${VLIBS_DIR}/vserial_port/vserial_port.cpp" )

set( V_HEADERS ${V_HEADERS} "${VLIBS_DIR}/vserial_port/vterminal_options.h"   )
set( V_SOURCES ${V_SOURCES} "${VLIBS_DIR}/vserial_port/vterminal_options.cpp" )

message( STATUS "vserial_port has included" )
#========================================================================================
