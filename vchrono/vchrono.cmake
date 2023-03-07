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

set( V_HEADERS ${V_HEADERS} "${impl_vchrono_path}/human_readable_time.h" )
set( V_SOURCES ${V_SOURCES} "${impl_vchrono_path}/human_readable_time.cpp" )

#-------
# Important think -- define gmtime_r or gmtime_s in system.
include( CheckSymbolExists )

# gmtime_[r/s] definition
set( CMAKE_REQUIRED_DEFINITIONS -D__STDC_WANT_LIB_EXT1__=1 )
check_symbol_exists( gmtime_r "time.h" have_gmtime_r )
check_symbol_exists( gmtime_s "time.h" have_gmtime_s )
if ( have_gmtime_r )
    add_definitions( "-DV_GMTIME_SAFE=gmtime_r" )
    message( STATUS "vchrono: found gmtime_r function (the best way)" )
elseif( have_gmtime_s )
    add_definitions( "-DV_GMTIME_SAFE=gmtime_s" )
    message( STATUS "vchrono: found gmtime_s function (c++11 way)" )
else()
    message( FATAL_ERROR "vchrono: cannot define gmtime_[r/s] function" )
endif()
unset( have_gmtime_r )
unset( have_gmtime_s )

set( V_HEADERS ${V_HEADERS} "${impl_vchrono_path}/sys_helper_vchrono.h" )
set( V_SOURCES ${V_SOURCES} "${impl_vchrono_path}/sys_helper_vchrono.cpp" )
#-------

unset( vchrono_path )
unset( impl_vchrono_path )

message( STATUS "vchrono_path has included" )
#========================================================================================
