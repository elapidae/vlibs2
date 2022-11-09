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
message( STATUS "About include vchrono_path..." )

include( "${VLIBS_DIR}/cmake/vcompiler_traits.cmake" )

set ( vchrono_path "${VLIBS_DIR}/vchrono" )
set ( impl_vchrono_path "${vchrono_path}/impl_vchrono" )

include_directories( ${vchrono_path} )

set( V_HEADERS ${V_HEADERS} "${vchrono_path}/vtime_point.h" )
set( V_HEADERS ${V_HEADERS} "${vchrono_path}/vtime_meter.h" )

#----------------------------------------------------------------------------------------

set( V_SOURCES ${V_SOURCES} "${impl_vchrono_path}/vtime_point.cpp" )

set( V_HEADERS ${V_HEADERS} "${impl_vchrono_path}/time_point_base.h" )
set( V_HEADERS ${V_HEADERS} "${impl_vchrono_path}/time_meter.h" )

set( V_HEADERS ${V_HEADERS} "${impl_vchrono_path}/sys_helper_vchrono.h" )
set( V_SOURCES ${V_SOURCES} "${impl_vchrono_path}/sys_helper_vchrono.cpp" )

set( V_HEADERS ${V_HEADERS} "${impl_vchrono_path}/human_readable_time.h" )
set( V_SOURCES ${V_SOURCES} "${impl_vchrono_path}/human_readable_time.cpp" )

unset( vchrono_path )
unset( impl_vchrono_path )

message( STATUS "vchrono_path has included" )
#========================================================================================
