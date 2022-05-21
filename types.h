#ifndef TYPES_H
#define TYPES_H
#include "defs.h"
#include "tjson.hpp"



#define WRITE_BASE_INFO(X)                                                                              \
                                    jsonb.add(tjson::JsonField{ "srcip", eth.sourceIP});                  \
                                   jsonb.add(tjson::JsonField{ "dstip", eth.destIP });                  \
                                   jsonb.add(tjson::JsonField{ "srcPort", ntohs((X)->source); \
                                   jsonb.add(tjson::JsonField{ "dstPort", ntohs((X)->dest);


struct EthL4
{
    enum {
        TCP,
        UDP,
        UNKNOWN
    } type;

    enum {
        NOP = 0x1,
        MAX_SEG_SIZE = 0x2,
        WINDOW_SCALE = 0x3,
        SACK = 0x4,
        TIMESTAMP = 0x8,
        EOL = 0x00
    } tcp_opt_kind;

    const   struct ether_header* ethernetHeader;
    const struct ip* ipHeader;
    const struct udphdr* udpHeader;
    const struct tcphdr* tcpHeader;
    unsigned char* options;
    size_t options_len;
    size_t payload_len;
    char sourceIP[INET_ADDRSTRLEN];
    char destIP[INET_ADDRSTRLEN];
    uint16_t sourcePort;
    uint16_t destPort;
};


template <typename T, typename CrtpBase>
struct IParseable
{
    typedef  T type;

    type value;

    bool Valid;

    IParseable() = delete;

    IParseable(const IParseable& other) = delete;

    IParseable(IParseable&& other) = delete;

    explicit IParseable(const T& ref) : value{ref}, Valid{false} { }

    CrtpBase& impl()
    {
        return static_cast<CrtpBase&>(*this);
    }

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
        MQTT,
        WebSocket,
        UNKNOWN
    };

    const u_char* data;

    struct pcap_pkthdr out;

    TypeRFC type;

};


#endif // TYPES_H
