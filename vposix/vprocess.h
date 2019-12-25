#ifndef VPROCESS_H
#define VPROCESS_H

#include <memory>
#include <vector>
#include "vsignal.h"

class vprocess final
{
public:
    using args_list = std::vector<std::string>;

    vprocess();
    ~vprocess();

    vsignal<std::string> cerr;
    vsignal<std::string> cout;
    vsignal<> cerr_closed;
    vsignal<> cout_closed;

    void cin( const std::string& in );

    //  Режет по пробелам.
    //  NB! Не делает экранирование, просто режет. Метод для удобства простый команд.
    void exec_simple( std::string cmd_with_args );

    void exec( std::string cmd, args_list args = {} );

    bool is_processing() const;
    bool is_finished()   const;
    void wait();

//    int  child_pid()     const;
//    void kill();
//    void kill9();

private:
    class _pimpl; std::unique_ptr<_pimpl> _p;
};

#endif // VPROCESS_H
