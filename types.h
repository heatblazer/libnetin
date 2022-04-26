#ifndef TYPES_H
#define TYPES_H
#include "defs.h"
#include "tjson.hpp"


struct EthL4
{
    enum {
        TCP,
        UDP,
        UNKNOWN
    } type;

    const   struct ether_header* ethernetHeader;
    const struct ip* ipHeader;
    const struct udphdr* udpHeader;
    const struct tcphdr* tcpHeader;
    char sourceIP[INET_ADDRSTRLEN];
    char destIP[INET_ADDRSTRLEN];
};


template <typename T, typename CrtpBase>
struct IParseable
{
    typedef  T type;

    type value;

    bool Valid;

    IParseable() : Valid{false} {};

    IParseable(const T& ref) : value{ref}, Valid{false} { }

    CrtpBase& impl()
    {
        return static_cast<CrtpBase&>(*this);
    }

    /**
    template<typename U>
    bool Parse(U&& uref)
    {
        impl().parse(uref);
    }*/

    tjson::JsonBuilder jsonb;
};

struct Result_t
{
    enum TypeRFC
    {
        STUN,
        RTP,
        RTCP,
        RTSP,
        TURN,
        T38,
        UNKNOWN
    };

    const u_char* data;

    struct pcap_pkthdr out;

    TypeRFC type;

};


#endif // TYPES_H
