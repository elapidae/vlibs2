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

    #------------------------------------------------------------------------------------
    #   CORE
    set( V_HEADERS ${V_HEADERS} "${VLIBS_DIR}/vlog/vlog.h"   )
    set( V_SOURCES ${V_SOURCES} "${VLIBS_DIR}/vlog/vlog.cpp" )

    set( V_HEADERS ${V_HEADERS} "${VLIBS_DIR}/vlog/impl_vlog/vlog_entry.h"    )
    set( V_SOURCES ${V_SOURCES} "${VLIBS_DIR}/vlog/impl_vlog/vlog_entry.cpp"  )

    set( V_HEADERS ${V_HEADERS} "${VLIBS_DIR}/vlog/impl_vlog/vlog_logger.h"   )
    set( V_SOURCES ${V_SOURCES} "${VLIBS_DIR}/vlog/impl_vlog/vlog_logger.cpp" )

    set( V_HEADERS ${V_HEADERS} "${VLIBS_DIR}/vlog/impl_vlog/vlog_error.h"    )
    set( V_SOURCES ${V_SOURCES} "${VLIBS_DIR}/vlog/impl_vlog/vlog_error.cpp"  )

    set( V_HEADERS ${V_HEADERS} "${VLIBS_DIR}/vlog/impl_vlog/vlog_position_fix.h"    )
    set( V_SOURCES ${V_SOURCES} "${VLIBS_DIR}/vlog/impl_vlog/vlog_position_fix.cpp"  )

    #------------------------------------------------------------------------------------
    #   PRE POSIX FILE LOG
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


    message( "=== vlog included ===" )
endif()
# vlog.cmake
#========================================================================================
