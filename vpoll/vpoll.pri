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
# vpoll.pri

#========================================================================================
isEmpty(qi_vpoll) {
    qi_vpoll = 1;
    isEmpty(qi_not_print_pri_messages): message("=== vpoll appended ===")

    isEmpty(VLIBS_DIR): error("vpoll: Need VLIBS_DIR correct path.")

    #include( $$VLIBS_DIR/qmake/v_has_qt.pri )
    include( $$VLIBS_DIR/qmake/vposix.pri )

    INCLUDEPATH += $$VLIBS_DIR/vpoll

    HEADERS     += $$VLIBS_DIR/vpoll/vpoll.h
    SOURCES     += $$VLIBS_DIR/vpoll/vpoll.cpp

    HEADERS     += $$VLIBS_DIR/vpoll/impl_vpoll/real_poll.h
    SOURCES     += $$VLIBS_DIR/vpoll/impl_vpoll/real_poll.cpp

    OTHER_FILES += $$VLIBS_DIR/vpoll/vpoll.cmake
}
# vpoll.pri
#========================================================================================
