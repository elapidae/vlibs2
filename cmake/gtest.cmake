#########################################################################################
##  GNU Lesser General Public License Usage
##  This file may be used under the terms of the GNU Lesser General Public License
##  version 3 as published by the Free Software Foundation and appearing in the file
##  LICENSE.LGPL3 included in the packaging of this file. Please review the following
##  information to ensure the GNU Lesser General Public License version 3 requirements
##  will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
#########################################################################################
include_guard()

#========================================================================================
message( STATUS "About to find gtest library..." )

find_library( GTEST_LIBRARY NAMES libgtest.a PATHS ${EXT_LIBS_PATH} )
set( V_LIBRARIES ${V_LIBRARIES} ${GTEST_LIBRARY} )
set( V_LIBRARIES ${V_LIBRARIES} -lgtest )
unset( GTEST_LIBRARY )

include( "${VLIBS_DIR}/cmake/threads.cmake" )

message( STATUS "gtest library added to V_LIBRARIES" )
#========================================================================================
