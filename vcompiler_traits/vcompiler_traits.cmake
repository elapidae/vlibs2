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

    include_directories( "${VLIBS_DIR}/vcompiler_traits/")

    set(V_HEADERS ${V_HEADERS} "${VLIBS_DIR}/vcompiler_traits/vcompiler_traits.h")
    set(V_HEADERS ${V_HEADERS} "${VLIBS_DIR}/vcompiler_traits/vvoid_type.h")


 message( "=== vcompiler_traits includede ===" )

endif()
# # vcompiler_traits.cmake
#========================================================================================
