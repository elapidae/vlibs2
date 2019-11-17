#include "impl_vlog/vlog_error.h"

//=======================================================================================
//      vlog::error
//=======================================================================================
vlog::error::error( const char* file, int32_t line, const char* func )
    : _info{ vtime_point::now(), file, line, func }
{}
//=======================================================================================
//  Ой, здесь магия...
//  Копирование происходит при развертке стека из-за исключения.
//  Соответственно, в момент копирования сообщение составлено. Будем это сообщение
//  бросать в систему логирования. Но не через логер, а напрямую (чтобы не захватывать
//  контекст метода копирования).
vlog::error::error( const error &other )
    : impl_vcat::vcat_iface<vlog::error>()
    , _info{ other._info }
    , _sealed( other._sealed )
    , _sealed_msg( other._sealed_msg )
{
    if ( _sealed ) return;

    _sealed_msg = vcat( "VERROR AT [",
                        base_name(other._info._file), ':', other._info._line, "][",
                        other._info._func, "] ==> ", _stream.str() );

    if ( other.delimiter_was_added() )
        _sealed_msg.back() = '\n';
    else
        _sealed_msg.push_back( '\n' );

    auto ent = entry( entry::Level::Fatal, _info._stamp, _info._file, _info._line, _info._func, _sealed_msg, {} );

    _execute( ent );
}
//=======================================================================================
const char* vlog::error::what() const noexcept
{
    return _sealed_msg.c_str();
}
//=======================================================================================
//      vlog::error
//=======================================================================================
