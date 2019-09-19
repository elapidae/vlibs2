#########################################################################################
##
##  VLIBS codebase, NIIAS
##
##  Authors:
##  Alexandre Gromtsev aka elapidae     elapidae@yandex.ru
##  Nadezhda Churikova aka claorisel    claorisel@gmail.com
##  Ekaterina Boltenkova aka kataretta  kitkat52@yandex.ru
##  Ivan Deylid aka sid1057             ivanov.dale@gmail.com>
##
##  GNU Lesser General Public License Usage
##  This file may be used under the terms of the GNU Lesser General Public License
##  version 3 as published by the Free Software Foundation and appearing in the file
##  LICENSE.LGPL3 included in the packaging of this file. Please review the following
##  information to ensure the GNU Lesser General Public License version 3 requirements
##  will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
#########################################################################################
# vcat.cmake

#========================================================================================
if ( NOT  VCAT_INCLUDED )
    set ( VCAT_INCLUDED TRUE )

    message( "=== include vcat... ===" )

    #<<< Start your code here -----------------------------------------------------------
    include( "${VLIBS_DIR}/cmake/c++11.cmake" )
    include( "${VLIBS_DIR}/cmake/vcompiler_traits.cmake" )

    set(V_HEADERS ${V_HEADERS} "${VLIBS_DIR}/vcat/vcat_impl/vcat_iface.h")
    set(V_HEADERS ${V_HEADERS} "${VLIBS_DIR}/vcat/vcat_impl/tuple.h")
    set(V_HEADERS ${V_HEADERS} "${VLIBS_DIR}/vcat/vcat_impl/containers.h")
    #>>> Stop your code here ------------------------------------------------------------

    include_directories( "${VLIBS_DIR}/vcat/")

    
    set(V_SOURCES ${V_SOURCES} "${VLIBS_DIR}/vcat/vcat.cpp")
    set(V_HEADERS ${V_HEADERS} "${VLIBS_DIR}/vcat/vcat.h")

    message( "=== vcat included ===" )

endif()
# vcat.cmake
#========================================================================================
