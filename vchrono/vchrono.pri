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
# vchrono.pri

#========================================================================================
isEmpty(qi_vchrono) {
    qi_vchrono = 1;
    isEmpty(qi_not_print_pri_messages): message("=== vchrono appended ===")

    isEmpty(VLIBS_DIR): error("vchrono: Need VLIBS_DIR correct path.")

    include( $$VLIBS_DIR/qmake/vcompiler_traits.pri )

    INCLUDEPATH += $$VLIBS_DIR/vchrono

    HEADERS     += $$VLIBS_DIR/vchrono/vtime_point.h

    HEADERS     += $$VLIBS_DIR/vchrono/vchrono_impl/time_point_base.h

    HEADERS     += $$VLIBS_DIR/vchrono/vchrono_impl/human_readable_time.h
    SOURCES     += $$VLIBS_DIR/vchrono/vchrono_impl/human_readable_time.cpp
}
# vchrono.pri
#========================================================================================
