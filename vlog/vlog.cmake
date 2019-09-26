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
# vlog.cmake

#========================================================================================
if ( NOT  VLOG_INCLUDED )
    set ( VLOG_INCLUDED TRUE )

    message( "=== about include vlog... ===" )

    include( "${VLIBS_DIR}/cmake/vcat.cmake"    )
    include( "${VLIBS_DIR}/cmake/vchrono.cmake" )

    include_directories( "${VLIBS_DIR}/vlog/" )

    set( V_HEADERS ${V_HEADERS} "${VLIBS_DIR}/vlog/vlog.h" )

    set( V_HEADERS ${V_HEADERS} "${VLIBS_DIR}/vlog/vlogger.h"   )
    set( V_SOURCES ${V_SOURCES} "${VLIBS_DIR}/vlog/vlogger.cpp" )

    set( V_HEADERS ${V_HEADERS} "${VLIBS_DIR}/vlog/verror.h"   )
    set( V_SOURCES ${V_SOURCES} "${VLIBS_DIR}/vlog/verror.cpp" )

    set( V_HEADERS ${V_HEADERS} "${VLIBS_DIR}/vlog/vlogentry.h"   )
    set( V_SOURCES ${V_SOURCES} "${VLIBS_DIR}/vlog/vlogentry.cpp" )

    message( "=== vlog included ===" )
endif()
# vlog.cmake
#========================================================================================
