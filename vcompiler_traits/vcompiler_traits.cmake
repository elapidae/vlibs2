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
# vcompiler_traits.cmake

#========================================================================================
if ( NOT  VMACROSES_INCLUDED )
    set ( VMACROSES_INCLUDED TRUE )

    message( "=== About include vcompiler_traits... ===" )

    include( "${VLIBS_DIR}/cmake/c++11.cmake" )

    set( vcomp_path "${VLIBS_DIR}/vcompiler_traits/" )
    include_directories( ${vcomp_path} )

    set( V_HEADERS ${V_HEADERS} "${vcomp_path}/vcompiler_traits.h"  )

    set( V_HEADERS ${V_HEADERS} "${vcomp_path}/details/vplatform.h" )
    set( V_HEADERS ${V_HEADERS} "${vcomp_path}/details/vendian.h"   )
    set( V_HEADERS ${V_HEADERS} "${vcomp_path}/details/vpretty_function.h" )

    #deprecated
    set(V_HEADERS ${V_HEADERS} "${vcomp_path}/vvoid_type.h")

    unset( vcomp_path )
    message( "=== vcompiler_traits includede ===" )

endif()
# # vcompiler_traits.cmake
#========================================================================================
