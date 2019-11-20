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
    if ( _sealed ) return;
    _sealed = true;

    _sealed_msg = vcat( "VERROR AT ", _pos.place(),
                        '[', _pos.function(), "] ==> ", other._stream.str() );

    if ( other.delimiter_was_added() )
        _sealed_msg.back() = '\n';
    else
        _sealed_msg.push_back( '\n' );

    auto ent = entry( _pos, entry::Level::Fatal, _sealed_msg, {} );

    vlog::_execute( ent );
}
//=======================================================================================
const char* error::what() const noexcept
{
    return _sealed_msg.c_str();
}
//=======================================================================================
