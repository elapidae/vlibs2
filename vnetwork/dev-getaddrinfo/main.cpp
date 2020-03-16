#include <iostream>
#include <netdb.h>

#include "vlog.h"
#include "vsocket_address.h"
#include "vbyte_buffer_view.h"
#include "vcat.h"

using namespace std;

//(const char *__restrict __name,
//			const char *__restrict __service,
//			const struct addrinfo *__restrict __req,
//			struct addrinfo **__restrict __pai);

string protocol(int proto)
{
    if ( proto == IPPROTO_TCP ) return "PROT_TCP";
    if ( proto == IPPROTO_UDP ) return "PROT_UDP";
    if ( proto == IPPROTO_IP  ) return "PROT_IP_";
    throw verror << "Bad proto" << proto;
}

int main()
{

    struct addrinfo *addr = nullptr;
    //auto res = getaddrinfo( "goo.gl", nullptr, nullptr, &addr );
    //auto res = getaddrinfo( "goo.gl", "https", nullptr, &addr );
    //auto res = getaddrinfo( "yandex.ru", nullptr, nullptr, &addr );
    //auto res = getaddrinfo( "192.168.0.25", nullptr, nullptr, &addr );
    //auto res = getaddrinfo( nullptr, "http", nullptr, &addr );
    //auto res = getaddrinfo( "goo.gl", "imap", nullptr, &addr );
    auto res = getaddrinfo( nullptr, "ipc", nullptr, &addr );

    if ( res != 0 ) throw verror( gai_strerror(res) );

    auto cur = addr;
    while ( cur )
    {
        vdeb << (cur->ai_family == AF_INET
                    ? "ip4"
                    : cur->ai_family == AF_INET6
                        ? "ip6"
                        : throw verror)

             //<< "name" << cur->ai_canonname
             << vsocket_address::from_raw_sockaddr( cur->ai_addr,
                                           cur->ai_addrlen,
                                           cur->ai_family ).str()
             << protocol(cur->ai_protocol)

             << "stype:" << (cur->ai_socktype == SOCK_STREAM ? "TCP"
                                : cur->ai_socktype == SOCK_DGRAM ? "UDP"
                                    : vcat("<",cur->ai_socktype,">").str() )

             << hex << "flags:" << cur->ai_flags
             ;

        cur = cur->ai_next;
    }

    freeaddrinfo(addr);

    //AI_V4MAPPED
    //AI_PASSIVE

    return 0;
//SOL_SOCKET


    cout << "Hello World!" << endl;

    //auto h = gethostbyname( "yandex.ru" );
    auto h = gethostbyname( "goo.gl" );

    if (!h) throw verror;

    vdeb << "can name:" << h->h_name;

    auto al = h->h_aliases;
    while( *al )
    {
        string a(*al++);
        vdeb << "alias:" << a;
    }
    vdeb << "type:" << (h->h_addrtype == AF_INET ? 4:6) << ",len:" << h->h_length;

    auto addrs = h->h_addr_list;
    while ( *addrs )
    {
        //in_addr
        vbyte_buffer_view v(*addrs++, h->h_length);
        vdeb << v.u32_BE();
    }

    return 0;
}
