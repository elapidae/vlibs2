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
# vgit.pri


#========================================================================================
isEmpty(qi_vgit) {
    qi_vgit = 1;
    isEmpty(qi_not_print_pri_messages): message("=== vgit appended ===")

    isEmpty(VLIBS_DIR): error("vgit: Need VLIBS_DIR correct path.")

    #<<< Start your code here -----------------------------------------------------------
    # Вся машинерия там, т.к. между этими палочками не очень-то развернешься.
    include( $$VLIBS_DIR/vgit/vgit_impl.pri )
    OTHER_FILES += $$VLIBS_DIR/vgit/README
    OTHER_FILES += $$VLIBS_DIR/vgit/vgit.cmake
    OTHER_FILES += $$VLIBS_DIR/vgit/vgit_impl.cmake
    OTHER_FILES += $$VLIBS_DIR/vgit/vgit_post.cmake
    #>>> Stop your code here ------------------------------------------------------------

    INCLUDEPATH += $$VLIBS_DIR/vgit

    HEADERS     += $$VLIBS_DIR/vgit/vgit.h
    SOURCES     += $$VLIBS_DIR/vgit/vgit.cpp
}
# vgit.pri
#========================================================================================
