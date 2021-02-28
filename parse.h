#ifndef PARSE_H
#define PARSE_H

#include <netinet/in.h>
#include <netinet/ip.h>
#include <net/if.h>
#include <netinet/if_ether.h>
#include <net/ethernet.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <arpa/inet.h>

template <typename T>
struct Parse_T
{
    void parse(T data)
    {

    }
};

#endif // PARSE_H
