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
message( STATUS "About include vgit_post..." )

set( VGIT_HEAD_FILE "${CMAKE_SOURCE_DIR}/.git/HEAD" )

if ( EXISTS ${VGIT_HEAD_FILE} )
    add_custom_target( vgit_regenerate ALL
                       COMMAND           cmake "${CMAKE_SOURCE_DIR}"
                       WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
                       BYPRODUCTS        ${VGIT_HEAD_FILE} )

    message( STATUS "git HEAD found in ${VGIT_HEAD_FILE}" )
else()
    message( AUTHOR_WARNING ">>>>> WARNING! "
                            "vgit_post cannot find .git/HEAD of repository. "
                            "Autorebuild by new commits is impossible." )
endif()

message( STATUS "vgit_post has included" )
#========================================================================================
