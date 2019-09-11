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
isEmpty(qi_gtest) {
    qi_gtest = 1;
    isEmpty(qi_not_print_pri_messages): message("=== gtest appended ===")

    isEmpty(VLIBS_DIR): error("vcat: Need VLIBS_DIR correct path.")


    #<<< Start your code here -----------------------------------------------------------
    LIBS += -lgtest
    LIBS += -lpthread
    #>>> Stop your code here ------------------------------------------------------------
}
# gtest.pri
#========================================================================================


