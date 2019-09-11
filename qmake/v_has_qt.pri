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


#========================================================================================
isEmpty(qi_v_has_qt) {
    qi_v_has_qt = 1;
    isEmpty(qi_not_print_pri_messages): message("=== v_has_qt appended ===")

    #<<< Start your code here -----------------------------------------------------------
    contains( CONFIG, qt ) {
        DEFINES += V_HAS_QT
        isEmpty(qi_not_print_pri_messages): message("=== macro V_HAS_QT added ===")
    }
    #>>> Stop your code here ------------------------------------------------------------
}
# v_has_qt.pri
#========================================================================================
