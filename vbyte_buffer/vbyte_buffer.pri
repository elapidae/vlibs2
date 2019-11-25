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
# vbyte_buffer.pri

#========================================================================================
isEmpty(qi_vbyte_buffer) {
    qi_vbyte_buffer = 1;
    isEmpty(qi_not_print_pri_messages): message("=== vbyte_buffer appended ===")

    isEmpty(VLIBS_DIR): error("vbyte_buffer: Need VLIBS_DIR correct path.")

    #include( $$VLIBS_DIR/qmake/vcompiler_traits.pri )

    INCLUDEPATH += $$VLIBS_DIR/vbyte_buffer

    HEADERS     += $$VLIBS_DIR/vbyte_buffer/vbyte_buffer.h
    SOURCES     += $$VLIBS_DIR/vbyte_buffer/vbyte_buffer.cpp

    HEADERS     += $$VLIBS_DIR/vbyte_buffer/vbyte_buffer_view.h
    SOURCES     += $$VLIBS_DIR/vbyte_buffer/vbyte_buffer_view.cpp

    OTHER_FILES += $$VLIBS_DIR/vbyte_buffer/vbyte_buffer.cmake
}
# vbyte_buffer.pri
#========================================================================================

