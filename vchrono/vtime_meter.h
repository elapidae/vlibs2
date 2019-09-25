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

/****************************************************************************************
 *
 *  UPD 2019-09-25  by elapidae
 *
 *  Конструкторы:
 *  vtime_meter();                          // запускает сразу же
 *  vtime_meter( vtime_meter::stopped );    // ждет restart() / unpause()
 *
 *  Duration elapsed() const;               // сколько насчитал
 *  Duration restart();                     // сколько насчитал и перезапуск счетчика.
 *
 *  Duration pause();                       // сколько насчитал и приостанови счет.
 *  Duration unpause();                     // сколько насчитал и продолжи счет.
 *
 *  Duration есть наследник std::chrono::duration с методами:
 *          std::chrono::seconds       sec() const;
 *          std::chrono::milliseconds  ms()  const;
 *          std::chrono::microseconds  us()  const;
 *          std::chrono::nanoseconds   ns()  const;
 *
 *  Чтобы было удобнее снимать показания, например:
 *  vdeb << "elapsed: " << meter.restart().ms() << "ms.";
 *
****************************************************************************************/


#ifndef VTIME_METER_H
#define VTIME_METER_H

#include "vchrono_impl/time_meter.h"

//=======================================================================================
using vtime_meter_system          = impl::time_meter<std::chrono::system_clock>;
using vtime_meter_steady          = impl::time_meter<std::chrono::steady_clock>;
using vtime_meter_high_resolution = impl::time_meter<std::chrono::high_resolution_clock>;
//=======================================================================================
using vtime_meter = vtime_meter_steady;
//=======================================================================================


#endif // VTIME_METER_H
