#include "impl_vlog/vlog_error.h"

//=======================================================================================
//      vlog::error
//=======================================================================================
vlog::error::error( const char* file, int32_t line, const char* func )
    : _stamp( vtime_point::now() )
    , _file ( file )
    , _line ( line )
    , _func ( func )
{}
//=======================================================================================
//  Ой, здесь магия...
//  Копирование происходит при развертке стека из-за исключения.
//  Соответственно, в момент копирования сообщение составлено. Будем это сообщение
//  бросать в систему логирования. Но не через логер, а напрямую (чтобы не захватывать
//  контекст метода копирования).
vlog::error::error( const error &other )
    : impl_vcat::vcat_iface<vlog::error>()
    , _stamp( other._stamp )
    , _file ( other._file  )
    , _line ( other._line  )
    , _func ( other._func  )
    , _msg  ( vcat( "VERROR AT [",
                    base_name(other._file), ':', other._line, "][",
                    other._func, "] ==> ", other._msg ) )
{
    if ( other.delimiter_was_added() )
        _msg.back() = '\n';
    else
        _msg.push_back( '\n' );

    auto ent = entry( entry::Level::Fatal, _stamp, _file, _line, _func, _msg, {} );

    _execute( ent );
}
//=======================================================================================
const char* vlog::error::what() const noexcept
{
    return _msg.c_str();
}
//=======================================================================================
//      vlog::error
//=======================================================================================
