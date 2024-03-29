#########################################################################################
##  GNU Lesser General Public License Usage
##  This file may be used under the terms of the GNU Lesser General Public License
##  version 3 as published by the Free Software Foundation and appearing in the file
##  LICENSE.LGPL3 included in the packaging of this file. Please review the following
##  information to ensure the GNU Lesser General Public License version 3 requirements
##  will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
#########################################################################################
include_guard()

#   UPD 2020-11-04 Need to run in winapi, so, posix part must be turned off.

#========================================================================================
message( STATUS "About include vlog..." )

include( "${VLIBS_DIR}/cmake/vcat.cmake"    )
include( "${VLIBS_DIR}/cmake/vchrono.cmake" )

include_directories( "${VLIBS_DIR}/vlog/" )

#----------------------------------------------------------------------------------------
#   CORE
set( V_HEADERS ${V_HEADERS} "${VLIBS_DIR}/vlog/vlog.h"   )
set( V_SOURCES ${V_SOURCES} "${VLIBS_DIR}/vlog/vlog.cpp" )

set( V_HEADERS ${V_HEADERS} "${VLIBS_DIR}/vlog/impl_vlog/entry.h"    )
set( V_SOURCES ${V_SOURCES} "${VLIBS_DIR}/vlog/impl_vlog/entry.cpp"  )

set( V_HEADERS ${V_HEADERS} "${VLIBS_DIR}/vlog/impl_vlog/logger.h"   )
set( V_SOURCES ${V_SOURCES} "${VLIBS_DIR}/vlog/impl_vlog/logger.cpp" )

set( V_HEADERS ${V_HEADERS} "${VLIBS_DIR}/vlog/impl_vlog/error.h"    )
set( V_SOURCES ${V_SOURCES} "${VLIBS_DIR}/vlog/impl_vlog/error.cpp"  )

set( V_HEADERS ${V_HEADERS} "${VLIBS_DIR}/vlog/impl_vlog/position_fix.h"    )
set( V_SOURCES ${V_SOURCES} "${VLIBS_DIR}/vlog/impl_vlog/position_fix.cpp"  )

#----------------------------------------------------------------------------------------
#   PRE POSIX FILE LOG
if ( CMAKE_SYSTEM_NAME STREQUAL Windows )

    message( WARNING " >>> vlog turned off files logging <<<" )
    add_definitions( -DV_LOG_NOT_USE_POSIX )

else()

    set( V_HEADERS ${V_HEADERS} "${VLIBS_DIR}/vlog/impl_vlog/pre_posix.h"           )
    set( V_SOURCES ${V_SOURCES} "${VLIBS_DIR}/vlog/impl_vlog/pre_posix.cpp"         )

    set( V_HEADERS ${V_HEADERS} "${VLIBS_DIR}/vlog/impl_vlog/log_file.h"            )
    set( V_SOURCES ${V_SOURCES} "${VLIBS_DIR}/vlog/impl_vlog/log_file.cpp"          )

    set( V_HEADERS ${V_HEADERS} "${VLIBS_DIR}/vlog/impl_vlog/rotate_log_file.h"     )
    set( V_SOURCES ${V_SOURCES} "${VLIBS_DIR}/vlog/impl_vlog/rotate_log_file.cpp"   )

    set( V_HEADERS ${V_HEADERS} "${VLIBS_DIR}/vlog/impl_vlog/shared_log.h"          )
    set( V_SOURCES ${V_SOURCES} "${VLIBS_DIR}/vlog/impl_vlog/shared_log.cpp"        )

    set( V_HEADERS ${V_HEADERS} "${VLIBS_DIR}/vlog/impl_vlog/leveled_log.h"         )
    set( V_SOURCES ${V_SOURCES} "${VLIBS_DIR}/vlog/impl_vlog/leveled_log.cpp"       )

endif() #   if not windows

message( STATUS "vlog has included" )
#========================================================================================
