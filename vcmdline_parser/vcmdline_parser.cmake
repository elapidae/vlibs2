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
message( STATUS "About include vcmdline_parser..." )

include( "${VLIBS_DIR}/cmake/vcat.cmake" )

include_directories( "${VLIBS_DIR}/vcmdline_parser" )

set( V_HEADERS ${V_HEADERS} "${VLIBS_DIR}/vcmdline_parser/vcmdline_parser.h"   )
set( V_SOURCES ${V_SOURCES} "${VLIBS_DIR}/vcmdline_parser/vcmdline_parser.cpp" )

message( STATUS "vcmdline_parser has included" )
#========================================================================================
