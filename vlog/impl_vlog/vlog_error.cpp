#include "impl_vlog/vlog_error.h"

//=======================================================================================
//      vlog::error
//=======================================================================================
vlog::error::error( const char* file, int32_t line, const char* func )
    : _stamp ( vtime_point::now() )
    , _file  ( file )
    , _line  ( line )
    , _func  ( func )
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
vlog::error::error( const error &other )
    : impl_vcat::vcat_iface<vlog::error>()
    , _stamp ( other._stamp )
    , _file  ( other._file  )
    , _line  ( other._line  )
    , _func  ( other._func  )

    , _sealed ( other._sealed )
    , _sealed_msg( other._sealed_msg )
{
    if ( _sealed ) return;
    _sealed = true;

    _sealed_msg = vcat( "VERROR AT [",
                        base_name(_file), ':', _line, "][",
                        _func, "] ==> ", other._stream.str() );

    if ( other.delimiter_was_added() )
        _sealed_msg.back() = '\n';
    else
        _sealed_msg.push_back( '\n' );

    auto ent = entry( entry::Level::Fatal, _stamp, _file, _line, _func, _sealed_msg, {});

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
