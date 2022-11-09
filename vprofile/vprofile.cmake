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
message( STATUS "include vprofile..." )

add_definitions( -DV_NEED_PROFILE )

#   If you'd like turn off profiling, use this directive
#   after profile in your project:
#remove_definitions( -DV_NEED_PROFILE )

include_directories( "${VLIBS_DIR}/vprofile" )

set( V_HEADERS ${V_HEADERS} "${VLIBS_DIR}/vprofile/vprofile.h"   )
set( V_SOURCES ${V_SOURCES} "${VLIBS_DIR}/vprofile/vprofile.cpp" )

message( STATUS "vprofile has included" )
#========================================================================================
