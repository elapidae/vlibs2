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

    #------------------------------------------------------------------------------------
    #   CORE
    HEADERS     += $$VLIBS_DIR/vlog/vlog.h
    SOURCES     += $$VLIBS_DIR/vlog/vlog.cpp

    HEADERS     += $$VLIBS_DIR/vlog/impl_vlog/position_fix.h
    SOURCES     += $$VLIBS_DIR/vlog/impl_vlog/position_fix.cpp

    HEADERS     += $$VLIBS_DIR/vlog/impl_vlog/entry.h
    SOURCES     += $$VLIBS_DIR/vlog/impl_vlog/entry.cpp

    HEADERS     += $$VLIBS_DIR/vlog/impl_vlog/logger.h
    SOURCES     += $$VLIBS_DIR/vlog/impl_vlog/logger.cpp

    HEADERS     += $$VLIBS_DIR/vlog/impl_vlog/error.h
    SOURCES     += $$VLIBS_DIR/vlog/impl_vlog/error.cpp

    #------------------------------------------------------------------------------------
    #   PRE POSIX FILE LOG
    HEADERS     += $$VLIBS_DIR/vlog/impl_vlog/pre_posix.h
    SOURCES     += $$VLIBS_DIR/vlog/impl_vlog/pre_posix.cpp

    HEADERS     += $$VLIBS_DIR/vlog/impl_vlog/log_file.h
    SOURCES     += $$VLIBS_DIR/vlog/impl_vlog/log_file.cpp

    HEADERS     += $$VLIBS_DIR/vlog/impl_vlog/rotate_log_file.h
    SOURCES     += $$VLIBS_DIR/vlog/impl_vlog/rotate_log_file.cpp

    HEADERS     += $$VLIBS_DIR/vlog/impl_vlog/shared_log.h
    SOURCES     += $$VLIBS_DIR/vlog/impl_vlog/shared_log.cpp

    HEADERS     += $$VLIBS_DIR/vlog/impl_vlog/leveled_log.h
    SOURCES     += $$VLIBS_DIR/vlog/impl_vlog/leveled_log.cpp

    #------------------------------------------------------------------------------------
    OTHER_FILES += $$VLIBS_DIR/vlog/vlog.cmake
    OTHER_FILES += $$VLIBS_DIR/vlog/README
    OTHER_FILES += $$VLIBS_DIR/vlog/readme.md
}
# vlog.pri
#========================================================================================

