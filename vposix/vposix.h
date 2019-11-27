/****************************************************************************************
**
**  VLIBS codebase, NIIAS
**
**  GNU Lesser General Public License Usage
**  This file may be used under the terms of the GNU Lesser General Public License
**  version 3 as published by the Free Software Foundation and appearing in the file
**  LICENSE.LGPL3 included in the packaging of this file. Please review the following
**  information to ensure the GNU Lesser General Public License version 3 requirements
**  will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
****************************************************************************************/

#ifndef VPOSIX_H
#define VPOSIX_H

//=======================================================================================
/*
 *  2019-11-19      by elapidae
 *
 *  Здесь все подключаем, даем дайджест функций.
*/
//=======================================================================================

//  errno не включается сюда, потому что с ним, по хорошему, надо закончить работать
//  внутри имплементаций.
//#include "impl_vposix/wrap_errno.h"

//=======================================================================================

//  int     open
//  int     open_write_append
#include "impl_vposix/wrap_fcntl.h"

//=======================================================================================

//  void    file_rename
#include "impl_vposix/wrap_stdio.h"

//=======================================================================================

//  off_t   file_size
//  bool    file_exists
//  void    make_path
#include "impl_vposix/wrap_sys_stat.h"

//=======================================================================================

//  void    close
//  void    write
#include "impl_vposix/wrap_unistd.h"

//=======================================================================================

#endif // VPOSIX_H
