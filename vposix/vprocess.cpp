#include "vprocess.h"

#include <unistd.h>                     // STDIN_FILENO, ...
#include <sys/resource.h>               // for struct rusage;

#include <sys/wait.h>
#include <algorithm>
#include "impl_vposix/linux_call.h"

#include "impl_vposix/wrap_unistd.h"
#include "impl_vposix/pipe.h"
#include "vlog.h"
#include "vbyte_buffer.h"


using namespace impl_vposix;

//=======================================================================================
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
class vprocess::_pimpl final
{
public:

    _pimpl( vprocess *owner, const std::string& cmd, const args_list& args );
    ~_pimpl();

    void init_as_parent();
    void init_as_child( const std::string& cmd, const args_list& args );

    void wait();

    vprocess *owner;
    impl_vposix::pipe in, out, err;

    int pid;

    safe_fd clone_in, clone_out, clone_err;
};
#pragma GCC diagnostic pop
//---------------------------------------------------------------------------------------
vprocess::_pimpl::_pimpl( vprocess* owner, const std::string& cmd, const args_list& args)
    : owner( owner )
{
    pid = wrap_unistd::fork();

    if ( pid > 0 )
        init_as_parent();
    else
        init_as_child( cmd, args );
}
//---------------------------------------------------------------------------------------
vprocess::_pimpl::~_pimpl()
{
    wait();
}
//---------------------------------------------------------------------------------------
void vprocess::_pimpl::init_as_parent()
{
    in.close_read();
    out.close_write();
    err.close_write();

    out.has_read += [&](std::string str){ owner->cout(str); };
    err.has_read += [&](std::string str){ owner->cerr(str); };

    out.read_has_closed += [&]{ owner->cout_closed(); };
    err.read_has_closed += [&]{ owner->cerr_closed(); };

    out.poll_read();
    err.poll_read();
}
//---------------------------------------------------------------------------------------
void vprocess::_pimpl::init_as_child( const std::string& cmd, const args_list& args )
{
    in.dup_read   ( STDIN_FILENO  );
    out.dup_write ( STDOUT_FILENO );
    err.dup_write ( STDERR_FILENO );

    in.close_write();
    out.close_read();
    err.close_read();


    const char *c_cmd = cmd.c_str();
    std::vector<const char*> c_args;
    c_args.push_back( c_cmd );      // First must be command.
    for( auto& a:args )
        c_args.push_back( a.c_str() );

    c_args.push_back( nullptr );    // Last must be null!

    wrap_unistd::exec( c_cmd, c_args.data() );
}
//---------------------------------------------------------------------------------------
void vprocess::_pimpl::wait()
{
    struct rusage age;
    int status;
    linux_call::check( ::wait4, pid, &status, 0, &age );

    auto o = out.do_read();
    if ( !o.empty() ) owner->cout( o );

    auto e = err.do_read();
    if ( !e.empty() ) owner->cerr( e );
}
//=======================================================================================


//=======================================================================================
vprocess::vprocess()
{}
//=======================================================================================
vprocess::~vprocess()
{}
//=======================================================================================
void vprocess::cin( const std::string& in )
{
    if ( !_p ) throw verror << "vprocess not run for cin.";

    _p->in.write( in );
}
//=======================================================================================
void vprocess::exec_simple( std::string cmd_with_args )
{
    auto list = vbyte_buffer(cmd_with_args).split_by_spaces();
    if ( list.empty() ) return;

    exec( list.at(0), {list.begin() + 1, list.end()} );
}
//=======================================================================================
void vprocess::exec( std::string cmd, vprocess::args_list args )
{
    _p.reset( new _pimpl(this, cmd, args) );
}
//=======================================================================================
bool vprocess::is_processing() const
{
    return bool(_p);
}
//=======================================================================================
bool vprocess::is_finished() const
{
    return !is_processing();
}
//=======================================================================================
void vprocess::wait()
{
    if ( !_p ) throw verror << "vprocess not run for cin.";
    _p.reset();
}
//=======================================================================================
void vprocess::kill()
{
    if ( !_p )
    {
        vwarning << "Kill cannot send to empty process.";
        return;
    }
    ::kill( _p->pid, SIGTERM );
}
//=======================================================================================
void vprocess::force_kill()
{
    if ( !_p )
    {
        vwarning << "Kill cannot send to empty process.";
        return;
    }
    ::kill( _p->pid, SIGKILL );
}
//=======================================================================================

