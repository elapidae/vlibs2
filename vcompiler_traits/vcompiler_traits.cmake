#########################################################################################
##  GNU Lesser General Public License Usage
##  This file may be used under the terms of the GNU Lesser General Public License
##  version 3 as published by the Free Software Foundation and appearing in the file
##  LICENSE.LGPL3 included in the packaging of this file. Please review the following
##  information to ensure the GNU Lesser General Public License version 3 requirements
##  will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
#########################################################################################

cmake_minimum_required( VERSION 3.10 )
include_guard()

#========================================================================================
message( STATUS "About include vcompiler_traits..." )

set( vcomp_path "${VLIBS_DIR}/vcompiler_traits/" )
include_directories( ${vcomp_path} )

set( V_HEADERS ${V_HEADERS} "${vcomp_path}/vcompiler_traits.h"  )

set( V_HEADERS ${V_HEADERS} "${vcomp_path}/details/vplatform.h" )
set( V_HEADERS ${V_HEADERS} "${vcomp_path}/details/vendian.h"   )
set( V_HEADERS ${V_HEADERS} "${vcomp_path}/details/vpretty_function.h" )

unset( vcomp_path )

message( STATUS "vcompiler_traits has included" )
#========================================================================================
