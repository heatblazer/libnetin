#ifndef TYPES_H
#define TYPES_H
#include "tjson.hpp"
#include <pcap/pcap.h>

#include <netinet/in.h>
#include <netinet/ip.h>
#include <net/if.h>
#include <netinet/if_ether.h>
#include <net/ethernet.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <arpa/inet.h>

struct EthL4
{
    enum {
        TCP,
        UDP,
        UNKNOWN
    } type;

    const struct ether_header* ethernetHeader;
    const struct ip* ipHeader;
    const struct udphdr* udpHeader;
    const struct tcphdr* tcpHeader;
    char sourceIP[INET_ADDRSTRLEN];
    char destIP[INET_ADDRSTRLEN];

};


template <typename T>
struct IParseable
{
    typedef  T type;

    type value;

    bool Valid;

    IParseable() {};

    IParseable(const T& ref) : value{ref}, Valid{false} { }

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
        UNKNOWN
    };

    const u_char* data;

    struct pcap_pkthdr out;

    TypeRFC type;

};


#endif // TYPES_H
