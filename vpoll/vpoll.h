#ifndef VPOLL_H
#define VPOLL_H

//=======================================================================================
class vpoll final
{
public:

    static void poll();

    static void stop();

private:
    vpoll() = delete;
};
//=======================================================================================

#endif // VPOLL_H