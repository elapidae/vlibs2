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
message( STATUS "About include vutf..." )

include_directories( "${VLIBS_DIR}/vutf" )

set( V_HEADERS ${V_HEADERS} ${VLIBS_DIR}/vutf/vutf8_validator.h   )
set( V_SOURCES ${V_SOURCES} ${VLIBS_DIR}/vutf/vutf8_validator.cpp )

message( STATUS "vutf has included" )

#========================================================================================
