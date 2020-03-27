#ifndef VAPPLICATION_H
#define VAPPLICATION_H

#include <vector>
#include <string>
#include "vinvoke_iface.h"

/*
 * TODO: Сделать полноценный pid-locker, чтобы захватывать атомарно.
*/

//=======================================================================================
class vapplication final : public vinvoke_iface
{
public:
    size_t tasks_count() const override;
    void   tasks_clear() override;

    static void poll();
    static void stop();

    class args_parser;

    static pid_t pid();

private:
    void _invoke( task_type && f ) override;
};
//=======================================================================================
class vapplication::args_parser
{
public:
    using str  = std::string;
    using cstr = const str&;

    args_parser( int argc, char const * const * const argv );

    str app_name() const;
    str app_path() const;
    str full_app() const;

    bool has( cstr val )    const;
    str get_std( cstr key ) const;

private:
    std::vector<str> _args;
};
//=======================================================================================

#endif // VAPPLICATION_H
