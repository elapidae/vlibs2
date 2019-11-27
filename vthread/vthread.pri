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
# vthread.pri

#========================================================================================
isEmpty(qi_vthread) {
    qi_vthread = 1;
    isEmpty(qi_not_print_pri_messages): message("=== vthread appended ===")

    isEmpty(VLIBS_DIR): error("vthread: Need VLIBS_DIR correct path.")

    include( $$VLIBS_DIR/qmake/vposix.pri )
    include( $$VLIBS_DIR/qmake/vpoll.pri  )

    INCLUDEPATH += $$VLIBS_DIR/vthread

    HEADERS     += $$VLIBS_DIR/vthread/vthread.h
    SOURCES     += $$VLIBS_DIR/vthread/vthread.cpp

    OTHER_FILES += $$VLIBS_DIR/vthread/vthread.cmake
}
# vthread.pri
#========================================================================================
