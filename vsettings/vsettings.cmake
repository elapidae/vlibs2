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

    include( "${VLIBS_DIR}/cmake/vlog.cmake"         )
    include( "${VLIBS_DIR}/cmake/vbyte_buffer.cmake" )

    include_directories( "${VLIBS_DIR}/vsettings/" )

    set( V_HEADERS ${V_HEADERS} "${VLIBS_DIR}/vsettings/vsettings.h"   )
    set( V_SOURCES ${V_SOURCES} "${VLIBS_DIR}/vsettings/vsettings.cpp" )

    message( "=== vsettings included ===" )
endif()
# vsettings.cmake
#========================================================================================
