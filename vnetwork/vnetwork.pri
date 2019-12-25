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
# vnetwork.pri

#========================================================================================
isEmpty(qi_vnetwork) {
    qi_vnetwork = 1;
    isEmpty(qi_not_print_pri_messages): message("=== vnetwork appended ===")

    isEmpty(VLIBS_DIR): error("vnetwork: Need VLIBS_DIR correct path.")

    include( $$VLIBS_DIR/qmake/vlog.pri    )
    include( $$VLIBS_DIR/qmake/vposix.pri  )
    include( $$VLIBS_DIR/qmake/vsignal.pri )

    INCLUDEPATH += $$VLIBS_DIR/vnetwork

    HEADERS     += $$VLIBS_DIR/vnetwork/vsocket_address.h
    SOURCES     += $$VLIBS_DIR/vnetwork/vsocket_address.cpp

    HEADERS     += $$VLIBS_DIR/vnetwork/vtcp_socket.h
    SOURCES     += $$VLIBS_DIR/vnetwork/vtcp_socket.cpp

    HEADERS     += $$VLIBS_DIR/vnetwork/vtcp_server.h
    SOURCES     += $$VLIBS_DIR/vnetwork/vtcp_server.cpp

    HEADERS     += $$VLIBS_DIR/vnetwork/vudp_socket.h
    SOURCES     += $$VLIBS_DIR/vnetwork/vudp_socket.cpp

    OTHER_FILES += $$VLIBS_DIR/vnetwork/vnetwork.cmake
    OTHER_FILES += $$VLIBS_DIR/vnetwork/README
}
# vnetwork.pri
#========================================================================================
