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
# vpoll.cmake

#========================================================================================
if ( NOT  VPOLL_INCLUDED )
    set ( VPOLL_INCLUDED TRUE )

    message( "=== include vpoll... ===" )

    include( "${VLIBS_DIR}/cmake/c++11.cmake"  )
    include( "${VLIBS_DIR}/cmake/vposix.cmake" )

    include_directories( "${VLIBS_DIR}/vpoll/" )

    set( V_HEADERS ${V_HEADERS} "${VLIBS_DIR}/vpoll/vpoll.h"   )
    set( V_SOURCES ${V_SOURCES} "${VLIBS_DIR}/vpoll/vpoll.cpp" )

    set( V_HEADERS ${V_HEADERS} "${VLIBS_DIR}/vpoll/vinvoke_iface.h" )

    set( V_HEADERS ${V_HEADERS} "${VLIBS_DIR}/vpoll/impl_vpoll/real_poll.h"   )
    set( V_SOURCES ${V_SOURCES} "${VLIBS_DIR}/vpoll/impl_vpoll/real_poll.cpp" )

    message( "=== vpoll included ===" )
endif()
# vpoll.cmake
#========================================================================================
