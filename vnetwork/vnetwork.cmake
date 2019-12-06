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

    include( "${VLIBS_DIR}/cmake/c++11.cmake"  )
    include( "${VLIBS_DIR}/cmake/vposix.cmake" )

    include_directories( "${VLIBS_DIR}/vnetwork/" )

#    set( V_HEADERS ${V_HEADERS} "${VLIBS_DIR}/vnetwork/vinvoke_iface.h"   )
#    set( V_SOURCES ${V_SOURCES} "${VLIBS_DIR}/vnetwork/vinvoke_iface.cpp" )

#    set( V_HEADERS ${V_HEADERS} "${VLIBS_DIR}/vnetwork/vapplication.h"   )
#    set( V_SOURCES ${V_SOURCES} "${VLIBS_DIR}/vnetwork/vapplication.cpp" )

#    set( V_HEADERS ${V_HEADERS} "${VLIBS_DIR}/vnetwork/vthread.h"   )
#    set( V_SOURCES ${V_SOURCES} "${VLIBS_DIR}/vnetwork/vthread.cpp" )

#    set( V_HEADERS ${V_HEADERS} "${VLIBS_DIR}/vnetwork/impl_vnetwork/poll_context.h"   )
#    set( V_SOURCES ${V_SOURCES} "${VLIBS_DIR}/vnetwork/impl_vnetwork/poll_context.cpp" )

    message( "=== vnetwork included ===" )
endif()
# vnetwork.cmake
#========================================================================================
