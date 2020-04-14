#include "profile.h"

#include <unordered_map>
#include <assert.h>
#include <algorithm>
#include <iostream>
#include <iomanip>


using namespace impl_vprofile;
using namespace std;
using namespace std::chrono;

//=======================================================================================
static std::unordered_map<int,profile::point> map;
static int _next_id = 0;
int profile::_register_id( const char *file, const char *func, int line )
{
    point p;
    p.id = ++_next_id;
    p.file = file;
    p.func = func;
    p.line = line;

    map.insert( {p.id,p} );

    return p.id;
}
//=======================================================================================
void profile::_append( int id, const clock::duration& dur )
{
    auto it = map.find( id );
    assert( it != map.end() );
    (*it).second.duration += dur;
}
//=======================================================================================
static bool greater_point( const profile::point& p1, const profile::point& p2 )
{
    return p1.duration > p2.duration;
}
//---------------------------------------------------------------------------------------
profile::point::vector profile::current()
{
    point::vector res;
    for ( auto &rec: map )
    {
        res.push_back( rec.second );
    }
    std::sort( res.begin(), res.end(), greater_point );
    return res;
}
//=======================================================================================
profile::profile( int id )
    : _id( id )
    , _from( clock::now() )
{}
//=======================================================================================
profile::~profile()
{
    auto to = clock::now();
    auto dur = to - _from;
    _append( _id, dur );
}
//=======================================================================================

//=======================================================================================
namespace impl_vprofile
{
    ostream& operator << ( ostream& os, const profile::point& p )
    {
        auto ns = duration_cast<nanoseconds>( p.duration );
        double sec = ns.count() / 1e9;

        os << left << setw(72) << p.func << sec;

        return os;
    }
}
//=======================================================================================

//=======================================================================================
struct result_printer final
{
    ~result_printer()
    {
        auto list = profile::current();
        if ( list.empty() ) return;

        cout << "===================================== PROFILE "
                "=====================================\n";
        cout << left << setw(72) << "FUNCTION" << "TIME\n";
        cout << "-----------------------------------------------"
             << "------------------" << "------------------";
        for ( auto&& point: list )
        {
            cout << point << '\n';
        }
        cout << "==============================================="
                "==================" << "=================="
                << endl;
    }
}
printer;
//=======================================================================================
