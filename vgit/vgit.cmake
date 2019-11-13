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


#========================================================================================
#   NB! Add it after add_executable!
#   include( "${VLIBS_DIR}/cmake/vgit_post.cmake" ) # NB! НЕ ЗАБУДЬТЕ!!!!
#========================================================================================


#========================================================================================
if ( NOT  VGIT_INCLUDED )
    set ( VGIT_INCLUDED TRUE )

    message( "=== About to include vgit... ===" )

    include( "${VLIBS_DIR}/cmake/c++11.cmake" )     # need for old compiler...
    include( "${VLIBS_DIR}/vgit/impl_vgit/impl_vgit.cmake" )

    include_directories( "${VLIBS_DIR}/vgit/" )

    set( V_HEADERS ${V_HEADERS} "${VLIBS_DIR}/vgit/vgit.h"   )
    set( V_SOURCES ${V_SOURCES} "${VLIBS_DIR}/vgit/vgit.cpp" )

    message( "=== vgit included ===" )
endif()
# vgit.cmake
#========================================================================================
