#ifndef VAPPLICATION_H
#define VAPPLICATION_H

//#include <memory>
#include "vinvoke_iface.h"


//=======================================================================================
class _vapplication;
extern _vapplication vapplication;
//=======================================================================================


//=======================================================================================
//      Implementation
//=======================================================================================
class _vapplication final : public vinvoke_iface
{
public:
    ~_vapplication();

    void stop();

private:
    _vapplication();

    void _invoke( func_invokable && f ) override;

    friend _vapplication _init();

    class _pimpl;
    //std::unique_ptr<_pimpl> _p;
    _pimpl *_p;
};
//=======================================================================================


#endif // VAPPLICATION_H
