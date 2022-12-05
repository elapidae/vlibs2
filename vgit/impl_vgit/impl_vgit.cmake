#########################################################################################
##  GNU Lesser General Public License Usage
##  This file may be used under the terms of the GNU Lesser General Public License
##  version 3 as published by the Free Software Foundation and appearing in the file
##  LICENSE.LGPL3 included in the packaging of this file. Please review the following
##  information to ensure the GNU Lesser General Public License version 3 requirements
##  will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
#########################################################################################


#========================================================================================
#
#   Это система автоматического подтягивания в плюсовый код информации о текущем коммите.
#
#   UPD 2019-02-06  by Elapidae
#   Вся механика перенесена в этот файл (impl_vgit.pri) чтобы случайно не затереть.
#
#========================================================================================

message( STATUS "About include vgit impl..." )

# MAIN_DIR testing and defining. ----------------------------------------------------
if ( NOT MAIN_DIR )
    set ( MAIN_DIR "${CMAKE_SOURCE_DIR}" )
else()
    message( STATUS ">>> VGIT: Variable MAIN_DIR setted, so MAIN_DIR=${MAIN_DIR} <<<" )
endif()

# define git app ------------------------------------------------------------------------
execute_process( COMMAND git --version
                 WORKING_DIRECTORY ${MAIN_DIR}
                 OUTPUT_VARIABLE VGIT_VERSION )

if ( NOT VGIT_VERSION )
    execute_process( COMMAND git-lfs --version
                     WORKING_DIRECTORY ${MAIN_DIR}
                     OUTPUT_VARIABLE VGIT_VERSION )

    if ( NOT VGIT_VERSION )
         message( FATAL_ERROR "git not found, trying git & git-lfs." )
    endif()
    set( VGIT_CMD "git-lfs" )
else()
    set( VGIT_CMD "git" )
endif()

string( STRIP ${VGIT_VERSION} VGIT_VERSION )
message( STATUS "git found, command: '${VGIT_CMD}', version: '${VGIT_VERSION}'")

# hash ------------------------------------------------------------------------------
#   Проверку на ошибки есть смысл делать только в первом вызове, далее по накатанной.
execute_process( COMMAND ${VGIT_CMD} log -n 1 --pretty=format:"%H"
                 WORKING_DIRECTORY ${MAIN_DIR}
                 OUTPUT_VARIABLE VGIT_HASH
                 ERROR_VARIABLE  VGIT_ERROR )

if ( NOT VGIT_HASH )
    message( FATAL_ERROR ">>>>>>>> git not found: \"${VGIT_ERROR}\" "
                         "(maybe repo has not inited, use "
                         "git init && git add . && git commit) <<<<<<<<" )
endif()

string(REGEX REPLACE "[\"\r\n]+" "" VGIT_HASH ${VGIT_HASH})
add_definitions( -DVGIT_HASH_ELPD=${VGIT_HASH} )

# revcount --------------------------------------------------------------------------
#   Надо найти способ вырезать лишние переносы строки
execute_process( COMMAND ${VGIT_CMD} rev-list HEAD --count
                 WORKING_DIRECTORY ${MAIN_DIR}
                 OUTPUT_VARIABLE VGIT_REVCOUNT )

string(REGEX REPLACE "[\"\r\n]+" "" VGIT_REVCOUNT ${VGIT_REVCOUNT})
add_definitions( -DVGIT_REVCOUNT_ELPD=${VGIT_REVCOUNT} )

# branch ----------------------------------------------------------------------------
execute_process( COMMAND ${VGIT_CMD} symbolic-ref --short HEAD
                 WORKING_DIRECTORY ${MAIN_DIR}
                 OUTPUT_VARIABLE VGIT_BRANCH )

if ( NOT VGIT_BRANCH )      #   На случай оторванной головы.
    set( VGIT_BRANCH " " )  #   Когда отоврана, команда дает ошибку и переменная не
