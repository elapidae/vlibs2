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
# vthread.cmake

#========================================================================================
if ( NOT  VTHREAD_INCLUDED )
    set ( VTHREAD_INCLUDED TRUE )

    message( "=== include vthread... ===" )

    include( "${VLIBS_DIR}/cmake/c++11.cmake"  )
    include( "${VLIBS_DIR}/cmake/vposix.cmake" )
    include( "${VLIBS_DIR}/cmake/vpoll.cmake"  )
    include( "${VLIBS_DIR}/cmake/vcompiler_traits.cmake" )

    include_directories( "${VLIBS_DIR}/vthread/" )

    set( V_HEADERS ${V_HEADERS} "${VLIBS_DIR}/vthread/vthread.h"   )
    set( V_SOURCES ${V_SOURCES} "${VLIBS_DIR}/vthread/vthread.cpp" )

    message( "=== vthread included ===" )
endif()
# vthread.cmake
#========================================================================================
