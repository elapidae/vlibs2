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
# vlog.pri

#========================================================================================
isEmpty(qi_vlog) {
    qi_vlog = 1;
    isEmpty(qi_not_print_pri_messages): message("=== vlog appended ===")

    isEmpty(VLIBS_DIR): error("vlog: Need VLIBS_DIR correct path.")

    include( $$VLIBS_DIR/qmake/vcat.pri    )
    include( $$VLIBS_DIR/qmake/vchrono.pri )

    INCLUDEPATH += $$VLIBS_DIR/vlog

    HEADERS     += $$VLIBS_DIR/vlog/vlog.h

#    HEADERS     += $$VLIBS_DIR/vlog/verror.h
#    SOURCES     += $$VLIBS_DIR/vlog/verror.cpp

#    HEADERS     += $$VLIBS_DIR/vlog/vlogger.h
#    SOURCES     += $$VLIBS_DIR/vlog/vlogger.cpp

#    HEADERS     += $$VLIBS_DIR/vlog/vlogentry.h
#    SOURCES     += $$VLIBS_DIR/vlog/vlogentry.cpp

#    contains( CONFIG, qt ) {
#        HEADERS += $$VLIBS_DIR/vlog/vlog_qt.h
#        SOURCES += $$VLIBS_DIR/vlog/vlog_qt.cpp
#    }
}
# vlog.pri
#========================================================================================
