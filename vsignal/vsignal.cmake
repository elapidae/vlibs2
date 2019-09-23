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
# vsignal.cmake

#========================================================================================

if ( NOT  VSIGNAL_INCLUDED )
    set ( VSIGNAL_INCLUDED TRUE )

    message( "=== About include vsignal... ===" )

    include_directories( "${VLIBS_DIR}/vsignal/" )
    
    set( V_HEADERS ${V_HEADERS} "${VLIBS_DIR}/vsignal/vsignal.h" ) 

    message( "=== vsignal included ===" )
endif()
# vsignal.cmake
#========================================================================================
