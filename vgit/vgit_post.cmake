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


#========================================================================================
if ( NOT VGIT_INCLUDED )
    message( FATAL_ERROR "Следует включить vgit.cmake перед add_executable()..." )
endif()
#========================================================================================
if ( NOT VGIT_POST_INCLUDED )
    set( VGIT_POST_INCLUDED TRUE )

    message( "=== About to vgit_post... ===" )

    set( VGIT_HEAD_FILE "${CMAKE_SOURCE_DIR}/.git/HEAD" )

    if ( EXISTS ${VGIT_HEAD_FILE} )
        add_custom_target( vgit_regenerate ALL
                           COMMAND           cmake "${CMAKE_SOURCE_DIR}"
                           WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
                           BYPRODUCTS        ${VGIT_HEAD_FILE} )
    else()
        # делаю столь странным образом, чтобы логи со старых компиляторов
        # читать было приятнее.
        message( ">>>>> WARNING!" )
        message( ">>>>> vgit_post cannot find .git/HEAD of repository" )
        message( ">>>>> autorebuild by new commits is impossible."     )
    endif()

    message( "=== vgit_post included ===" )
endif()
#========================================================================================
