#include "impl_vlog/error.h"

#include "impl_vlog/entry.h"
#include "vcat.h"
#include "vlog.h"

using namespace impl_vlog;

//=======================================================================================
error::error( position_fix && pos )
    : _pos ( pos )
{
    // Включаем флаги по умолчанию.
    _stream << std::showbase << std::boolalpha;
}
//=======================================================================================
//  Ой, здесь магия...
//  Копирование происходит при развертке стека из-за исключения.
//  Соответственно, в момент копирования сообщение составлено. Будем это сообщение
//  бросать в систему логирования. Но не через логер, а напрямую (чтобы не захватывать
//  контекст метода копирования).
error::error( const error& other )
    : impl_vcat::vcat_iface<error>()
    , _pos        ( other._pos        )
    , _sealed     ( other._sealed     )
    , _sealed_msg ( other._sealed_msg )
{
    _seal_msg( other._stream.str(), other._delimiter_has_been_added() );

    auto ent = entry( _pos, entry::Level::Fatal, _sealed_msg, {} );

    vlog::_execute( ent );
}
//=======================================================================================
//  NB! Если перехватывать исключение по catch(...), то не происходит запечатывание
//  сообщения. См. комментарий в заголовке.
const char* error::what() const noexcept
{
    _seal_msg( _stream.str(), _delimiter_has_been_added() );
    return _sealed_msg.c_str();
}
//=======================================================================================
void error::_seal_msg( const std::string& msg, bool delimiter_added ) const
{
    if ( _sealed ) return;
    _sealed = true;

    _sealed_msg = vcat( "VERROR AT ", _pos.place(),
                        '[', _pos.function(), "] ==> ", msg );

    if ( delimiter_added )
        _sealed_msg.back() = '\n';
    else
        _sealed_msg.push_back( '\n' );
}
//=======================================================================================
