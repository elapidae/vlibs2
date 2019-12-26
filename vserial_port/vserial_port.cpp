#include "vserial_port.h"

#include "impl_vposix/safe_fd.h"
#include "impl_vposix/wrap_termios.h"
#include "impl_vposix/wrap_unistd.h"
#include "impl_vposix/wrap_fcntl.h"
#include "vlog.h"

#include <fcntl.h> // need for flags O_*...

using namespace impl_vposix;


//=======================================================================================
class vserial_port::_pimpl : public epoll_receiver
{
public:
    _pimpl( vserial_port *own );
    void on_events(events) override;

    void open( const std::string& fname );
    void close();

    void apply_opts( const vterminal_options& opts );

    void write( const std::string& data );
    void flush();
    bool is_opened() const;

private:
    safe_fd fd;

    vserial_port * const owner;
    bool nonblock = false;
};
//=======================================================================================
vserial_port::_pimpl::_pimpl( vserial_port *own )
    : owner( own )
{}
//=======================================================================================
void vserial_port::_pimpl::on_events( epoll_receiver::events e )
{
    e.take_read();

    if ( e.take_error() )
    {
        if ( e.take_hang_up() )
        {
            fd.close();
            owner->err_hang_up();
        }
        e.check_empty();
        return;
    }

    e.check_empty();

    auto data = wrap_unistd::read(fd);
    if ( !data.empty() )
        owner->received( data );
}
//=======================================================================================
void vserial_port::_pimpl::open( const std::string& fname )
{
    close();

    int flags = O_RDWR| O_NOCTTY | O_EXCL | O_CLOEXEC;
    //  O_NONBLOCK следует сначала запомнить, чтобы потом можно было восстановить.

    fd = wrap_fcntl::open( fname, flags );

    nonblock = wrap_fcntl::get_nonblock( fd );
    wrap_fcntl::set_nonblock( fd, true );

    wrap_termios::set_tio_exclusive( fd, true );

    fd.poll_add_read( this );
}
//=======================================================================================
void vserial_port::_pimpl::close()
{
    if ( !fd.has() ) return;

    wrap_termios::set_tio_exclusive( fd, false );
    wrap_fcntl::set_nonblock( fd, nonblock );
    fd.close();
}
//=======================================================================================
void vserial_port::_pimpl::apply_opts( const vterminal_options& opts )
{
    opts._store_as_binary( fd );
}
//=======================================================================================
void vserial_port::_pimpl::write( const std::string& data )
{
    wrap_unistd::write( fd, data );
}
//=======================================================================================
void vserial_port::_pimpl::flush()
{
    wrap_termios::flush( fd );
}
//=======================================================================================
bool vserial_port::_pimpl::is_opened() const
{
    return fd.has();
}
//=======================================================================================


//=======================================================================================
vserial_port::vserial_port()
    : _p( new _pimpl(this) )
{}
//=======================================================================================
vserial_port::vserial_port( const std::string& fname )
    : _p( new _pimpl(this) )
{
    open( fname );
}
//=======================================================================================
vserial_port::~vserial_port()
{}
//=======================================================================================
void vserial_port::open( const std::string& fname )
{
    _p->open( fname );
}
//=======================================================================================
void vserial_port::close()
{
    _p->close();
}
//=======================================================================================
void vserial_port::set_options( const vterminal_options& opt )
{
    if ( !is_opened() )
    {
        vwarning << "Try to set options to closed serial port.";
        return;
    }
    _p->apply_opts( opt );
}
//=======================================================================================
void vserial_port::write( const std::string& data )
{
    if ( !is_opened() )
    {
        vwarning << "write: serial port is not open.";
        return;
    }
    _p->write( data );
}
//=======================================================================================
void vserial_port::flush()
{
    _p->flush();
}
//=======================================================================================
bool vserial_port::is_opened() const
{
    return _p->is_opened();
}
//=======================================================================================
