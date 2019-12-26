#ifndef VSERIAL_PORT_H
#define VSERIAL_PORT_H

//=======================================================================================
/*  vserial_port    2019-12-26  by elapidae
 *
 *  Предоставляет доступ к последовательному порту.
 *
 *  При открытии не меняет опций передачи, но меняет режим блокировки (O_NONBLOCK),
 *  который восстанавливает при закрытии.
 *
 *  Если после открытия устанавливаются настройки, то порт переводится в режим RAW,
 *  биты в восемь. При закрытии старые настройки не восстанавливаются.
 *
 *  NB! В настройках speed пишем нормальным числом, но оно должно соответствовать
 *  стандартному значению (см. линк ниже). Для установки произвольной скорости есть
 *  какие-то танцы с бубном, в текущей реализации не сделано.
 *
 *  https://www.opennet.ru/man.shtml?topic=cfsetispeed&category=3&russian=0
*/
//=======================================================================================

#include <string>
#include <memory>
#include "vsignal.h"
#include "vterminal_options.h"

//=======================================================================================
class vserial_port final
{
public:

    vsignal<std::string> received;
    vsignal<> err_hang_up;

    explicit vserial_port();
    explicit vserial_port( const std::string& fname );
    ~vserial_port();

    void open( const std::string& fname );
    void close();

    void set_options( const vterminal_options& opt );

    void write( const std::string& data );
    void flush();

    bool is_opened() const;

private:
    class _pimpl; std::unique_ptr<_pimpl> _p;
};
//=======================================================================================


#endif // VSERIAL_PORT_H