endif()                     #   инициализируется. REGEX дает ошибку...
string(REGEX REPLACE "[\"\r\n]+" "" VGIT_BRANCH ${VGIT_BRANCH})
add_definitions( -DVGIT_BRANCH_ELPD=${VGIT_BRANCH} )

# author name -----------------------------------------------------------------------
execute_process( COMMAND ${VGIT_CMD} log -n 1 --pretty=format:"%an"
                 WORKING_DIRECTORY ${MAIN_DIR}
                 OUTPUT_VARIABLE VGIT_AUTHOR_NAME )

string(REGEX REPLACE "[\"\r\n]+" "" VGIT_AUTHOR_NAME ${VGIT_AUTHOR_NAME})
add_definitions( -DVGIT_AUTHOR_NAME_ELPD=${VGIT_AUTHOR_NAME} )

# author email ----------------------------------------------------------------------
execute_process( COMMAND ${VGIT_CMD} log -n 1 --pretty=format:"%ae"
                 WORKING_DIRECTORY ${MAIN_DIR}
                 OUTPUT_VARIABLE VGIT_AUTHOR_EMAIL )

string(REGEX REPLACE "[\"\r\n]+" "" VGIT_AUTHOR_EMAIL ${VGIT_AUTHOR_EMAIL})
add_definitions( -DVGIT_AUTHOR_EMAIL_ELPD=${VGIT_AUTHOR_EMAIL} )

# date ------------------------------------------------------------------------------
execute_process( COMMAND ${VGIT_CMD} log -n 1 --pretty=format:"%ci"
                 WORKING_DIRECTORY ${MAIN_DIR}
                 OUTPUT_VARIABLE VGIT_DATE )

string(REGEX REPLACE "[\"\r\n]+" "" VGIT_DATE ${VGIT_DATE})
add_definitions( -DVGIT_DATE_ELPD=${VGIT_DATE} )

# vlibs -----------------------------------------------------------------------------
# vlibs hash ------------------------------------------------------------------------
execute_process( COMMAND ${VGIT_CMD} log -n 1 --pretty=format:"%H"
                 WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
                 OUTPUT_VARIABLE VGIT_VLIBS_HASH )

string(REGEX REPLACE "[\"\r\n]+" "" VGIT_VLIBS_HASH ${VGIT_VLIBS_HASH})
add_definitions( -DVGIT_VLIBS_HASH_ELPD=${VGIT_VLIBS_HASH} )

# vlibs revcount --------------------------------------------------------------------
execute_process( COMMAND ${VGIT_CMD} rev-list HEAD --count
                 WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
                 OUTPUT_VARIABLE VGIT_VLIBS_REVCOUNT )

string(REGEX REPLACE "[\"\r\n]+" "" VGIT_VLIBS_REVCOUNT ${VGIT_VLIBS_REVCOUNT})
add_definitions( -DVGIT_VLIBS_REVCOUNT_ELPD=${VGIT_VLIBS_REVCOUNT} )

# -----------------------------------------------------------------------------------
set( VGIT_VLIBS "${VGIT_VLIBS_HASH}(${VGIT_VLIBS_REVCOUNT})" )
add_definitions( -DVLIBS_REVCOUNT=${VGIT_VLIBS_REVCOUNT} )
set( VLIBS_REVCOUNT ${VGIT_VLIBS_REVCOUNT} )

# -----------------------------------------------------------------------------------
message( STATUS "Catched git hash: ${VGIT_HASH}"
              ", rev-count: ${VGIT_REVCOUNT}"
                 ", branch: ${VGIT_BRANCH}"
            ", author-name: ${VGIT_AUTHOR_NAME}"
           ", author-email: ${VGIT_AUTHOR_EMAIL}"
                   ", date: ${VGIT_DATE}"
                  ", vlibs: ${VGIT_VLIBS}" )

message( STATUS "vgit impl has included" )
#========================================================================================
