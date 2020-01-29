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
# vsettings.pri

#========================================================================================
isEmpty(qi_vsettings) {
    qi_vsettings = 1;
    isEmpty(qi_not_print_pri_messages): message("=== vsettings appended ===")

    isEmpty(VLIBS_DIR): error("vsettings: Need VLIBS_DIR correct path.")

    include( $$VLIBS_DIR/qmake/vlog.pri    )
    include( $$VLIBS_DIR/qmake/vposix.pri  )

    INCLUDEPATH += $$VLIBS_DIR/vsettings

    HEADERS     += $$VLIBS_DIR/vsettings/vsettings.h
    SOURCES     += $$VLIBS_DIR/vsettings/vsettings.cpp

    OTHER_FILES += $$VLIBS_DIR/vsettings/vsettings.cmake
    OTHER_FILES += $$VLIBS_DIR/vsettings/README
}
# vsettings.pri
#========================================================================================
