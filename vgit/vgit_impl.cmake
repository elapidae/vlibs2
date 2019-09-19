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
#   NB! Краткие комментарии см. в vgit_impl.pri
#========================================================================================


#========================================================================================
if ( NOT  VGIT_IMPL_INCLUDED )
    set ( VGIT_IMPL_INCLUDED TRUE )
    message( "=== including vgit impl... ===" )

    # MAIN_DIR testing and defining. ----------------------------------------------------
    if ( NOT MAIN_DIR )
        set ( MAIN_DIR "${CMAKE_SOURCE_DIR}" )
        message( ">>>>>>>> Variable MAIN_DIR has not set, "
                 "so now MAIN_DIR=${MAIN_DIR} <<<<<<<<" )
    endif()

    # hash ------------------------------------------------------------------------------
    #   Проверку на ошибки есть смысл делать только в первом вызове, далее по накатанной.
    execute_process( COMMAND git log -n 1 --pretty=format:"%H"
                     WORKING_DIRECTORY ${MAIN_DIR}
                     OUTPUT_VARIABLE VGIT_HASH
                     ERROR_VARIABLE  VGIT_ERROR )

    if ( NOT VGIT_HASH )
        message( FATAL_ERROR ">>>>>>>> GIT ERROR: \"${VGIT_ERROR}\" "
                 "(скорее всего, в папке еще нету репа, сделайте "
                 "git init && git add . && git commit) <<<<<<<<" )
    endif()

    string(REGEX REPLACE "[\"\r\n]+" "" VGIT_HASH ${VGIT_HASH})
    add_definitions( -DVGIT_HASH_ELPD=${VGIT_HASH} )

    # revcount --------------------------------------------------------------------------
    #   Надо найти способ вырезать лишние переносы строки
    execute_process( COMMAND git rev-list HEAD --count
                     WORKING_DIRECTORY ${MAIN_DIR}
                     OUTPUT_VARIABLE VGIT_REVCOUNT )

    string(REGEX REPLACE "[\"\r\n]+" "" VGIT_REVCOUNT ${VGIT_REVCOUNT})
    add_definitions( -DVGIT_REVCOUNT_ELPD=${VGIT_REVCOUNT} )

    # branch ----------------------------------------------------------------------------
    execute_process( COMMAND git symbolic-ref --short HEAD
                     WORKING_DIRECTORY ${MAIN_DIR}
                     OUTPUT_VARIABLE VGIT_BRANCH )

    if ( NOT VGIT_BRANCH )      #   На случай оторванной головы.
        set( VGIT_BRANCH " " )  #   Когда отоврана, команда дает ошибку и переменная не
    endif()                     #   инициализируется. REGEX дает ошибку...
    string(REGEX REPLACE "[\"\r\n]+" "" VGIT_BRANCH ${VGIT_BRANCH})
    add_definitions( -DVGIT_BRANCH_ELPD=${VGIT_BRANCH} )

    # author name -----------------------------------------------------------------------
    execute_process( COMMAND git log -n 1 --pretty=format:"%an"
                     WORKING_DIRECTORY ${MAIN_DIR}
                     OUTPUT_VARIABLE VGIT_AUTHOR_NAME )

    string(REGEX REPLACE "[\"\r\n]+" "" VGIT_AUTHOR_NAME ${VGIT_AUTHOR_NAME})
    add_definitions( -DVGIT_AUTHOR_NAME_ELPD=${VGIT_AUTHOR_NAME} )

    # author email ----------------------------------------------------------------------
    execute_process( COMMAND git log -n 1 --pretty=format:"%ae"
                     WORKING_DIRECTORY ${MAIN_DIR}
                     OUTPUT_VARIABLE VGIT_AUTHOR_EMAIL )

    string(REGEX REPLACE "[\"\r\n]+" "" VGIT_AUTHOR_EMAIL ${VGIT_AUTHOR_EMAIL})
    add_definitions( -DVGIT_AUTHOR_EMAIL_ELPD=${VGIT_AUTHOR_EMAIL} )

    # date ------------------------------------------------------------------------------
    execute_process( COMMAND git log -n 1 --pretty=format:"%ci"
                     WORKING_DIRECTORY ${MAIN_DIR}
                     OUTPUT_VARIABLE VGIT_DATE )

    string(REGEX REPLACE "[\"\r\n]+" "" VGIT_DATE ${VGIT_DATE})
    add_definitions( -DVGIT_DATE_ELPD=${VGIT_DATE} )

    # -----------------------------------------------------------------------------------
    message( "=== Catched git hash: ${VGIT_HASH}"
                  ", rev-count: ${VGIT_REVCOUNT}"
                     ", branch: ${VGIT_BRANCH}"
                ", author-name: ${VGIT_AUTHOR_NAME}"
               ", author-email: ${VGIT_AUTHOR_EMAIL}"
                       ", date: ${VGIT_DATE} ===" )

    message( "=== vgit impl included ===" )

    # -----------------------------------------------------------------------------------
endif()
# vgit_impl.cmake
#========================================================================================
