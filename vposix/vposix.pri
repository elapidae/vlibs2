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
# vposix.pri

#========================================================================================
isEmpty(qi_vposix) {
    qi_vposix = 1;
    isEmpty(qi_not_print_pri_messages): message("=== vposix appended ===")

    isEmpty(VLIBS_DIR): error("vposix: Need VLIBS_DIR correct path.")

    include( $$VLIBS_DIR/qmake/vcat.pri )
    include( $$VLIBS_DIR/qmake/vlog.pri )

    INCLUDEPATH += $$VLIBS_DIR/vposix

    #------------------------------------------------------------------------------------
    #   CORE
    HEADERS     += $$VLIBS_DIR/vposix/vposix.h
    SOURCES     += $$VLIBS_DIR/vposix/vposix.cpp

    #------------------------------------------------------------------------------------

    OTHER_FILES += $$VLIBS_DIR/vposix/vposix.cmake
}
# vposix.pri
#========================================================================================

