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
# vprofile.pri

#========================================================================================
isEmpty(qi_vprofile) {
    qi_vprofile = 1;
    isEmpty(qi_not_print_pri_messages): message("=== vprofile appended ===")

    isEmpty(VLIBS_DIR): error("vprofile: Need VLIBS_DIR correct path.")

    DEFINES += V_NEED_PROFILE

    #   If you'd like turn off profiling, use this directive
    #   after profile in your project:
    #DEFINES -= V_NEED_PROFILE

    include( $$VLIBS_DIR/qmake/v_has_qt.pri )

    INCLUDEPATH += $$VLIBS_DIR/vprofile

    HEADERS     += $$VLIBS_DIR/vprofile/vprofile.h

    HEADERS     += $$VLIBS_DIR/vprofile/impl_vprofile/profile.h
    SOURCES     += $$VLIBS_DIR/vprofile/impl_vprofile/profile.cpp

    contains(DEFINES, V_HAS_QT): include( $$VLIBS_DIR/qmake/vsettings.pri )
    contains(DEFINES, V_HAS_QT): HEADERS += $$VLIBS_DIR/vprofile/vprofile_widget.h
    contains(DEFINES, V_HAS_QT): SOURCES += $$VLIBS_DIR/vprofile/vprofile_widget.cpp

    OTHER_FILES += $$VLIBS_DIR/vprofile/vprofile.cmake
}
# vprofile.pri
#========================================================================================

